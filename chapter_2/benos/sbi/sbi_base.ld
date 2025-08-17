    /*
     * 设置sbi的加载入口地址为0x80000000
     *
     */

    /*
     * '.' 代表当前位置计数器（Location Counter, LC
     * 这里把 .text 段的链接地址设置为 0x8000000，其中链接地址指的是加载地址（load address）
     */
	. = 0x80000000,

    /*
     * 其中 *(.text.boot) 可以表示这个文件的 .text.boot 段由所有输入文件的 .text.boot 段组成
     */
	.text.boot : { *(.text.boot) }
	.text : { *(.text) }
	.rodata : { *(.rodata) }
	.data : { *(.data) }

	. = ALIGN(0x8);

	bss_begin = .;
	.bss : { *(.bss*) } 
	bss_end = .;
