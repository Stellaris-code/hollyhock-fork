/**
 * @file
 * @brief Functions and classes to create/display GUI elements.
 *
 * Example: display a simple dialog with a label
 * @code{cpp}
 * GUIDialog dialog(
 *     GUIDialog::Height25, GUIDialog::AlignTop,
 *     "Dialog Title",
 *     GUIDialog::KeyboardStateABC
 * );
 * 
 * GUILabel label(
 *     dialog.GetLeftX() + 10,
 *     dialog.GetTopY() + 10,
 *     "Label Text"
 * );
 * dialog.AddElement(label);
 * 
 * dialog.ShowDialog();
 * @endcode
 */

#pragma once
#include "gui/button.hpp"
#include "gui/dialog.hpp"
#include "gui/dropDown.hpp"
#include "gui/label.hpp"
#include "gui/longLabel.hpp"
#include "gui/radioButton.hpp"
#include "gui/textBox.hpp"

const int BUTTON_OK = 1 << 5;
const int BUTTON_YES = 1 << 6;
const int BUTTON_NO = 1 << 7;
const int BUTTON_ABORT = 1 << 8;
const int BUTTON_RETRY = 1 << 9;
const int BUTTON_CANCEL = 1 << 10;

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
 * @param[in] titleString A string to use for the message box's title.
 * @param[in] contentPrefix A string to prefix the content with.
 * @param[in] contentString A string to use for the message box's content.
 * @param buttons A bitfield specifying which buttons to show.
 * @param disableCloseButton Set to @c true to disable the close button.
 * @return An unknown GUI struct.
 */
extern "C"
void *GUI_DisplayMessageBox_Internal(
	int unknown,
	const char *titleString,
	const char *contentPrefix, const char *contentString,
	int buttons, bool disableCloseButton
);
