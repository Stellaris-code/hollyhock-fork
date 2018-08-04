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
		uint16_t eventID
	);
	GUIButton(
		uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
		const char *text,
		uint16_t eventID, int flags
	);
};

/// @cond INTERNAL
extern "C"
void *GUIButton_ctor(
	void *button,
	uint16_t bounds[4],
	const char *text,
	uint16_t eventID, int unk0, int unk1
);
/// @endcond
