! r9 - function call
! r12 - file descriptor
! all delay slots are nops cause i can't be bothered

! Open the file and get a file descriptor
mova file_path, r0
mov r0, r4
mov #5, r5

mov.l open, r9
jsr @r9
nop

! Save our file descriptor so it's not overwritten
mov r0, r12

! Print the error if there was one
mov r0, r4
mova open_str, r0
mov r0, r5

mov.l PrintStandardErrorString, r9
jsr @r9
nop

! Run fstat to get the file size
mov r12, r4
mov.l buf, r5

mov.l fstat, r9
jsr @r9
nop

! Print the error if there was one
mov r0, r4
mova fstat_str, r0
mov r0, r5

mov.l PrintStandardErrorString, r9
jsr @r9
nop

! Read the file into our buffer
mov r12, r4
mov.l buf, r5
mov.l @(4,r5), r6 ! 4 bytes into the fstat struct is the file size in bytes

mov.l read, r9
jsr @r9
nop

! Print the error if there was one
mov r0, r4
mova read_str, r0
mov r0, r5

mov.l PrintStandardErrorString, r9
jsr @r9
nop

! Close the file
mov r12, r4

mov.l close, r9
jsr @r9
nop

! Print the error if there was one
mov r0, r4
mova close_str, r0
mov r0, r5

mov.l PrintStandardErrorString, r9
jsr @r9
nop

! Jump to the code we just loaded, as a subroutine/function
mov.l buf, r9
jsr @r9
nop

! Hang, because returning to the function is too hard
hang:
	bra hang
	nop

.align 4
open:
	.long 0x80057854
fstat:
	.long 0x8005798E
read:
	.long 0x800578A2
close:
	.long 0x80057912
PrintStandardErrorString:
	.long 0x80065998

buf:
	.long 0x8CFE0000

num_bytes:
	.long 0x000000FF

.align 4
open_str:
	.string "open"
.align 4
fstat_str:
	.string "fstat"
.align 4
read_str:
	.string "read"
.align 4
close_str:
	.string "close"

.align 4	
file_path:
	.string "\\fls0\\run.bin"
