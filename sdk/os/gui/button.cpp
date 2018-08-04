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
 * @param eventID The ID to use for events relating to this button.
 */
GUIButton::GUIButton(
	uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
	const char *text,
	uint16_t eventID
) : GUIButton(
	leftX, topY, rightX, bottomY, text, eventID,
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
 * @param eventID The ID to use for events relating to this button.
 * @param flags A bitfield of flags specified by bitwise-ORing members of
 * the @ref Flag enum.
 */
GUIButton::GUIButton(
	uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
	const char *text,
	uint16_t eventID, int flags
) {
	uint16_t bounds[4] = {
		leftX, topY,
		rightX, bottomY
	};

	m_wrapped = GUIButton_ctor(
		0,
		bounds, text, eventID,
		flags, 0
	);
}
