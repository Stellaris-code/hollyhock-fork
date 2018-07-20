#include <sdk/os/gui.hpp>
#include <sdk/os/mem.hpp>

/**
 * Creates a dialog.
 * 
 * @param height The height of the dialog.
 * @param alignment The screen position to align the dialog with.
 * @param[in] title The string to display in the title bar of the dialog.
 * @param keyboard The keyboard to display when the dialog is shown.
 */
GUIDialog::GUIDialog(
	enum Height height, enum Alignment alignment,
	const char* title,
	enum KeyboardState keyboard
) : m_vtable({}) {
	m_wrapped = GUI_CreateDialog(
		0,
		height, alignment,
		title,
		0, 0,
		keyboard
	);

	// Save the old vtable
	m_oldVTable = GetWrapped<GUIDialog_Wrapped>()->vtable;

	// Copy the pre-existing vtable
	memcpy(&m_vtable, GetWrapped<GUIDialog_Wrapped>()->vtable, sizeof(m_vtable));

	m_vtable.me = this;
	m_vtable.OnEvent = OnEvent_Wrap;

	GetWrapped<GUIDialog_Wrapped>()->vtable = &m_vtable;
}

int GUIDialog::OnEvent_Wrap(struct GUIDialog_Wrapped *dialog, struct GUIDialog_OnEvent_Data *event) {
	// Call the OnEvent function of our class (not of CASIO's one!)
	return dialog->vtable->me->OnEvent(dialog, event);
}

int GUIDialog::OnEvent(struct GUIDialog_Wrapped *dialog, struct GUIDialog_OnEvent_Data *event) {
	// We've overidden the entry for OnEvent in our vtable with one that
	// redirects to this method. Since we've done that, if we want to call the
	// actual base OnEvent method, we use the old vtable we backed up in the
	// constructor.
	return VTABLE_CALL_EXT(
		dialog, m_oldVTable,
		OnEvent,
		event
	);
}

/**
 * Returns the X position of the left edge of the dialog body, in pixels.
 *
 * @return The X position of the left edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetLeftX() {
	return GetWrapped<GUIDialog_Wrapped>()->leftX;
}

/**
 * Returns the Y position of the top edge of the dialog body, in pixels.
 *
 * @return The Y position of the top edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetTopY() {
	return GetWrapped<GUIDialog_Wrapped>()->topY;
}

/**
 * Returns the X position of the right edge of the dialog body, in pixels.
 *
 * @return The X position of the right edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetRightX() {
	return GetWrapped<GUIDialog_Wrapped>()->rightX;
}

/**
 * Returns the Y position of the bottom edge of the dialog body, in pixels.
 *
 * @return The Y position of the bottom edge of the dialog body, in pixels.
 */
uint16_t GUIDialog::GetBottomY() {
	return GetWrapped<GUIDialog_Wrapped>()->bottomY;
}

/**
 * Adds a GUI element to the dialog.
 * 
 * @param element The GUI element to add.
 */
void GUIDialog::AddElement(GUIElement &element) {
	VTABLE_CALL(
		GetWrapped<GUIDialog_Wrapped>(), vtable,
		AddElement,
		element.GetWrapped<void>(), 1
	);
}

/**
 * Refreshes the dialog, redrawing all components.
 */
void GUIDialog::Refresh() {
	VTABLE_CALL(
		GetWrapped<GUIDialog_Wrapped>(), vtable,
		Refresh
	);
}

/**
 * Presents the dialog to the user. Blocks until the dialog is closed.
 */
int GUIDialog::ShowDialog() {
	return VTABLE_CALL(
		GetWrapped<GUIDialog_Wrapped>(), vtable,
		ShowDialog
	);
}
