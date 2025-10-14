# readme

## example4-1

### asm

```asm
	.section .rodata
	.align   3
.string:
.string "data:
	%d       %d %d %d %d %d %d %d %d %d\n"

data:
	.word    1, 2, 3, 4, 5, 6, 7, 8, 9, -1

	.text
	.align   2

	.global  main
main:
	addi     sp,sp,-48                     # 栈往下扩展48字节
	sd       ra,40(sp)					   # 将return address存放在栈中，最终写入地址 = 新 sp + 40 = (S - 49) + 40 = S - 8。

	la       a0, .string                   # a0传递第一个参数：.string字符串
	li       a1,1                          # a1-a7 传递printf()前7个参数
	li       a2,2
	li       a3,3
	li       a4,4
	li       a5,5
	li       a6,6
	li       a7,7

	li       t0,8                          # printf()第8-10个参数通过栈来传递，由于sd的参数只能是寄存器，所以我们需要使用临时寄存器t0来辅助
	sd       t0,0(sp)
	li       t0,9
	sd       t0,8(sp)
	li       t0,-1
	sd       t0,16(sp)

	call     printf                        # 调printf()函数

	ld       ra,40(sp)                     # 从栈中恢复ra返回地址
	li       a0,0                          # 设置main函数返回值为0
	addi     sp,sp,48                      # SP回到原点

	ret
```

### stack

```mermaid
block
  block
    columns 1
    new("S + 48")
    new1("S + 40")
    new2("S + 32")
    new3("S + 24")
    new4("S + 16")
    new5("S + 8")
    new6("S = sp - 48")
  end
  block
    columns 1
    stack1("0x80200000")
    stack2("0x801FFFF8")
    stack3("0x801FFFF0")
    stack4("0x801FFFEC")
    stack5("0x801FFFE0")
    stack6("0x801FFFD8")
    stack7("0x801FFFD0")
  end
  block
    columns 1
    start("sp")
    sp1("sp - 8")
    sp2("sp - 16")
    sp3("sp - 24")
    sp4("sp - 32")
    sp5("sp - 40")
    sp6("sp - 48")
  end

  start --> stack1
  new1 --> stack2
  
  class start pink
  class new1 pink
  class stack2 green
  class stack1 dotted

  classDef pink   fill:#FFCCCC,stroke:#333,ont-weight: bold;
  classDef green  fill:#696,color: #fff,font-weight: bold;
  classDef purple fill:#969,stroke:#333;
  classDef dotted fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```