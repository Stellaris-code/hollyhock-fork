#pragma once
#include "util.hpp"

/// @private
struct GUILongLabel_Wrapped_VTable {
    VTABLE_FAKE_ENTRY(9, 0);

	VTableFunction<void> Refresh;

    VTABLE_FAKE_ENTRY(33, 1);

	// Args: text
	VTableFunction<void, const char *> SetText;
};

/// @private
struct GUILongLabel_Wrapped {
    uint8_t unknown0[0x4C];

    struct GUILongLabel_Wrapped_VTable *vtable;

    uint8_t unknown1[0x5C];
};
static_assert(sizeof(struct GUILongLabel_Wrapped) == 0xAC);

class GUILongLabel : public GUIElement {
public:
    GUILongLabel(
        uint16_t leftX, uint16_t topY, uint16_t rightX, uint16_t bottomY,
        const char *text
    );

    void SetText(const char *text);
    void Refresh();
};

/// @cond INTERNAL
extern "C"
void *GUILongLabel_ctor(void *longLabel, uint16_t bounds[4], int unk0, int unk1, const char *text, int unk2);
/// @endcond
