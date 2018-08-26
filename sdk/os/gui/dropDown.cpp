#include <sdk/os/gui.hpp>

/**
 * Creates a drop down menu.
 * 
 * The bounds do not effect the height of the normally visible part of the menu,
 * but are used to set the height of the menu which appears when the drop down
 * menu is selected.
 * 
 * @param leftX,topY,rightX,bottomY The coordinates of the bounding box of the
 * drop down menu, in screen pixels relative to the top left of the display.
 * @param eventID The ID to use for events relating to this drop down menu.
 */
GUIDropDownMenu::GUIDropDownMenu(
	uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
	uint16_t eventID
) {
	uint16_t bounds[] = {leftX, topY, rightX, bottomY};
	m_wrapped = GUIDropDownMenu_ctor(
		nullptr, bounds, eventID, 8
	);
}

/**
 * Sets the visibility of the scroll bar.
 * 
 * @param visibility The visibility of the scroll bar.
 */
void GUIDropDownMenu::SetScrollBarVisibility(ScrollBarVisibility visibility) {
	GetWrapped<GUIDropDownMenu_Wrapped>()->vtable->SetScrollBarVisibility(
		m_wrapped,
		visibility
	);
}

/**
 * Adds an item to the drop down menu.
 * 
 * @param The item to add.
 */
void GUIDropDownMenu::AddMenuItem(GUIDropDownMenuItem &dropDownMenuItem) {
	GetWrapped<GUIDropDownMenu_Wrapped>()->vtable->AddMenuItem(
		m_wrapped,
		dropDownMenuItem.GetWrapped<void>(), 1
	);
}

/**
 * Creates a item for a drop down menu (@ref GUIDropDownMenu).
 * 
 * @param text The item's text.
 * @param index A unique 1-based index for the item.
 * @param flags A bitfield of flags specified by bitwise-ORing members of the
 * @ref Flag enum.
 */
GUIDropDownMenuItem::GUIDropDownMenuItem(const char *text, int index, int flags) {
	m_wrapped = GUIDropDownMenuItem_ctor(
		0, 0, 0, text, index, flags, 0
	);
}
