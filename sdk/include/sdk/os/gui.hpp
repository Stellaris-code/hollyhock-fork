/**
 * @file
 * Functions and classes used to create/display GUI elements.
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
#include <stdint.h>

const int BUTTON_OK = 1 << 5;
const int BUTTON_YES = 1 << 6;
const int BUTTON_NO = 1 << 7;
const int BUTTON_ABORT = 1 << 8;
const int BUTTON_RETRY = 1 << 9;
const int BUTTON_CANCEL = 1 << 10;

extern "C"
struct GUI_Dialog *GUI_CreateDialog(
	void *dialog,
	int height, int alignment,
	const char *title,
	int unknown2, int unknown3,
	int keyboard
);

extern "C"
void *GUI_CreateLabel(
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

extern "C"
void *GUI_CreateRadioButton(
	void *radioButton,
	int x, int y,
	const char *text,
	int unknown0,
	int flags,
	void *font,
	int unknown2
);

extern "C"
void *GUI_CreateTextBox(
	void *textBox,
	int x, int y, int width,
	const char *text,
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
	const char *titleString,
	const char *contentPrefix, const char *contentString,
	int buttons, bool disableCloseButton
);

class Wrapped {
public:
	/**
	 * Returns a pointer to the internal class this class wraps.
	 * 
	 * @return The pointer to the internal class this class wraps.
	 */
	virtual void *GetWrapped() = 0;
};

class GUIElement : public Wrapped {};

class GUIDialog : public Wrapped {
public:
	enum Height : int {
		Height25 = 0,
		Height55 = 1,
		Height75 = 2,
		Height95 = 3,
		Height35 = 4,
		Height60 = 5
	};

	enum Alignment : int {
		AlignTop = 0,
		AlignCenter = 1,
		AlignBottom = 2
	};

	enum KeyboardState : int {
		KeyboardStateNone = 0, // 2 gives same effect
		KeyboardStateMath1 = 1, // 3 gives same effect
		KeyboardStateMath2 = 4,
		KeyboardStateMath3 = 5,
		KeyboardStateTrig = 6,
		KeyboardStateVar = 7,
		KeyboardStateABC = 8,
		KeyboardStateCatalog = 9,
		KeyboardStateAdvance = 10,
		KeyboardStateNumber = 11
	};

	/**
	 * Creates a dialog.
	 * 
	 * @param height The height of the dialog.
	 * @param alignment The screen position to align the dialog with.
	 * @param title The string to display in the title bar of the dialog.
	 * @param keyboard The keyboard to display when the dialog is shown.
	 */
	GUIDialog(
		enum Height height, enum Alignment alignment,
		const char* title,
		enum KeyboardState keyboard
	) {
		m_wrapped = reinterpret_cast<decltype(m_wrapped)>(GUI_CreateDialog(
			0,
			height, alignment,
			title,
			0, 0,
			keyboard
		));
	}

	/**
	 * Returns the X position of the left edge of the dialog body, in pixels.
	 *
	 * @return The X position of the left edge of the dialog body, in pixels.
	 */
	uint16_t GetLeftX() {
		return m_wrapped->leftX;
	}

	/**
	 * Returns the Y position of the top edge of the dialog body, in pixels.
	 *
	 * @return The Y position of the top edge of the dialog body, in pixels.
	 */
	uint16_t GetTopY() {
		return m_wrapped->topY;
	}

	/**
	 * Returns the X position of the right edge of the dialog body, in pixels.
	 *
	 * @return The X position of the right edge of the dialog body, in pixels.
	 */
	uint16_t GetRightX() {
		return m_wrapped->rightX;
	}

	/**
	 * Returns the Y position of the bottom edge of the dialog body, in pixels.
	 *
	 * @return The Y position of the bottom edge of the dialog body, in pixels.
	 */
	uint16_t GetBottomY() {
		return m_wrapped->bottomY;
	}

	/**
	 * Adds a GUI element to the dialog.
	 * 
	 * @param element The GUI element to add.
	 */
	void AddElement(GUIElement &element) {
		m_wrapped->vtable->AddElement(m_wrapped, element.GetWrapped(), 0);
	}

	/**
	 * Presents the dialog to the user. Blocks until the dialog is closed.
	 */
	void ShowDialog() {
		m_wrapped->vtable->ShowDialog(m_wrapped);
	}

	void *GetWrapped() {
		return m_wrapped;
	}

private:
	struct {
		uint8_t unknown0[0x10];

		// refer to accessors for documentation
		uint16_t leftX;
		uint16_t topY;
		uint16_t rightX;
		uint16_t bottomY;

		uint8_t unknown1[0x34];

		struct {
			uint8_t unknown0[0x38];

			// unknown0 - always pass 0
			void (*const AddElement)(void *dialog, void *element, int unknown0);

			uint8_t unknown1[0x158];
			void (*const ShowDialog)(void *dialog);

			// TODO: work out the true length of the vtable
			uint8_t unknown2[0x200];
		} *vtable;
	} *m_wrapped;
};

class GUILabel : public GUIElement {
public:
	enum Flag : int {
		/** Enables displaying the background color of the label. */
		FlagBackground = 1 << 0,

		/**
		 * Allows the label to be selected/brought into focus.
		 * 
		 * When the label is selected, the text and background colors switch.
		 */
		FlagSelectable = 1 << 15
	};

