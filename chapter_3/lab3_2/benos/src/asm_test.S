.align 3
.globl my_test_data
my_test_data:
	.dword 0x12345678abcdabcd

#define MY_OFFSET -2048

.align 3
.global load_store_test
load_store_test:
# auipc    t0, 1
# addi     t0, t0, -2048;
# ld       t1, -2048(t0)    # t1 = (double *) t0
#
# 在这段代码中，我们可以认为过程如下：
#
# t0 = PC + 0x1000
# t0 = t0 - 0x800
# t1 = *(t0 - 0x800)

	# 计算跳转目标值
	# ► 0x80200014 <_start+20> auipc t0, 1 T0 => 0x80201014 (stacks_start+16)
	# t0 = 0x80201014
	auipc    t0, 1

	# t0 = 0x80201014 - 0x800 = 0x80200814
	addi     t0, t0, -2048;

	# *T1   0x8002829300001297 —▸ 0 ◂— 0
	# pwndbg> print /x *(long long *) 0x80200014
	# $1 = 0x8002829300001297
	ld       t1, -2048(t0)    # t1 = (double *) t0

	li t0, 0x80200000

	lb t1, (t0)
	lb t1, 4(t0)
	lb t1, -4(t0)
	ld t1, (t0)
	lb t1, 4(t0)

	lui t0, 0x80200
	lui t1, 0x40200

    # la 和 lla 都是伪指令
    # load address，将一个符号的绝对地址加载到寄存器中
	# load local address，将一个符号的局部地址加载到寄存器中
	la t0, my_test_data
	lla t1, my_test_data

	ret

.global pc_related_test
pc_related_test:
    # pc             0x80200208       0x80200208 <pc_related_test>
    # t0             0x802001c8       2149581256

	auipc t0, 1
    # auipc t0, 1 的结果是将当前指令地址（pc）加上 0x1000（1 << 12），
	# pc             0x8020020c       0x8020020c <pc_related_test+4>
	# t0             0x80201208       2149585416

	addi t0, t0, MY_OFFSET
    # addi t0, t0, MY_OFFSET 的结果是将 t0 的值加上 -2048（MY_OFFSET）
	# pc             0x80200210       0x80200210 <pc_related_test+8>
	# t0             0x80200a08       2149583368
	# t1             0x802001c8       2149581256

	ld t1, MY_OFFSET(t0)
	# ld t1, MY_OFFSET(t0) 的结果是将 t0 的值加上 -2048（MY_OFFSET）作为地址，从该地址加载一个 64 位值到 t1
    # pc             0x80200214       0x80200214 <pc_related_test+12>
    # t0             0x80200a08       0x80200a08
    # t1             0x8002829300001297       0x8002829300001297

	ret
