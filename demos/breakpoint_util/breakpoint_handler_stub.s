.global _BreakpointHandlerStub
.type _BreakpointHandlerStub, @function
_BreakpointHandlerStub:
	/*
		I'm not sure why we have to push r0 to r7. When we switch into the
		exception handler, we should get our own bank so our changes to r0 to r7
		shouldn't have any effect. They seem to though, and I don't know why.
	*/
	sts.l pr, @-r15
	mov.l r0, @-r15
	mov.l r1, @-r15
	mov.l r2, @-r15
	mov.l r3, @-r15
	mov.l r4, @-r15
	mov.l r5, @-r15
	mov.l r6, @-r15
	mov.l r7, @-r15
	mov.l r8, @-r15
	mov.l r9, @-r15
	mov.l r10, @-r15
	mov.l r11, @-r15
	mov.l r12, @-r15
	mov.l r13, @-r15
	mov.l r14, @-r15

	mov.l cpuState, r4

	/* 
		Since we've moved to privileged mode, the registers r0 to r7 from where
		the breakpoint was triggered are now stored in r0_bank to r7_bank
	*/
	stc r0_bank, r5
	mov.l r5, @(4*0, r4)
	stc r1_bank, r5
	mov.l r5, @(4*1, r4)
	stc r2_bank, r5
	mov.l r5, @(4*2, r4)
	stc r3_bank, r5
	mov.l r5, @(4*3, r4)
	stc r4_bank, r5
	mov.l r5, @(4*4, r4)
	stc r5_bank, r5
	mov.l r5, @(4*5, r4)
	stc r6_bank, r5
	mov.l r5, @(4*6, r4)
	stc r7_bank, r5
	mov.l r5, @(4*7, r4)

	/*
		Since we haven't modified r8 to r14, so they've retained the values they
		had when the breakpoint was triggered.
	*/
	mov.l r8, @(4*8, r4)
	mov.l r9, @(4*9, r4)
	mov.l r10, @(4*10, r4)
	mov.l r11, @(4*11, r4)
	mov.l r12, @(4*12, r4)
	mov.l r13, @(4*13, r4)
	mov.l r14, @(4*14, r4)

	/* Fix r15 so it points to the right place before we pushed stuff */
	mov r15, r6
	mov #0x40, r7
	sub r7, r6
	mov.l r6, @(4*15, r4)

	/*
		We're out of range for doing @(4*?, r4) so use r7 as the pointer into
		the CPUState struct
	*/
	mov r4, r7
	add #(4*16), r7

	stc gbr, r6
	mov.l r6, @r7

	sts pr, r6
	add #4, r7
	mov.l r6, @r7

	sts mach, r6
	add #4, r7
	mov.l r6, @r7

	sts macl, r6
	add #4, r7
	mov.l r6, @r7

	mov.l BreakpointHandler_Addr, r0
	jsr @r0
	nop

	mov.l @r15+, r14
	mov.l @r15+, r13
	mov.l @r15+, r12
	mov.l @r15+, r11
	mov.l @r15+, r10
	mov.l @r15+, r9
	mov.l @r15+, r8
	mov.l @r15+, r7
	mov.l @r15+, r6
	mov.l @r15+, r5
	mov.l @r15+, r4
	mov.l @r15+, r3
	mov.l @r15+, r2
	mov.l @r15+, r1
	mov.l @r15+, r0
	lds.l @r15+, pr

	stc sgr, r15
	rte
	nop

.align 4
BreakpointHandler_Addr:
	.long _BreakpointHandler
cpuState:
	/* 20 x 32-bit (4-byte) registers */
	.space 20 * 4
