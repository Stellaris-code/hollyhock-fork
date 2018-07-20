#include <sdk/os/gui.hpp>
#include <sdk/os/mem.hpp>

/**
 * Creates a dialog.
 * 
 * @param height The height of the dialog.
 * @param alignment The screen position to align the dialog with.
 * @param[in] title The string to display in the title bar of the dialog.
 * @param keyboard The keyboard to display when the dialog is shown.
 */
GUIDialog::GUIDialog(
	enum Height height, enum Alignment alignment,
	const char* title,
	enum KeyboardState keyboard
) : m_vtable({}) {
	m_wrapped = GUI_CreateDialog(
		0,
		height, alignment,
		title,
		0, 0,
		keyboard
	);

	// Save the old vtable
	m_oldVTable = GetWrapped<GUIDialog_Wrapped>()->vtable;

	// Copy the pre-existing vtable
	memcpy(&m_vtable, GetWrapped<GUIDialog_Wrapped>()->vtable, sizeof(m_vtable));

	m_vtable.me = this;
	m_vtable.OnEvent = OnEvent_Wrap;

	GetWrapped<GUIDialog_Wrapped>()->vtable = &m_vtable;
}

int GUIDialog::OnEvent_Wrap(struct GUIDialog_Wrapped *dialog, struct GUIDialog_OnEvent_Data *event) {
	// Call the OnEvent function of our class (not of CASIO's one!)
	return dialog->vtable->me->OnEvent(dialog, event);
}

int GUIDialog::OnEvent(struct GUIDialog_Wrapped *dialog, struct GUIDialog_OnEvent_Data *event) {
	// We've overidden the entry for OnEvent in our vtable with one that
	// redirects to this method. Since we've done that, if we want to call the
	// actual base OnEvent method, we use the old vtable we backed up in the
	// constructor.
	return VTABLE_CALL_EXT(
		dialog, m_oldVTable,
		OnEvent,
		event
	);
}

/**
 * Returns the X position of the left edge of the dialog body, in pixels.
 *
 * @return The X position of the left edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetLeftX() {
	return GetWrapped<GUIDialog_Wrapped>()->leftX;
}

/**
 * Returns the Y position of the top edge of the dialog body, in pixels.
 *
 * @return The Y position of the top edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetTopY() {
	return GetWrapped<GUIDialog_Wrapped>()->topY;
}

/**
 * Returns the X position of the right edge of the dialog body, in pixels.
 *
 * @return The X position of the right edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetRightX() {
	return GetWrapped<GUIDialog_Wrapped>()->rightX;
}

/**
 * Returns the Y position of the bottom edge of the dialog body, in pixels.
 *
 * @return The Y position of the bottom edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetBottomY() {
	return GetWrapped<GUIDialog_Wrapped>()->bottomY;
}

/**
 * Adds a GUI element to the dialog.
 * 
 * @param element The GUI element to add.
 */
void GUIDialog::AddElement(GUIElement &element) {
	VTABLE_CALL(
		GetWrapped<GUIDialog_Wrapped>(), vtable,
		AddElement,
		element.GetWrapped<void>(), 1
	);
}

/**
 * Refreshes the dialog, redrawing all components.
 */
void GUIDialog::Refresh() {
	VTABLE_CALL(
		GetWrapped<GUIDialog_Wrapped>(), vtable,
		Refresh
	);
}

/**
 * Presents the dialog to the user. Blocks until the dialog is closed.
 */
void GUIDialog::ShowDialog() {
	VTABLE_CALL(
		GetWrapped<GUIDialog_Wrapped>(), vtable,
		ShowDialog
	);
}

/**
 * Creates a button.
 * 
 * The @c event.type value which arises when the button is pressed is not the
 * value passed in the @p eventType parameter. You must use the
 * @ref GetEventType method to convert the @p eventType parameter passed in here
 * to the @c event.type value. 0 is not a valid value for the @p eventType
 * parameter.
 * 
 * @param leftX,topY,rightX,bottomY The coordinates of the bounding box of the
 * button, in screen pixels relative to the top left of the display.
 * @param[in] text The text to display on the button.
 * @param eventType Part of the type to pass in the event data to the dialog's
 * OnEvent method when the button is pressed.
 */
