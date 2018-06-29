/**
 * @file
 * Functions used to retrieve user input via the touch screen or keypad.
 */

#ifndef _SDK_OS_INPUT_H
#define _SDK_OS_INPUT_H
#include <stdint.h>

#define EVENT_KEY 0x0017
#define EVENT_ACTBAR_RESIZE 0x1003
#define EVENT_ACTBAR_SWAP 0x1004
#define EVENT_ACTBAR_ROTATE 0x1005
#define EVENT_ACTBAR_ESC 0x3009
#define EVENT_ACTBAR_SETTINGS 0x300A
#define EVENT_TOUCH 0x4000

#define KEY_PRESSED 1
#define KEY_HELD 0x100
#define KEY_RELEASED 0x40

#define KEYCODE_KEYBOARD 0x00B3
#define KEYCODE_SHIFT 0x00A0
#define KEYCODE_BACKSPACE 0x0097
#define KEYCODE_POWER_CLEAR 0x0080
#define KEYCODE_UP 0x0090
#define KEYCODE_DOWN 0x0091
#define KEYCODE_LEFT 0x0092
#define KEYCODE_RIGHT 0x0093
#define KEYCODE_EQUALS 0x003D
#define KEYCODE_X 0xEDB8
#define KEYCODE_Y 0xEDB9
#define KEYCODE_Z 0xEDBA
#define KEYCODE_POWER 0x005E
#define KEYCODE_DIVIDE 0x002F
#define KEYCODE_OPEN_PARENTHESIS 0x0028
#define KEYCODE_7 0x0037
#define KEYCODE_8 0x0038
#define KEYCODE_9 0x0039
#define KEYCODE_TIMES 0xEE10
#define KEYCODE_CLOSE_PARENTHESIS 0x0029
#define KEYCODE_4 0x0034
#define KEYCODE_5 0x0035
#define KEYCODE_6 0x0036
#define KEYCODE_MINUS 0x002D
#define KEYCODE_COMMA 0x002C
#define KEYCODE_1 0x0031
#define KEYCODE_2 0x0032
#define KEYCODE_3 0x0033
#define KEYCODE_PLUS 0x002B
#define KEYCODE_NEGATIVE 0x001F
#define KEYCODE_0 0x0030
#define KEYCODE_DOT 0x002E
#define KEYCODE_EXP 0x001D
#define KEYCODE_EXE 0x0094

#define TOUCH_DOWN 1
#define TOUCH_HOLD_DRAG 2
#define TOUCH_ACT_BAR 0x100
#define TOUCH_UP 0x40

/**
 * Information about an input event returned from @ref GetInput. See
 * documentation for individual members for more information.
 */
struct InputEvent {
	/**
	 * Code representing which event occurred. Events with a value for this
	 * field which do not correspond to a macro beginning with @c EVENT_ must
	 * be ignored.
	 */
	uint16_t type;
	uint16_t zero;

	/**
	 * The data associated with the event. Under no circumstances should the
	 * sub-structure for an event type other than the one communicated by 
	 * @c eventType be accessed.
	 *
	 * Corresponding members:
	 * - @c EVENT_KEY: @c key
	 * - @c EVENT_TOUCH: @c touch_single
	 *
	 * The events @c EVENT_ACTBAR_RESIZE, @c EVENT_ACTBAR_SWAP,
	 * @c EVENT_ACTBAR_ROTATE, @c EVENT_ACTBAR_ESC, and @c EVENT_ACTBAR_SETTINGS
	 * do not report any data.
	 */
	union {
		struct {
			/**
			 * The direction the key traveled. One of @c KEY_PRESSED,
			 * @c KEY_HELD or @c KEY_RELEASED.
			 */
			uint32_t direction;

			/**
			 * The key code for the key. See macros beginning with @c KEYCODE_.
			 */
			uint16_t keyCode;
		} key;

		struct {
			/**
			 * The direction of the touch. One of @c TOUCH_DOWN,
			 * @c TOUCH_HOLD_DRAG, @c TOUCH_ACT_BAR, or @c TOUCH_UP.
			 */
			uint32_t direction;

			/**
			 * The X position of the cursor, in screen pixels. May be negative
			 * or be greater than or equal to the width of the screen.
			 */
			int32_t p1_x;

			/**
			 * The Y position of the cursor, in screen pixels. May be negative
			 * or be greater than or equal to the height of the screen.
			 */
			int32_t p1_y;

			uint16_t adc_x1;
			uint16_t adc_y1;
			uint16_t adc_z1;
			uint16_t adc_z2;
			uint16_t adc_x2;
			uint16_t adc_y2;

			uint16_t adc_gh;
			uint16_t adc_dm;
		} touch_single;

		struct {
			uint32_t direction;

			int32_t p1_x;
			int32_t p1_y;
			int32_t p1_z;

			int32_t p2_x;
			int32_t p2_y;
			int32_t p2_z;

			uint32_t distance;

			uint16_t adc_x1;
			uint16_t adc_y1;
			uint16_t adc_z1;
			uint16_t adc_z2;
			uint16_t adc_x2;
			uint16_t adc_y2;

			uint16_t adc_gh;
			uint16_t adc_dm;
		} touch_multi;
	} data;
};

/**
 * Polls for any input events.
 *
 * The @p event structure should be zeroed before calling @ref GetInput.
 *
 * @param event A pointer to an @c struct @ref InputEvent to store the input
 * event's data in.
 * @param unknown1 An unknown value. The value @c 0xFFFFFFFF must be supplied.
 * @param unknown2 An unknown value. The value @c 0x10 must be supplied.
 * @return Always returns 0.
 */
extern "C"
int GetInput(struct InputEvent *event, uint32_t unknown1, uint32_t unknown2);

#endif
