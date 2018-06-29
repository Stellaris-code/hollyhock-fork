/**
 * @file
 * Functions for creating GUI elements, such as dialogs and message boxes.
 *
 * Example: display a simple dialog with a label
 * @code{c}
 * struct GUI_Dialog *dialog = GUI_CreateDialog(
 *     0,
 *     DIALOG_HEIGHT_55, DIALOG_ALIGN_CENTER,
 *     "Dialog title",
 *     0, 0,
 *     DIALOG_KBD_NONE
 * );
 *
 * void *label = GUI_CreateLabel(
 *     0,
 *     dialog->leftX + 5, dialog->topY + 5,
 *     "Label Text",
 *     0, 0, 0, 0, 0, false, 0, 0
 * );
 * dialog->functions->AddElement(dialog, label, 0);
 *
 * dialog->functions->ShowDialog(dialog);
 * @endcode
 */

#ifndef _SDK_OS_GUI_H
#define _SDK_OS_GUI_H
#include <stdbool.h>
#include <stdint.h>

#define BUTTON_OK (1 << 5)
#define BUTTON_YES (1 << 6)
#define BUTTON_NO (1 << 7)
#define BUTTON_ABORT (1 << 8)
#define BUTTON_RETRY (1 << 9)
#define BUTTON_CANCEL (1 << 10)

#define DIALOG_HEIGHT_25 0
#define DIALOG_HEIGHT_55 1
#define DIALOG_HEIGHT_75 2
#define DIALOG_HEIGHT_95 3
#define DIALOG_HEIGHT_35 4
#define DIALOG_HEIGHT_60 5

#define DIALOG_ALIGN_TOP 0
#define DIALOG_ALIGN_CENTER 1
#define DIALOG_ALIGN_BOTTOM 2

#define DIALOG_KBD_NONE 0 // 2 gives same effect
#define DIALOG_KBD_MATH1 1 // 3 gives same effect
#define DIALOG_KBD_MATH2 4
#define DIALOG_KBD_MATH3 5
#define DIALOG_KBD_TRIG 6
#define DIALOG_KBD_VAR 7
#define DIALOG_KBD_ABC 8
#define DIALOG_KBD_CATALOG 9
#define DIALOG_KBD_ADVANCE 10
#define DIALOG_KBD_NUMBER 11

#define LABEL_BACKGROUND (1 << 0)
#define LABEL_SELECTABLE (1 << 15)

#define RADIOBUTTON_SELECTED (1 << 2)
#define RADIOBUTTON_ENABLED (1 << 15)

#define TEXTBOX_DRAW_BOX (1 << 3)
#define TEXTBOX_EDITABLE (1 << 8)

// Forward declaration - the dialog struct is passed as an argument to some
// functions in the function table.
struct GUI_Dialog;

struct GUI_Dialog_FunctionTable {
	uint8_t unknown0[0x38];

	/**
	 * Adds a GUI element to the dialog.
	 *
	 * @param dialog A pointer to the dialog to add the GUI element to.
	 * @param element The GUI element to add.
	 * @param unknown0 Unknown parameter. Value 0 should be passed.
	 */
	void (*const AddElement)(struct GUI_Dialog *dialog, void *element, int unknown0);

	uint8_t unknown2[0x158];

	/**
	 * Presents the dialog to the user.
	 *
	 * @param dialog A pointer to the dialog to display.
	 */
	void (*const ShowDialog)(struct GUI_Dialog *dialog);
};

struct GUI_Dialog {
	uint8_t unknown0[0x10];

	/**
	 * The X position of the left edge of the dialog body, in pixels.
	 */
	uint16_t leftX;

	/**
	 * The Y position of the top edge of the dialog body, in pixels.
	 */
	uint16_t topY;

	/**
	 * The X position of the right edge of the dialog body, in pixels.
	 */
	uint16_t rightX;

	/**
	 * The Y position of the bottom edge of the dialog body, in pixels.
	 */
	uint16_t bottomY;

	uint8_t unknown1[0x34];

	/**
	 * Table of functions performing a wide range of operations on the dialog.
	 * For information about each function, see the documentation for
	 * @c struct @ref GUI_Dialog_FunctionTable.
	 */
	struct GUI_Dialog_FunctionTable *functions;
};

/**
 * Creates a dialog.
 *
 * If 0 is passed for the argument @p dialog, a dialog struct will be allocated
 * inside the function.
 *
 * @param dialog A pointer to a dialog struct to populate, or 0.
 * @param height The height of the dialog. Specify with macros beginning with
 * @c DIALOG_HEIGHT_.
 * @param alignment The screen position to align the dialog with. One of
 * @c DIALOG_ALIGN_TOP, @c DIALOG_ALIGN_CENTER or @c DIALOG_ALIGN_BOTTOM.
 * @param title The string to display in the title bar of the dialog.
 * @param unknown2 Unknown parameter. Value 0 should be passed.
 * @param unknown3 Unknown parameter. Value 0 should be passed.
 * @param keyboard The keyboard to display when the dialog is shown. Specify
 * with macros beginning with @c DIALOG_KBD_.
 * @return A pointer to the dialog struct created.
 */
extern "C"
struct GUI_Dialog *GUI_CreateDialog(
	void *dialog,
	int height, int alignment,
	char *title,
	int unknown2, int unknown3,
	int keyboard
);

