#include <sdk/os/gui.hpp>

/**
 * Creates a long label, capable of displaying multiple lines of text.
 * 
 * @param leftX,topY,rightX,bottomY The coordinates of the bounding box of the
 * long label, in screen pixels relative to the top left of the display.
 * @param[in] text The text for the long label to display.
 */
GUILongLabel::GUILongLabel(
    uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
    const char *text
) {
    uint16_t bounds[] = {leftX, topY, rightX, bottomY};
    m_wrapped = GUILongLabel_ctor(0, bounds, 0, 0x441, text, 0x3E8);
}

/**
 * Sets the long label's text.
 * 
 * @param[in] text The new string for the long label.
 */
void GUILongLabel::SetText(const char *text) {
	GetWrapped<GUILongLabel_Wrapped>()->vtable->SetText(
		m_wrapped,
		text
	);
}

/**
 * Refreshes the long label, redrawing it.
 */
void GUILongLabel::Refresh() {
	GetWrapped<GUILongLabel_Wrapped>()->vtable->Refresh(m_wrapped);
}
