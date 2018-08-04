#pragma once
#include <stdint.h>
#include "util.hpp"

class GUILabel : public GUIElement {
public:
	enum Flag : int {
		/// Enables displaying the background color of the label.
		FlagBackground = 1 << 0,

		/**
		 * Allows the label to be selected/brought into focus.
		 * 
		 * When the label is selected, the text and background colors switch.
		 */
		FlagSelectable = 1 << 15
	};

	GUILabel(int x, int y, const char *text);
	GUILabel(
		int x, int y,
		const char *text,
		int flags,
		uint16_t *textColor, uint16_t *backgroundColor
	);
	GUILabel(
		int x, int y,
		const char *text,
		int flags,
		uint16_t *textColor, uint16_t *backgroundColor,
		bool showShadow, uint16_t shadowColor
	);
};

/// @cond INTERNAL
extern "C"
void *GUILabel_ctor(
	void *label,
	int x, int y,
	const char *text,
	int unknown0,
	int flags,
	void *font,
	uint16_t *textColor, uint16_t *backgroundColor,
	bool showShadow, uint16_t shadowColor,
	int unknown1
);
/// @endcond