GUIButton::GUIButton(
	uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
	const char *text,
	uint16_t eventType
) : GUIButton(
	leftX, topY, rightX, bottomY, text, eventType,
	Flag::FlagEnabled
) {
	
}

/**
 * Creates a button.
 *  
 * The @c event.type value which arises when the button is pressed is not the
 * value passed in the @p eventType parameter. You must use the
 * @ref GetEventType method to convert the @p eventType parameter passed in here
 * to the @c event.type value. 0 is not a valid value for the @p eventType
 * parameter.
 * 
 * @param leftX,topY,rightX,bottomY The coordinates of the bounding box of the
 * button, in screen pixels relative to the top left of the display.
 * @param[in] text The text to display on the button.
 * @param eventType The type to pass in the event data to the dialog's OnEvent
 * method when the button is pressed.
 * @param flags A bitfield of flags specified by bitwise-ORing members of
 * the @ref Flag enum.
 */
GUIButton::GUIButton(
	uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
	const char *text,
	uint16_t eventType, int flags
) {
	uint16_t bounds[4] = {
		leftX, topY,
		rightX, bottomY
	};

	m_wrapped = GUI_CreateButton(
		0,
		bounds, text, eventType,
		flags, 0
	);
}

/**
 * Creates a drop down menu.
 * 
 * The bounds do not effect the height of the normally visible part of the menu,
 * but are used to set the height of the menu which appears when the drop down
 * menu is selected.
 * 
 * @param leftX,topY,rightX,bottomY The coordinates of the bounding box of the
 * drop down menu, in screen pixels relative to the top left of the display.
 * @param eventType See @ref GUIButton documentation.
 */
GUIDropDownMenu::GUIDropDownMenu(
	uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
	uint16_t eventType
) {
	uint16_t bounds[] = {leftX, topY, rightX, bottomY};
	m_wrapped = GUI_CreateDropDownMenu(
		nullptr, bounds, eventType, 8
	);
}

/**
 * Sets the visibility of the scroll bar.
 * 
 * @param visibility The visibility of the scroll bar.
 */
void GUIDropDownMenu::SetScrollBarVisibility(ScrollBarVisibility visibility) {
	VTABLE_CALL(
		GetWrapped<GUIDropDownMenu_Wrapped>(), vtable, SetScrollBarVisibility,
		visibility
	);
}

/**
 * Adds an item to the drop down menu.
 * 
 * @param The item to add.
 */
