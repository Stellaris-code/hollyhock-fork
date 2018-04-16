`? GUI_CreateDialog(0, 1, 0, char *title, 2, 4, 0)`
`? GUI_AddRadioButton(0, 0, 0, char *?, ?, ?, ?)`
`? GUI_AddNumericUpDown(0, stack ptr + 0x18, char *label, 0, ?, ?, ?, ?, ?, ?)`
`? GUI_AddButton(0, stack ptr + 0x8, char *text, uint32_t ?, ?, ?)`
`? GUI_AddLabel(0, some ptr, another ptr, char *text, ?, ?, ?, ?, ?, ?)`

# GUI Functions
## `void GUI_DisplayMessageBox(int unknown, int titleStringID, int contentStringID)`

Displays a message box with the specified title and content, retrieved through their ID in the string table.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x80146A38` |

## `void *GUI_DisplayMessageBox_Internal(int unknown, char *titleString, char *contentPrefix, char *contentString, int buttons, bool disableCloseButton)`

Displays a message box with the specified title and content. A prefix to the content may be specified, which is displayed before the main content.

The close button can be disabled by passing `true` through the `disableCloseButton` argument.

The buttons to be displayed can be set using the `buttons` bit field with the following values. A maximum of 3 buttons can be specified. The true usage of the 4th bit is unknown, however it has been observed to shrink the black box which forms the title bar of the message box. The `buttons` bit field has definitions as follows.

<table>
	<tr>
		<td><b>Bit</b></td>
		<td><b>10</b></td>
		<td><b>9</b></td>
		<td><b>8</b></td>
		<td><b>7</b></td>
		<td><b>6</b></td>
		<td><b>5</b></td>
		<td><b>4</b></td>
	</tr>
	<tr>
		<td><b>Value</b></td>
		<td>"Cancel" button</td>
		<td>"Retry" button</td>
		<td>"Abort" button</td>
		<td>"No" button</td>
		<td>"Yes" button</td>
		<td>"OK" button</td>
		<td>Shrink title block?</td>
	</tr>
</table>

If no buttons are specified and the close button is disabled, it is impossible to exit the message box.

Returns an unknown GUI struct.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x800B7914` |