/**
 * Creates a label.
 *
 * If 0 is passed for the argument @p label, a label struct will be allocated
 * inside the function.
 *
 * If the flag @c LABEL_SELECTABLE is specified, when the label is in focus the
 * text and background color switch. Specifying @c LABEL_BACKGROUND enables
 * displaying the background color of the label.
 *
 * @param label A pointer to a label struct to populate, or 0.
 * @param x The X position to place the label at, in pixels and relative to the
 * top left of the display.
 * @param y The Y position to place the label at, in pixels and relative to the
 * top left of the display.
 * @param text The text to display as the label.
 * @param unknown0 Unknown parameter. Value 0 should be passed.
 * @param flags A bitfield of flags, customizing how the label should behave.
 * @param font A pointer to the font to use for the label, or 0 for the default
 * font.
 * @param textColor A pointer to an RGB565 color to use for the text, or 0 for
 * the default color.
 * @param backgroundColor A pointer to an RGB565 color to use for the
 * background, or 0 for the default color.
 * @param showShadow True if the text shadow should be shown.
 * @param shadowColor An RGB565 color to use for the text shadow.
 * @param unknown1 Unknown parameter. Value 0 should be passed.
 * @return A pointer to the label struct created.
 */
extern "C"
void *GUI_CreateLabel(
	void *label,
	int x, int y,
	char *text,
	int unknown0,
	int flags,
	void *font,
	uint16_t *textColor, uint16_t *backgroundColor,
	bool showShadow, uint16_t shadowColor,
	int unknown1
);

/**
 * Creates a radio button.
 *
 * If 0 is passed for the argument @p radioButton, a radio button struct will be
 * allocated inside the function.
 *
 * The flag @c RADIOBUTTON_ENABLED must be set in the parameter @p flags for the
 * radio button to be interactive. A radio button can be set as selected by
 * default with the @c RADIOBUTTON_SELECTED flag.
 *
 * @param radioButton A pointer to a radio button struct to populate, or 0.
 * @param x The X position to place the radio button at, in pixels and relative
 * to the top left of the display.
 * @param y The Y position to place the radio button at, in pixels and relative
 * to the top left of the display.
 * @param text The text to display to the right-hand side of the radio button.
 * @param unknown0 Unknown parameter. Value 0 should be passed.
 * @param flags A bitfield of flags, customizing how the radio button should
 * behave.
 * @param font A pointer to the font to use for the radio button's label, or 0
 * for the default font.
 * @param unknown2 Unknown parameter. Value 0 should be passed.
 * @return A pointer to the radio button struct created.
 */
extern "C"
void *GUI_CreateRadioButton(
	void *radioButton,
	int x, int y,
	char *text,
	int unknown0,
	int flags,
	void *font,
	int unknown2
);

/**
 * Creates a text box.
 *
 * If 0 is passed for the argument @p textBox, a text box struct will be
 * allocated inside the function.
 *
 * The flag @c TEXTBOX_EDITABLE must be set in @p flags if the content of the
 * text box should be editable by the user. If it is not set, the content of the
 * text box is read-only. The flag @c TEXTBOX_DRAW_BOX must be set for the text
 * box to render correctly.
 *
 * With an example value for the parameter @p maxLength of 4, the behaviour of
 * @p countLengthByBytes is as follows:
 * - If true, 4 bytes worth of text can be written in the text box. This may be
 * two 2-byte characters, one 2-byte character and two 1-byte characters, etc.
 * - If false, 4 characters worth of text can be written in the text box. This
 * may be four 2-byte characters, one 2-byte character and three 1-byte
 * characters, etc.
 *
 * @param textBox A pointer to a text box struct to populate, or 0.
 * @param x The X position to place the text box at, in pixels and relative to
 * the top left of the display.
 * @param y The Y position to place the text box at, in pixels and relative to
 * the top left of the display.
 * @param width The width of the text box, in pixels.
 * @param text A string to pre-populate the text box with, or 0 if the text box
 * should be initially empty.
 * @param unknown0 Unknown parameter. Value 0 should be passed.
 * @param flags A bitfield of flags, customizing how the text box should behave.
 * @param maxLength The maximum number of characters/bytes the text box should
 * hold.
 * @param countLengthByBytes True if @p maxLength specifies the maximum number
 * of bytes, and false if it specifies the maximum number of characters.
 * @return A pointer to the text box struct created.
 */
extern "C"
void *GUI_CreateTextBox(
	void *textBox,
	int x, int y, int width,
	char *text,
	int unknown0,
	int flags,
	int maxLength, bool countLengthByBytes
);

/**
 * Displays a message box with the specified title and content, retrieved
 * through their ID in the string table.
 *
 * @param unknown An unknown parameter.
 * @param titleStringID The ID of the string to use for the message box's title.
 * @param contentStringID The ID of the string to use for the message box's
 * content.
 */
extern "C"
void GUI_DisplayMessageBox(int unknown, int titleStringID, int contentStringID);

/**
 * Displays a message box with the specified title and content. A prefix to the
 * content may be specified, which is displayed before the main content.
 * 
 * The buttons to be displayed can be set using the @p buttons bitfield.
 * Possible values are defined by macros beginning with @c BUTTON_, which can be
 * bitwise OR'd together to display multiple buttons. A maximum of 3 buttons can
 * be specified.
 * 
 * The true usage of the 4th bit of the @p buttons bitfield is unknown, however
 * it has been observed to shrink the black box which forms the title bar of the
 * message box.
 * 
 * Note: if no buttons are specified, and the close button is disabled, it is
 * impossible to exit the message box.
 * 
 * @param unknown An unknown parameter.
 * @param titleString A string to use for the message box's title.
 * @param contentPrefix A string to prefix the content with.
 * @param contentString A string to use for the message box's content.
 * @param buttons A bitfield specifying which buttons to show.
 * @param disableCloseButton Set to @c true to disable the close button.
 * @return An unknown GUI struct.
 */
extern "C"
void *GUI_DisplayMessageBox_Internal(
	int unknown,
	char *titleString,
	char *contentPrefix, char *contentString,
	int buttons, bool disableCloseButton
);

#endif
