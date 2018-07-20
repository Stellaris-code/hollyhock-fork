#pragma once
#include <stdint.h>
#include "util.hpp"

/// @private
struct GUITextBox_Wrapped_VTable {
	VTABLE_FAKE_ENTRY(32, 0);

	VTABLE_ENTRY(
		void, SetText,
		struct GUITextBox_Wrapped *textBox, const char *text
	);

	// no need to length match
};

/// @private
struct GUITextBox_Wrapped {
	uint8_t unknown0[0x4C];

	struct GUITextBox_Wrapped_VTable *vtable;

	uint8_t unknown1[0x4];

	const char *text;

	uint8_t unknown2[0x48];
};
static_assert(sizeof(struct GUITextBox_Wrapped) == 0xA0);

class GUITextBox : public GUIElement {
public:
	enum Flag {
		/// Enables drawing the text box's outline and background.
		FlagDrawBox = 1 << 3,

		/// Allows the contents of the text box to be modified.
		FlagEditable = 1 << 8
	};

	GUITextBox(
		int x, int y, int width,
		int maxLength, bool countLengthByBytes
	);
	GUITextBox(
		int x, int y, int width,
		const char *text, int maxLength, bool countLengthByBytes
	);

	const char *GetText();
	void SetText(const char *text);
};

/// @cond INTERNAL
extern "C"
struct GUITextBox_Wrapped *GUI_CreateTextBox(
	void *textBox,
	int x, int y, int width,
	const char *text,
	int unknown0,
	int flags,
	int maxLength, bool countLengthByBytes
);
/// @endcond
