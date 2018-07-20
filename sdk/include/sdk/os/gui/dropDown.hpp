#pragma once
#include <stdint.h>
#include "util.hpp"

class GUIDropDownMenuItem;

/// @private
struct GUIDropDownMenu_Wrapped_VTable {
	VTABLE_FAKE_ENTRY(5, 0);

	VTABLE_ENTRY(
		void, AddMenuItem,
		void *dropDownMenu, void *dropDownMenuItem, uint32_t unk0
	);

	VTABLE_FAKE_ENTRY(28, 1);

	VTABLE_ENTRY(
		void, SetScrollBarVisibility,
		void *dropDownMenu, uint32_t visibility
	);
};

/// @private
struct GUIDropDownMenu_Wrapped {
	uint8_t unknown0[0x4C];

	struct GUIDropDownMenu_Wrapped_VTable *vtable;
};

class GUIDropDownMenu : public GUIElement {
public:
	enum ScrollBarVisibility : uint32_t {
		/// Always hide the scroll bar.
		ScrollBarHidden = 0,
		/// Always show the scroll bar.
		ScrollBarAlwaysVisible = 1,
		/// Only show the scroll bar when required.
		ScrollBarVisibleWhenRequired = 2
	};

	GUIDropDownMenu(
		uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
		uint16_t eventID
	);

	void SetScrollBarVisibility(ScrollBarVisibility visibility);
	void AddMenuItem(GUIDropDownMenuItem &dropDownMenuItem);
};

class GUIDropDownMenuItem : public GUIElement {
public:
	enum Flag : int {
		/// Align the text of the item within the drop down to the right.
		FlagTextAlignRight = 1 << 5,
		/// Align the text of the item within the drop down to the left.
		FlagTextAlignLeft = 1 << 6,
		/// Allow the item to be selected.
		FlagEnabled = 1 << 15
	};

	GUIDropDownMenuItem(const char *s, int idx, int flags);
};

/// @cond INTERNAL
extern "C"
void *GUI_CreateDropDownMenu(
	void *dropDownMenu,
	uint16_t bounds[4],
	uint16_t eventID, int flags1
);

extern "C"
void *GUI_CreateDropDownMenuItem(
	void *dropDownMenuItem,
	int unk0, int unk1,
	const char *text,
	int index, int flags,
	int unk2
);
/// @endcond