void GUIDropDownMenu::AddMenuItem(GUIDropDownMenuItem &dropDownMenuItem) {
	VTABLE_CALL(
		GetWrapped<GUIDropDownMenu_Wrapped>(), vtable, AddMenuItem,
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
	m_wrapped = GUI_CreateDropDownMenuItem(
		0, 0, 0, text, index, flags, 0
	);
}

/**
 * Creates a label.
 * 
 * @param x,y The position to place the label at, in pixels and relative to the
 * top left of the display.
 * @param[in] text The text to display as the label.
 */
GUILabel::GUILabel(int x, int y, const char *text) : GUILabel(
	x, y, text, 0, 0, 0, false, 0
) {

}

/**
 * Creates a label.
 * 
 * @param x,y The position to place the label at, in pixels and relative to the
 * top left of the display.
 * @param[in] text The text to display as the label.
 * @param flags A bitfield of flags specified by bitwise-ORing members of
 * the @ref Flag enum.
 * @param[in] textColor A pointer to an RGB565 color to use for the text, or 0
 * for the default color.
 * @param[in] backgroundColor A pointer to an RGB565 color to use for the
 * background, or 0 for the default color.
 */
GUILabel::GUILabel(
	int x, int y,
	const char *text,
	int flags,
	uint16_t *textColor, uint16_t *backgroundColor
) : GUILabel(x, y, text, flags, textColor, backgroundColor, false, 0) {

}

/**
 * Creates a label.
 * 
 * @param x,y The position to place the label at, in pixels and relative to the
 * top left of the display.
 * @param[in] text The text to display as the label.
 * @param flags A bitfield of flags specified by bitwise-ORing members of
 * the @ref Flag enum.
 * @param[in] textColor A pointer to an RGB565 color to use for the text, or 0
 * for the default color.
 * @param[in] backgroundColor A pointer to an RGB565 color to use for the
 * background, or 0 for the default color.
 * @param showShadow True if the text shadow should be shown.
 * @param shadowColor An RGB565 color to use for the text shadow.
 */
GUILabel::GUILabel(
	int x, int y,
	const char *text,
	int flags,
	uint16_t *textColor, uint16_t *backgroundColor,
	bool showShadow, uint16_t shadowColor
) {
	m_wrapped = GUI_CreateLabel(
		0,
		x, y, text,
		0,
		flags,
		0, 
		textColor, backgroundColor, showShadow, shadowColor,
		0
	);
}

/**
 * Creates a radio button.
 * 
 * @param x,y The position to place the radio button at, in pixels and relative
 * to the top left of the display.
 * @param[in] text The text to display to the right-hand side of the radio
 * button.
 * @param flags A bitfield of flags specified by bitwise-ORing members of
 * the @ref Flag enum.
 */
GUIRadioButton::GUIRadioButton(
	int x, int y,
	const char *text,
	int flags
) {
	m_wrapped = GUI_CreateRadioButton(
		0,
		x, y,
		text,
		0,
		flags,
		0, 0
	);
}

/**
 * Creates a text box.
 *
 * With an example value for the parameter @p maxLength of 4, the behaviour
 * of @p countLengthByBytes is as follows:
 * - If true, 4 bytes worth of text can be written in the text box. This may
 * be two 2-byte characters, one 2-byte character and two 1-byte characters,
 * etc.
 * - If false, 4 characters worth of text can be written in the text box.
 * This may be four 2-byte characters, one 2-byte character and three 1-byte
 * characters, etc.
 * 
 * @param x,y The position to place the text box at, in pixels and relative to
 * the top left of the display.
 * @param width The width of the textbox, in pixels.
 * @param maxLength The maximum number of characters/bytes the text box
 * should hold.
 * @param countLengthByBytes True if @p maxLength specifies the maximum
 * number of bytes, and false if it specifies the maximum number of
 * characters.
 */
GUITextBox::GUITextBox(
	int x, int y, int width,
	int maxLength, bool countLengthByBytes
) : GUITextBox(
	x, y, width, "", maxLength, countLengthByBytes
) {

}

/**
 * Creates a text box.
 *
 * With an example value for the parameter @p maxLength of 4, the behaviour
 * of @p countLengthByBytes is as follows:
 * - If true, 4 bytes worth of text can be written in the text box. This may
 * be two 2-byte characters, one 2-byte character and two 1-byte characters,
 * etc.
 * - If false, 4 characters worth of text can be written in the text box.
 * This may be four 2-byte characters, one 2-byte character and three 1-byte
 * characters, etc.
 * 
 * @param x,y The position to place the text box at, in pixels and relative to
 * the top left of the display.
 * @param width The width of the textbox, in pixels.
 * @param[in] text A string to pre-populate the text box with, or 0 if the
 * textbox should be initially empty.
 * @param maxLength The maximum number of characters/bytes the text box
 * should hold.
 * @param countLengthByBytes True if @p maxLength specifies the maximum
 * number of bytes, and false if it specifies the maximum number of
 * characters.
 */
GUITextBox::GUITextBox(
	int x, int y, int width,
	const char *text, int maxLength, bool countLengthByBytes
) {
	m_wrapped = GUI_CreateTextBox(
		0,
		x, y, width,
		text,
		0,
		Flag::FlagDrawBox | Flag::FlagEditable,
		maxLength, countLengthByBytes
	);
}

/**
 * Retrieves the text box's current text content.
 * 
 * @return The text box's text, or 0 if the text box is empty.
 */
const char *GUITextBox::GetText() {
	return GetWrapped<GUITextBox_Wrapped>()->text;
}

/**
 * Sets the text box's text.
 * 
 * If the text specified is longer than the maximum string length permitted by
 * the textbox, it is truncated to fit.
 * 
 * @param[in] text The new string for the textbox.
 */
void GUITextBox::SetText(const char *text) {
	VTABLE_CALL(
		GetWrapped<GUITextBox_Wrapped>(), vtable,
		SetText,
		text
	);
}