	/**
	 * Creates a label.
	 * 
	 * @param x The X position to place the label at, in pixels and relative to
	 * the top left of the display.
	 * @param y The Y position to place the label at, in pixels and relative to
	 * the top left of the display.
	 * @param text The text to display as the label.
	 */
	GUILabel(int x, int y, const char *text) : GUILabel(
		x, y, text, 0, 0, 0, false, 0
	) {

	}

	/**
	 * Creates a label.
	 * 
	 * @param x The X position to place the label at, in pixels and relative to
	 * the top left of the display.
	 * @param y The Y position to place the label at, in pixels and relative to
	 * the top left of the display.
	 * @param text The text to display as the label.
	 * @param flags A bitfield of flags specified by bitwise-ORing members of
	 * the @ref Flag enum.
	 * @param textColor A pointer to an RGB565 color to use for the text, or 0
	 * for the default color.
	 * @param backgroundColor A pointer to an RGB565 color to use for the
	 * background, or 0 for the default color.
	 */
	GUILabel(
		int x, int y,
		const char *text,
		int flags,
		uint16_t *textColor, uint16_t *backgroundColor
	) : GUILabel(x, y, text, flags, textColor, backgroundColor, false, 0) {

	}

	/**
	 * Creates a label.
	 * 
	 * @param x The X position to place the label at, in pixels and relative to
	 * the top left of the display.
	 * @param y The Y position to place the label at, in pixels and relative to
	 * the top left of the display.
	 * @param text The text to display as the label.
	 * @param flags A bitfield of flags specified by bitwise-ORing members of
	 * the @ref Flag enum.
	 * @param textColor A pointer to an RGB565 color to use for the text, or 0
	 * for the default color.
	 * @param backgroundColor A pointer to an RGB565 color to use for the
	 * background, or 0 for the default color.
	 * @param showShadow True if the text shadow should be shown.
	 * @param shadowColor An RGB565 color to use for the text shadow.
	 */
	GUILabel(
		int x, int y,
		const char *text,
		int flags,
		uint16_t *textColor, uint16_t *backgroundColor,
		bool showShadow, uint16_t shadowColor
	) {
		m_wrapped = reinterpret_cast<decltype(m_wrapped)>(GUI_CreateLabel(
			0,
			x, y, text,
			0,
			flags,
			0, 
			textColor, backgroundColor, showShadow, shadowColor,
			0
		));
	}

	void *GetWrapped() {
		return m_wrapped;
	}

private:
	void *m_wrapped;
};

class GUIRadioButton : public GUIElement {
public:
	enum Flag : int {
		/** Causes the radio button to be selected by default. */
		FlagSelected = 1 << 2,

		/** Makes the radio button interactive. */
		FlagEnabled = 1 << 15
	};

	/**
	 * Creates a radio button.
	 * 
	 * @param x The X position to place the radio button at, in pixels and
	 * relative to the top left of the display.
	 * @param y The Y position to place the radio button at, in pixels and
	 * relative to the top left of the display.
	 * @param text The text to display to the right-hand side of the radio
	 * button.
	 * @param flags A bitfield of flags specified by bitwise-ORing members of
	 * the @ref Flag enum.
	 */
	GUIRadioButton(
		int x, int y,
		const char *text,
		int flags
	) {
		m_wrapped = reinterpret_cast<decltype(m_wrapped)>(GUI_CreateRadioButton(
			0,
			x, y,
			text,
			0,
			flags,
			0, 0
		));
	}

	void *GetWrapped() {
		return m_wrapped;
	}

private:
	void *m_wrapped;
};

class GUITextBox : public GUIElement {
public:
	enum Flag {
		/** Enables drawing the text box's outline and background. */
		FlagDrawBox = 1 << 3,

		/** Allows the contents of the text box to be modified. */
		FlagEditable = 1 << 8
	};

	/**
	 * Creates a text box.
	 *
	 * With an example value for the parameter @p maxLength of 4, the behaviour
	 * of @p countLengthByBytes is as follows:
	 * - If true, 4 bytes worth of text can be written in the text box. This may
	 * be two 2-byte characters, one 2-byte character and two 1-byte characters,
	 * etc.
	 * - If false, 4 characters worth of text can be written in the text box.
	 * This may be four 2-byte characters, one 2-byte character and three 1-byte
	 * characters, etc.
	 * 
	 * @param x The X position to place the text box at, in pixels and relative
	 * to the top left of the display.
	 * @param y The Y position to place the text box at, in pixels and relative
	 * to the top left of the display.
	 * @param width The width of the textbox, in pixels.
	 * @param text A string to pre-populate the text box with, or 0 if the
	 * textbox should be initially empty.
	 * @param flags A bitfield of flags specified by bitwise-ORing members of
	 * the @ref Flag enum.
	 * @param maxLength The maximum number of characters/bytes the text box
	 * should hold.
	 * @param countLengthByBytes True if @p maxLength specifies the maximum
	 * number of bytes, and false if it specifies the maximum number of
	 * characters.
	 */
	GUITextBox(
		int x, int y, int width,
		const char *text,
		int flags, int maxLength, bool countLengthByBytes
	) {
		m_wrapped = reinterpret_cast<decltype(m_wrapped)>(GUI_CreateTextBox(
			0,
			x, y, width,
			text,
			0,
			flags, maxLength, countLengthByBytes
		));
	}

	void *GetWrapped() {
		return m_wrapped;
	}

private:
	void *m_wrapped;
};
