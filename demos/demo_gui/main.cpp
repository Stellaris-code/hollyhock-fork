#include <appdef.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/gui.hpp>
#include <sdk/os/lcd.hpp>

APP_NAME("Demo GUI")

class MyDialog : public GUIDialog {
public:
	MyDialog() : GUIDialog(
		GUIDialog::Height55, GUIDialog::AlignTop,
		"My Demo Dialog",
		GUIDialog::KeyboardStateABC
	), m_label(
		// The position of all GUI elements should be specified in relation to
		// the bounding box of the dialog. This means that if the height or
		// alignment of the dialog is changed, the GUI elements will move
		// accordingly.
		GetLeftX() + 10, GetTopY() + 10,
		"Label 0 Text"
	), m_buttonOK(
		// Some controls take a bounding box, rather than an XY position.
		GetLeftX() + 10, GetTopY() + 40,
		// With these controls, it's a lot clearer to specify the right/bottom
		// coords by repeating the left/top and adding the width/height.
		// Here, a width of 100 and height of 35 is used.
		GetLeftX() + 10 + 120, GetTopY() + 40 + 35,
		"Close (OK)",
		BUTTON_OK_EVENT_ID
	), m_buttonCancel(
		// This button is specified from the right edge of the dialog, rather
		// than the left edge like the OK button.
		GetRightX() - 10 - 120, GetTopY() + 40,
		GetRightX() - 10, GetTopY() + 40 + 35,
		"Close (Cancel)",
		BUTTON_CANCEL_EVENT_ID
	), m_buttonTest(
		GetLeftX() + 10, GetTopY() + 85,
		GetRightX() - 10, GetTopY() + 85 + 35,
		"Test",
		BUTTON_TEST_EVENT_ID
	), m_textBox(
		GetLeftX() + 10, GetTopY() + 130, GetRightX() - GetLeftX() - 10,
		"Initial text box text",
		// Specify the maximum length of the text box's content, and if that
		// length should be counted by bytes or by characters (there's no
		// distinction if using normal characters, but 1 wide character is 2
		// bytes).
		100, false
	), m_longLabel(
		GetLeftX() + 10, GetTopY() + 160, GetRightX() - 10, GetBottomY() - 10,
		"Long labels like these allow text wrapping, and support newlines. They're great for lots of text!\nHere's some text, after a newline."
	) {
		// Every element of the dialog must be added by calling AddElement.
		// Calling the element's constructor isn't enough!
		AddElement(m_label);
		AddElement(m_buttonOK);
		AddElement(m_buttonCancel);
		AddElement(m_buttonTest);
		AddElement(m_textBox);
		AddElement(m_longLabel);
	}

	// This is the method that's called when an event occurs.
	// The most important information about the event is contained within the
	// 'event' parameter.
	virtual int OnEvent(struct GUIDialog_Wrapped *dialog, struct GUIDialog_OnEvent_Data *event) {
		// Check the ID of the event, to see if it matches an event assigned to
		// one of our controls.
		if (event->GetEventID() == BUTTON_TEST_EVENT_ID) {
			m_textBox.SetText("Test button pressed!");

			// Refresh the dialog, to ensure the text box's updated content is
			// drawn.
			Refresh();

			// Since we processed the event, return 0.
			return 0;
		}

		// If we don't process the event, let the base class process it.
		return GUIDialog::OnEvent(dialog, event);
	}

private:
	// Each of the elements in our GUI is a property of our dialog's class.
	// They're instantiated in the constructor.
	GUILabel m_label;

	// To make a button close the dialog, we can use a DialogResult for the
	// event ID. When one of these buttons causes the dialog to close, the
	// DialogResult is the value returned from ShowDialog.
	const uint16_t BUTTON_OK_EVENT_ID = DialogResultOK;
	GUIButton m_buttonOK;

	const uint16_t BUTTON_CANCEL_EVENT_ID = DialogResultCancel;
	GUIButton m_buttonCancel;

	// When not using a DialogResult for an event ID, start incrementing from 1.
	const uint16_t BUTTON_TEST_EVENT_ID = 1;
	GUIButton m_buttonTest;

	GUITextBox m_textBox;

	GUILongLabel m_longLabel;
};

void main() {
	// Create an instance of our dialog - but don't show it yet.
	MyDialog dialog;

	// Call ShowDialog on our dialog to display it to the user.
	// This call blocks until the user or system closes the dialog, which causes
	// the ShowDialog method to return a value specifying how the dialog was
	// closed.
	GUIDialog::DialogResult result = dialog.ShowDialog();

	Debug_Printf(
		0, 0, false, 0,
		"Dialog closed with result %X", result
	);
	LCD_Refresh();
}
