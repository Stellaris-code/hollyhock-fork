#pragma once
#include <stdint.h>
#include "util.hpp"

class GUIButton : public GUIElement {
public:
	enum Flag : int {
		/// Allows the button to be pressed.
		FlagEnabled = 1 << 15
	};

	GUIButton(
		uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
		const char *text,
		uint16_t eventType
	);
	GUIButton(
		uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
		const char *text,
		uint16_t eventType, int flags
	);

	/**
	 * Returns the type which will be reported in the event data passed to a
	 * dialog's OnEvent from an @c eventType passed into the constructor for a
	 * @ref GUIButton.
	 * 
	 * @param eventType The @c eventType value passed into the button's
	 * constructor.
	 * @return The type reported in the event data for the specified
	 * @p eventType.
	 */
	static constexpr uint16_t GetEventType(uint16_t eventType) {
		return ((eventType + 8) << 4) | (1 << 3);
	}
};

/// @cond INTERNAL
extern "C"
void *GUI_CreateButton(
	void *button,
	uint16_t bounds[4],
	const char *text,
	uint16_t eventType, int unk0, int unk1
);
/// @endcond
