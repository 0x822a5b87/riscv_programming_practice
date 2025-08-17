SECTIONS
{
    /* 设置当前内存地址为 0x80200000 */
	. = 0x80200000,

    /* 指定 .text.boot 自定义段的输入字段 */
	.text.boot : { *(.text.boot) }
	.text : { *(.text) }
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	. = ALIGN(0x8);
	bss_begin = .;
	.bss : { *(.bss*) } 
	bss_end = .;
}

