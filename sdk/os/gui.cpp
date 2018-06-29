#include <sdk/os/gui.hpp>
#include <sdk/os/mem.hpp>

/**
 * Creates a dialog.
 * 
 * @param height The height of the dialog.
 * @param alignment The screen position to align the dialog with.
 * @param title The string to display in the title bar of the dialog.
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
	m_oldVTable = m_wrapped->vtable;

	// Copy the pre-existing vtable
	memcpy(&m_vtable, m_wrapped->vtable, sizeof(m_vtable));

	m_vtable.me = this;
	m_vtable.OnEvent = OnEvent_Wrap;

	m_wrapped->vtable = &m_vtable;
}

int GUIDialog::OnEvent_Wrap(struct GUIDialog_Wrapped *dialog, struct GUIDialog_OnEvent_Data *event) {
	return dialog->vtable->me->OnEvent(dialog, event);
}

int GUIDialog::OnEvent(struct GUIDialog_Wrapped *dialog, struct GUIDialog_OnEvent_Data *event) {
	return m_oldVTable->OnEvent(dialog, event);
}

/**
 * Returns the X position of the left edge of the dialog body, in pixels.
 *
 * @return The X position of the left edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetLeftX() {
	return m_wrapped->leftX;
}

/**
 * Returns the Y position of the top edge of the dialog body, in pixels.
 *
 * @return The Y position of the top edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetTopY() {
	return m_wrapped->topY;
}

/**
 * Returns the X position of the right edge of the dialog body, in pixels.
 *
 * @return The X position of the right edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetRightX() {
	return m_wrapped->rightX;
}

/**
 * Returns the Y position of the bottom edge of the dialog body, in pixels.
 *
 * @return The Y position of the bottom edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetBottomY() {
	return m_wrapped->bottomY;
}

/**
 * Adds a GUI element to the dialog.
 * 
 * @param element The GUI element to add.
 */
void GUIDialog::AddElement(GUIElement &element) {
	m_wrapped->vtable->AddElement(m_wrapped, element.GetWrapped(), 1);
}

/**
 * Refreshes the dialog, redrawing all components.
 */
void GUIDialog::Refresh() {
	m_wrapped->vtable->Refresh(m_wrapped);
}

/**
 * Presents the dialog to the user. Blocks until the dialog is closed.
 */
void GUIDialog::ShowDialog() {
	m_wrapped->vtable->ShowDialog(m_wrapped);
}

void *GUIDialog::GetWrapped() {
	return m_wrapped;
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
 * @param leftX The X coordinate of the left edge of the button, in pixels and
 * relative to the top left of the display.
 * @param topY The Y coordinate of the top edge of the button, in pixels and
 * relative to the top left of the display.
 * @param rightX The X coordinate of the right edge of the button, in pixels and
 * relative to the top left of the display.
 * @param bottomY The Y coordinate of the bottom edge of the button, in pixels
 * and relative to the top left of the display.
 * @param text The text to display on the button.
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
 * @param leftX The X coordinate of the left edge of the button, in pixels and
 * relative to the top left of the display.
 * @param topY The Y coordinate of the top edge of the button, in pixels and
 * relative to the top left of the display.
 * @param rightX The X coordinate of the right edge of the button, in pixels and
 * relative to the top left of the display.
 * @param bottomY The Y coordinate of the bottom edge of the button, in pixels
 * and relative to the top left of the display.
 * @param text The text to display on the button.
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

void *GUIButton::GetWrapped() {
	return m_wrapped;
}

/**
 * Creates a label.
 * 
 * @param x The X position to place the label at, in pixels and relative to
 * the top left of the display.
 * @param y The Y position to place the label at, in pixels and relative to
 * the top left of the display.
 * @param text The text to display as the label.
 */
GUILabel::GUILabel(int x, int y, const char *text) : GUILabel(
	x, y, text, 0, 0, 0, false, 0
) {

}

/**
 * Creates a label.
 * 
 * @param x The X position to place the label at, in pixels and relative to
 * the top left of the display.
 * @param y The Y position to place the label at, in pixels and relative to
 * the top left of the display.
 * @param text The text to display as the label.
 * @param flags A bitfield of flags specified by bitwise-ORing members of
 * the @ref Flag enum.
 * @param textColor A pointer to an RGB565 color to use for the text, or 0
 * for the default color.
 * @param backgroundColor A pointer to an RGB565 color to use for the
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
 * @param x The X position to place the label at, in pixels and relative to
 * the top left of the display.
 * @param y The Y position to place the label at, in pixels and relative to
 * the top left of the display.
 * @param text The text to display as the label.
 * @param flags A bitfield of flags specified by bitwise-ORing members of
 * the @ref Flag enum.
 * @param textColor A pointer to an RGB565 color to use for the text, or 0
 * for the default color.
 * @param backgroundColor A pointer to an RGB565 color to use for the
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

void *GUILabel::GetWrapped() {
	return m_wrapped;
}

/**
 * Creates a radio button.
 * 
 * @param x The X position to place the radio button at, in pixels and
 * relative to the top left of the display.
 * @param y The Y position to place the radio button at, in pixels and
 * relative to the top left of the display.
 * @param text The text to display to the right-hand side of the radio
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

void *GUIRadioButton::GetWrapped() {
	return m_wrapped;
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
 * @param x The X position to place the text box at, in pixels and relative
 * to the top left of the display.
 * @param y The Y position to place the text box at, in pixels and relative
 * to the top left of the display.
 * @param width The width of the textbox, in pixels.
 * @param text A string to pre-populate the text box with, or 0 if the
 * textbox should be initially empty.
 * @param flags A bitfield of flags specified by bitwise-ORing members of
 * the @ref Flag enum.
 * @param maxLength The maximum number of characters/bytes the text box
 * should hold.
 * @param countLengthByBytes True if @p maxLength specifies the maximum
 * number of bytes, and false if it specifies the maximum number of
 * characters.
 */
GUITextBox::GUITextBox(
	int x, int y, int width,
	const char *text,
	int flags, int maxLength, bool countLengthByBytes
) {
	m_wrapped = GUI_CreateTextBox(
		0,
		x, y, width,
		text,
		0,
		flags, maxLength, countLengthByBytes
	);
}

/**
 * Retrieves the text box's current text content.
 * 
 * @return The text box's text, or 0 if the text box is empty.
 */
const char *GUITextBox::GetText() {
	return m_wrapped->text;
}

/**
 * Sets the text box's text.
 * 
 * If the text specified is longer than the maximum string length permitted by
 * the textbox, it is truncated to fit.
 * 
 * @param text The new string for the textbox.
 */
void GUITextBox::SetText(const char *text) {
	m_wrapped->vtable->SetText(m_wrapped, text);
}

void *GUITextBox::GetWrapped() {
	return m_wrapped;
}
