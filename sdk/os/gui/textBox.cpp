#include <sdk/os/gui.hpp>

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
	m_wrapped = GUITextBox_ctor(
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
	GetWrapped<GUITextBox_Wrapped>()->vtable->SetText(m_wrapped, text);
}
