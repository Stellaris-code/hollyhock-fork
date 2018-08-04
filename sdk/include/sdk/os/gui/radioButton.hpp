#pragma once
#include "util.hpp"

class GUIRadioButton : public GUIElement {
public:
	enum Flag : int {
		/// Causes the radio button to be selected by default.
		FlagSelected = 1 << 2,

		/// Makes the radio button interactive.
		FlagEnabled = 1 << 15
	};

	GUIRadioButton(
		int x, int y,
		const char *text,
		int flags
	);
};

/// @cond INTERNAL
extern "C"
void *GUIRadioButton_ctor(
	void *radioButton,
	int x, int y,
	const char *text,
	int unknown0,
	int flags,
	void *font,
	int unknown2
);
/// @endcond
