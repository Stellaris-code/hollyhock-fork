.section .init

sts.l pr, @-r15

! Can't use the bsr instruction, because we get errors about the displacement not fitting in the 12-bit field.
! Gotta use jsr instead
mov.l main_bootstrap_addr, r0
jsr @r0
nop

lds.l @r15+, pr
rts
nop

.align 4
main_bootstrap_addr:
	.long _main_bootstrap
