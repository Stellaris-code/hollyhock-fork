#include <sdk/os/gui.hpp>

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
	m_wrapped = GUILabel_ctor(
		0,
		x, y, text,
		0,
		flags,
		0, 
		textColor, backgroundColor, showShadow, shadowColor,
		0
	);
}
