.align 3
.globl my_test_data
my_test_data:
	.dword 0x12345678abcdabcd

.global load_store_test
load_store_test:
    li a0, 0x80200000        # load data
    li a1, 16

    lb t1, (a0)
    lb t1, 4(a0)
    lb t1, -4(a0)

    ld t1, (a0)
    lb t1, 4(a0)

    # (gdb) i r t0
    # t0             0xffffffff8034f000       0xffffffff8034f000
    # (gdb) n
    # (gdb) xQuit
    # (gdb) i r t0
    # t0             0x400000 0x400000
    # (gdb) c
    lui t0, 0x8034f
    lui t0, 0x400

	ret
