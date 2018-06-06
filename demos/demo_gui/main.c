#include <sdk/os/debug.h>
#include <sdk/os/file.h>
#include <sdk/os/gui.h>
#include <sdk/os/lcd.h>

void main() {
	// Put your code here.
	struct GUI_Dialog *dialog = GUI_CreateDialog(
		0,
		DIALOG_HEIGHT_25, DIALOG_ALIGN_CENTER,
		"Dialog title",
		0, 0,
		DIALOG_KBD_NONE
	);

	// Add some labels
	uint16_t r = 0b1111100000000000;
	uint16_t g = 0b0000011111100000;
	uint16_t b = 0b0000000000011111;

	void *label0 = GUI_CreateLabel(
		0,
		dialog->leftX + 10,
		dialog->topY + 10,
		"Label 0",
		0,
		LABEL_BACKGROUND | LABEL_SELECTABLE,
		0, 
		&r, &g, true, b,
		0
	);
	dialog->functions->AddElement(dialog, label0, 0);

	void *label1 = GUI_CreateLabel(
		0,
		dialog->leftX + 10,
		dialog->topY + 50,
		"Label 1",
		0,
		LABEL_BACKGROUND | LABEL_SELECTABLE,
		0, 
		&r, &g, true, b,
		0
	);
	dialog->functions->AddElement(dialog, label1, 0);

	// Add a textbox
	void *textBox = GUI_CreateTextBox(
		0,
		dialog->leftX + 150,
		dialog->topY + 10,
		100,
		"Textbox!",
		0,
		TEXTBOX_EDITABLE | TEXTBOX_DRAW_BOX,
		100000, false
	);
	dialog->functions->AddElement(dialog, textBox, 0);

	// Add some radio buttons
	void *radioButton0 = GUI_CreateRadioButton(
		0,
		dialog->leftX + 150,
		dialog->topY + 40,
		"Radio button 0",
		0,
		RADIOBUTTON_ENABLED | RADIOBUTTON_SELECTED,
		0, 0
	);
	dialog->functions->AddElement(dialog, radioButton0, 0);

	void *radioButton1 = GUI_CreateRadioButton(
		0,
		dialog->leftX + 150,
		dialog->topY + 60,
		"Radio button 1",
		0,
		RADIOBUTTON_ENABLED,
		0, 0
	);
	dialog->functions->AddElement(dialog, radioButton1, 0);

	dialog->functions->ShowDialog(dialog);
}
