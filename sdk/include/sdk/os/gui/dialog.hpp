#pragma once
#include <stdint.h>
#include "util.hpp"

// pre-declare the class, since a pointer to it is placed in the vtable
class GUIDialog;

/// @private
struct GUIDialog_Wrapped_VTable {
	/**
	 * Stores the address of the @ref GUIDialog object this vtable belongs to.
	 * Should be set by the constructor of @ref GUIDialog.
	 * 
	 * In the original vtable, the first dword is zero. Since it appears to
	 * never be accessed, and the vtables in the firmware are stored in ROM (i.e
	 * even if the firmware wanted to change the value, it couldn't) we're safe
	 * to use it for our own purposes.
	 * 
	 * That purpose is to link the vtable to an instance of the @ref GUIDialog.
	 * Since we can't populate the vtable with instance function pointers, we
	 * have to use a static function. Without this entry, the static function
	 * would have no idea which @ref GUIDialog instance the function call was
	 * associated with.
	 */
	GUIDialog *me;
	uint32_t fakeentrypadding[2];

	VTABLE_FAKE_ENTRY(1, 0);
	
	// Args: event
	// Return: unknown?
	VTableFunction<int, struct GUIDialog_OnEvent_Data *> OnEvent;

	VTABLE_FAKE_ENTRY(1, 1);

	// Args: element, unknown0
	// unknown0 - always pass 0
	VTableFunction<void, void *, int> AddElement;

	VTABLE_FAKE_ENTRY(4, 2);

	VTableFunction<void> Refresh;

	VTABLE_FAKE_ENTRY(23, 3);

	// Return: dialog result
	VTableFunction<int> ShowDialog;

	VTABLE_FAKE_ENTRY(20, 4);
};

/// @private
struct GUIDialog_Wrapped {
	uint8_t unknown0[0x10];

	// refer to accessors in GUIDialog class for documentation
	uint16_t leftX;
	uint16_t topY;
	uint16_t rightX;
	uint16_t bottomY;

	uint8_t unknown1[0x34];

	struct GUIDialog_Wrapped_VTable *vtable;

	uint8_t unknown2[0x58];
};
static_assert(sizeof(struct GUIDialog_Wrapped) == 0xA8);

struct GUIDialog_OnEvent_Data {
	uint16_t type;

	/// Arbitary data. Usage dependent on event and element type.
	uint16_t data;

	/// The pointer to the internal GUI element class the event refers to. 
	void *element;

	/**
	 * Returns the event ID encoded in this event's @c type. This is the value
	 * which would have been passed in to the @c eventID parameter when the
	 * GUI element was created.
	 * 
	 * @returns The event ID of the event.
	 */
	constexpr uint16_t GetEventID() {
		return (type >> 4) - 8;
	}
};

class GUIDialog : public Wrapped {
public:
	enum Height : int {
		Height25 = 0,
		Height55 = 1,
		Height75 = 2,
		Height95 = 3,
		Height35 = 4,
		Height60 = 5
	};

	enum Alignment : int {
		AlignTop = 0,
		AlignCenter = 1,
		AlignBottom = 2
	};

	enum KeyboardState : int {
		KeyboardStateNone = 0, // 2 gives same effect
		KeyboardStateMath1 = 1, // 3 gives same effect
		KeyboardStateMath2 = 4,
		KeyboardStateMath3 = 5,
		KeyboardStateTrig = 6,
		KeyboardStateVar = 7,
		KeyboardStateABC = 8,
		KeyboardStateCatalog = 9,
		KeyboardStateAdvance = 10,
		KeyboardStateNumber = 11
	};

	enum DialogResult : int {
		DialogResultOK = 0x3EA,
		DialogResultCancel = 0x3EB
	};

	GUIDialog(
		enum Height height, enum Alignment alignment,
		const char* title,
		enum KeyboardState keyboard
	);

	virtual int OnEvent(struct GUIDialog_Wrapped *dialog, struct GUIDialog_OnEvent_Data *event);
	
	uint16_t GetLeftX();
	uint16_t GetTopY();
	uint16_t GetRightX();
	uint16_t GetBottomY();

	void AddElement(GUIElement &element);
	void Refresh();
	DialogResult ShowDialog();

private:
	struct GUIDialog_Wrapped_VTable *m_oldVTable;
	struct GUIDialog_Wrapped_VTable m_vtable;

	static int OnEvent_Wrap(struct GUIDialog_Wrapped *dialog, struct GUIDialog_OnEvent_Data *event);
};

/// @cond INTERNAL
extern "C"
struct GUIDialog_Wrapped *GUIDialog_ctor(
	void *dialog,
	int height, int alignment,
	const char *title,
	int unknown2, int unknown3,
	int keyboard
);
/// @endcond
