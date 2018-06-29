.macro DEFINE_OS_FUNC name, addr
.globl _\name
.set _\name, \addr
.endm
