#include <sdk/os/gui.hpp>

void main() {
	GUIDialog dialog(
		GUIDialog::Height25, GUIDialog::AlignTop,
		"Dialog title",
		GUIDialog::KeyboardStateABC
	);

	GUILabel label0(
		dialog.GetLeftX() + 10,
		dialog.GetTopY() + 10,
		"Label 0"
	);
	dialog.AddElement(label0);

	GUILabel label1(
		dialog.GetLeftX() + 10,
		dialog.GetTopY() + 50,
		"Label 1"
	);
	dialog.AddElement(label1);

	GUITextBox textBox(
		dialog.GetLeftX() + 150,
		dialog.GetTopY() + 10,
		100,
		"Textbox!",
		GUITextBox::FlagEditable | GUITextBox::FlagDrawBox,
		100000, false
	);
	dialog.AddElement(textBox);

	GUIRadioButton radioButton0(
		dialog.GetLeftX() + 150,
		dialog.GetTopY() + 40,
		"Radio button 0",
		GUIRadioButton::FlagEnabled
	);
	dialog.AddElement(radioButton0);

	GUIRadioButton radioButton1(
		dialog.GetLeftX() + 150,
		dialog.GetTopY() + 60,
		"Radio button 1",
		GUIRadioButton::FlagEnabled | GUIRadioButton::FlagSelected
	);
	dialog.AddElement(radioButton1);

	dialog.ShowDialog();
}
