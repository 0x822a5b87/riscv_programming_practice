# void *memset(void *s, int c, size_t count)
# {
# 	char *xs = s;
#
# 	while (count--)
# 		*xs++ = c;
# 	return s;
# }
	.global __my_memset_16bytes_asm
__my_memset_16bytes_asm:
	li      t0, 0                   # t0 = 0
.loop:
	beqz a2, .exit
	sd      a1, (a0)                # *(a0) = a1
	sd      a1, 8(a0)               # *(a0 + 8) = a1
	addi    t0, t0, 16              # t0 = t0 + 16。注意，这里是16个字节对齐的。
	blt     t0, a2, .loop           # if t0 < a2; goto .loop
	ret

.exit:
	ret