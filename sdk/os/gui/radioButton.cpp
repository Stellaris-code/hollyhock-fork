#include <sdk/os/gui.hpp>

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
	m_wrapped = GUIRadioButton_ctor(
		0,
		x, y,
		text,
		0,
		flags,
		0, 0
	);
}
