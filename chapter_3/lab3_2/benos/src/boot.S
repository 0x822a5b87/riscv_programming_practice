	.section ".text.boot"

	.globl   _start
_start:
	csrw     sie, zero

	la       sp, stacks_start
	li       t0, 4096
	add      sp, sp, t0

	tail     kernel_main

	.section .data
	.align   12
	.global  my_offset
my_offset:
	.word    -2048

	.global  stacks_start
stacks_start:
	.skip    4096
