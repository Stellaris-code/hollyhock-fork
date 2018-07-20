#include <sdk/os/gui.hpp>

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
