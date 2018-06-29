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
	m_wrapped = reinterpret_cast<decltype(m_wrapped)>(GUI_CreateDialog(
		0,
		height, alignment,
		title,
		0, 0,
		keyboard
	));

	// Save the old vtable
	m_oldVTable = m_wrapped->vtable;

	// Copy the pre-existing vtable
	memcpy(&m_vtable, m_wrapped->vtable, sizeof(m_vtable));

	m_vtable.me = this;
	m_vtable.OnEvent = OnEvent_Wrap;

	m_wrapped->vtable = &m_vtable;
}

int GUIDialog::OnEvent_Wrap(struct GUIDialog_Wrapped *dialog, void *eventInfo) {
	return dialog->vtable->me->OnEvent(dialog, eventInfo);
}

int GUIDialog::OnEvent(struct GUIDialog_Wrapped *dialog, void *eventData) {
	return m_oldVTable->OnEvent(dialog, eventData);
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
	m_wrapped->vtable->AddElement(m_wrapped, element.GetWrapped(), 0);
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
	m_wrapped = reinterpret_cast<decltype(m_wrapped)>(GUI_CreateLabel(
		0,
		x, y, text,
		0,
		flags,
		0, 
		textColor, backgroundColor, showShadow, shadowColor,
		0
	));
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
	m_wrapped = reinterpret_cast<decltype(m_wrapped)>(GUI_CreateRadioButton(
		0,
		x, y,
		text,
		0,
		flags,
		0, 0
	));
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
	m_wrapped = reinterpret_cast<decltype(m_wrapped)>(GUI_CreateTextBox(
		0,
		x, y, width,
		text,
		0,
		flags, maxLength, countLengthByBytes
	));
}

void *GUITextBox::GetWrapped() {
	return m_wrapped;
}
