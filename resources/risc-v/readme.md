# readme

## 单步调试

### 启动程序

```bash
# 切换到对应实现目录
cd riscv_programming_practice/chapter_3/lab3_1/benos

# 编译代码
make clean && make

# 在 qemu 中执行debug，如果只是执行程序可以使用 make run
make debug

# 也可以直接执行以下命令
# qemu-system-riscv64 -nographic -machine virt -m 128M  -bios mysbi.bin  -device loader,file=benos.bin,addr=0x80200000  -kernel benos.elf
```

### DEBUG

```bash
# 在新的终端中启动GDB工具，mysbi.elf 是我们 make 编译得到的系统文件
gdb-multiarch --tui mysbi.elf

# 在GDB命令行执行如下命令
target remote localhost:1234

# 在 _start 函数处打断点
b _start

# 执行到断点处，并根据需求调试
c
```

## 链接脚本（Linker Script, LS）

`.ld` 文件使用的是**链接器脚本语言（Linker Script Language）**，这是一种专门为链接器（如 GNU `ld`）设计的领域特定语言（DSL），用于描述程序的内存布局和段（Section）分配规则。

### **内存区域定义（MEMORY）**

用 `MEMORY` 块声明不同的内存区域（如 ROM、RAM），指定起始地址（ORIGIN）和大小（LENGTH）。

```less
MEMORY {
  flash (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  ram   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}
```

### **段分配（SECTIONS）**

用 `SECTIONS` 块定义目标文件中的段（如 `.text`、`.data`）如何映射到 `MEMORY` 中声明的内存区域。

```less
SECTIONS {
  .text : { *(.text) } > flash  # 所有代码段放入 flash 区域
  .data : { *(.data) } > ram    # 初始化数据放入 ram 区域
}
```

### **符号定义**

可自定义符号（如内存边界、入口点），供代码中引用。

```less
ENTRY(_start)  # 定义程序入口点为 _start 符号
__heap_start = .;  # 定义堆起始地址为当前位置
```

