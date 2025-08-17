/* 说明链接脚本对应RISC-V */
OUTPUT_ARCH(riscv)
/* 指定程序入口为 _start */
ENTRY(_start)

/*
 * 描述输出文件的内存布局，SECTIONS 表示如何把输入文件的段映射到输出文件的段。
 * 在这里，我们引入了另外一个linker script
*/
SECTIONS
{
	INCLUDE "sbi/sbi_base.ld"
}
