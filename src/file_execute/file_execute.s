.macro print_error error_string
	mov r0, r4
	mova \error_string, r0
	mov r0, r5

	mov.l PrintStandardErrorString, r0
	jsr @r0
	nop
.endm

! r8 - file descriptor
! all delay slots are nops cause i can't be bothered

mov.l r8, @-r15
sts.l pr, @-r15

! Open the file and get a file descriptor
mova file_path, r0
mov r0, r4
mov #5, r5

mov.l open, r0
jsr @r0
nop

! Save our file descriptor so it's not overwritten
mov r0, r8

print_error open_str

! Run fstat to get the file size
mov r8, r4
mov.l buf, r5

mov.l fstat, r0
jsr @r0
nop

print_error fstat_str

! Read the file into our buffer
mov r8, r4
mov.l buf, r5
mov.l @(4,r5), r6 ! 4 bytes into the fstat struct is the file size in bytes

mov.l read, r0
jsr @r0
nop

print_error read_str

! Close the file
mov r8, r4

mov.l close, r0
jsr @r0
nop

print_error close_str

! Jump to the code we just loaded, as a subroutine/function
mov.l buf, r0
jsr @r0
nop

lds.l @r15+, pr
rts
mov.l @r15+, r8

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
