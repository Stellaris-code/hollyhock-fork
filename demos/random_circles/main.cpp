#include <sdk/os/input.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/mem.hpp>

#define PIXEL(x, y) (vram[(x) + (y) * width])

uint16_t *vram;
int width, height;
uint16_t lfsr;

void drawCircle(int32_t x0, int32_t y0, int radius) {
	for (int32_t dx = -radius; dx < radius; ++dx) {
		for (int32_t dy = -radius; dy < radius; ++dy) {
			if (dx * dx + dy * dy < radius * radius) {
				int32_t x = x0 + dx;
				int32_t y = y0 + dy;

				if (x < 0 || x > width || y < 0 || y > height) {
					continue;
				}

				PIXEL(x, y) = lfsr;
			}
		}
	}
}

void main() {
	vram = LCD_GetVRAMAddress();
	LCD_GetSize(&width, &height);

	lfsr = 0x453A;

	struct InputEvent event;

	LCD_ClearScreen();
	LCD_Refresh();

	bool running = true;
	while (running) {
		memset(&event, 0, sizeof(event));
		GetInput(&event, 0xFFFFFFFF, 0x10);

		switch (event.type) {
			case EVENT_KEY:
				switch (event.data.key.keyCode) {
				case KEYCODE_POWER_CLEAR:
					running = false;
					break;
				case KEYCODE_0:
					LCD_ClearScreen();
					LCD_Refresh();
					break;
				}
				break;
			case EVENT_TOUCH: {
				int32_t x = event.data.touch_single.p1_x;
				int32_t y = event.data.touch_single.p1_y;

				if (x < 0 || x > width || y < 0 || y > height) {
					break;
				}

				uint16_t radius = (lfsr ^ (lfsr >> 4) ^ (lfsr >> 8) ^ (lfsr >> 12)) & 0xF;
				drawCircle(x, y, radius + 10);

				uint16_t bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
				lfsr = (lfsr >> 1) | (bit << 15);

				LCD_Refresh();
				break;
			}
		}
	}
}
