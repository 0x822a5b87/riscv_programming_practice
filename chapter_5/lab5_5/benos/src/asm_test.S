	.align   3
	.section .text

.macro op_func, label, a, b
	mv a0, \a
	mv a1, \b
	call add_\label
.endm

add_1:
	add a0, a1, a0
	ret

add_2:
	add a0, a1, a0
	ret

.global macro_test_1
macro_test_1:
	addi sp, sp, -32
	sd   ra, 24(sp)

	op_func 1, a0, a1

	ld   ra, 24(sp)
	addi sp, sp, 32

	ret


.global macro_test_2
macro_test_2:
	addi sp, sp, -32
	sd   ra, 24(sp)

	op_func 2, a0, a1

	ld   ra, 24(sp)
	addi sp, sp, 32

	ret