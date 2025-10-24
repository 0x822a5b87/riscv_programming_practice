# CMake 最低版本要求
cmake_minimum_required(VERSION 3.13)

# 定义项目名称和支持的语言 (C, C++, Assembly)
project(benos_project C CXX ASM)

# --- 1. 工具链和编译选项配置 ---

# 明确告诉 CMake 我们正在进行交叉编译
# 这对于正确处理 include 和链接非常重要
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

# 设置 RISC-V 交叉编译器前缀
# 你可以通过命令行 -DGNU=... 来覆盖，或者在这里直接修改
if(NOT DEFINED GNU)
    set(GNU riscv64-linux-gnu)
endif()
set(CMAKE_C_COMPILER ${GNU}-gcc)
set(CMAKE_ASM_COMPILER ${GNU}-gcc)
set(CMAKE_LINKER ${GNU}-ld)
set(CMAKE_OBJCOPY ${GNU}-objcopy)

# --- 2. 编译选项 (COPS) ---

# 通用编译选项 (对应 Makefile 中的 COPS)
# -g: 生成调试信息
# -O0: 无优化
# -Wall: 开启所有警告
# -nostdlib -nostdinc: 不使用标准库和标准头文件 (裸机开发)
# -mcmodel=medany: RISC-V 内存模型
# -mabi=lp64: 64位整数/32位指针 ABI
# -march=rv64imafd: RISC-V 指令集架构
# -fno-PIE: 不生成位置无关可执行文件
# -fomit-frame-pointer: 优化掉帧指针
add_compile_options(
        -g
        -O0
        -Wall
        -nostdlib
        -nostdinc
        -mcmodel=medany
        -mabi=lp64
        -march=rv64imafd
        -fno-PIE
        -fomit-frame-pointer
)

add_link_options(
        -nostdlib      # 【关键】链接时不链接标准库
        -nostartfiles  # 【关键】不使用标准启动文件 (如 crt0.o)
        -ffreestanding # 【关键】告诉编译器这是一个独立环境程序
)

# --- 3. 源文件和头文件 ---

# --- 3.1 benos (内核) ---
set(BENOS_SRC_DIR src)
file(GLOB_RECURSE BENOS_C_FILES "${BENOS_SRC_DIR}/*.c")
file(GLOB_RECURSE BENOS_ASM_FILES "${BENOS_SRC_DIR}/*.S")

# 定义 benos.elf 可执行目标
add_executable(benos.elf ${BENOS_C_FILES} ${BENOS_ASM_FILES} ${BENOS_HEADER_FILES})

# 指定 benos 的头文件目录
target_include_directories(benos.elf PRIVATE "include")

# 指定 benos 的链接脚						本
target_link_options(benos.elf PRIVATE "-T${CMAKE_CURRENT_SOURCE_DIR}/${BENOS_SRC_DIR}/linker.ld")

# --- 3.2 SBI ( Supervisor Binary Interface ) ---
set(SBI_SRC_DIR sbi)
file(GLOB_RECURSE SBI_C_FILES "${SBI_SRC_DIR}/*.c")
file(GLOB_RECURSE SBI_ASM_FILES "${SBI_SRC_DIR}/*.S")
file(GLOB_RECURSE SBI_HEADER_FILES "${SBI_SRC_DIR}/*.h")

# 定义 mysbi.elf 可执行目标
add_executable(mysbi.elf ${SBI_C_FILES} ${SBI_ASM_FILES} ${SBI_HEADER_FILES})

# 指定 SBI 的头文件目录 (与 benos 共享)
target_include_directories(mysbi.elf PRIVATE "include")

# 指定 SBI 的链接脚本
target_link_options(mysbi.elf PRIVATE "-T${CMAKE_CURRENT_SOURCE_DIR}/${SBI_SRC_DIR}/sbi_linker.ld")

# --- 3.3 Payload (SBI + benos 合并镜像) ---

# 定义 benos_payload.elf 可执行目标
# 注意：这里我们只链接 SBI 的目标文件，因为 benos.bin 会被 loader 加载
# 这与 Makefile 的逻辑稍有不同，但更符合 CMake 的目标模型
add_executable(benos_payload.elf ${SBI_C_FILES} ${SBI_ASM_FILES})

# 指定 payload 的头文件目录
target_include_directories(benos_payload.elf PRIVATE "include")

# 指定 payload 的链接脚本
target_link_options(benos_payload.elf PRIVATE "-T${CMAKE_CURRENT_SOURCE_DIR}/${SBI_SRC_DIR}/sbi_linker_payload.ld")

# --- 4. 自定义命令和目标 (生成 .bin 文件, 运行, 调试) ---

# --- 4.1 生成 .bin 文件 ---
add_custom_command(
        OUTPUT benos.bin
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:benos.elf> benos.bin
        DEPENDS benos.elf
        COMMENT "Generating benos.bin from benos.elf"
)

add_custom_command(
        OUTPUT mysbi.bin
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:mysbi.elf> mysbi.bin
        DEPENDS mysbi.elf
        COMMENT "Generating mysbi.bin from mysbi.elf"
)

add_custom_command(
        OUTPUT benos_payload.bin
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:benos_payload.elf> benos_payload.bin
        DEPENDS benos_payload.elf
        COMMENT "Generating benos_payload.bin from benos_payload.elf"
)

# 创建一个目标来驱动 .bin 文件的生成
add_custom_target(build_bin ALL
        DEPENDS benos.bin mysbi.bin benos_payload.bin
)

# --- 4.2 QEMU 运行和调试目标 ---

# 运行 QEMU (模拟 make run)
add_custom_target(run
        COMMAND qemu-system-riscv64 -nographic -machine virt -m 128M -bios mysbi.bin -device loader,file=benos.bin,addr=0x80200000 -kernel $<TARGET_FILE:benos.elf>
        DEPENDS build_bin
        COMMENT "Running QEMU with benos and mysbi"
        USES_TERMINAL
)

# 启动 QEMU 并等待 GDB 连接 (模拟 make debug)
add_custom_target(debug
        COMMAND qemu-system-riscv64 -nographic -machine virt -m 128M -bios mysbi.bin -device loader,file=benos.bin,addr=0x80200000 -kernel $<TARGET_FILE:benos.elf> -S -s
        DEPENDS build_bin
        COMMENT "Starting QEMU in debug mode (waiting for GDB on port 1234)"
        USES_TERMINAL
)

# 运行 payload (模拟 make payload)
add_custom_target(payload
        COMMAND qemu-system-riscv64 -nographic -machine virt -m 128M -bios none -device loader,file=benos_payload.bin,addr=0x80000000
        DEPENDS build_bin
        COMMENT "Running QEMU with benos_payload.bin"
        USES_TERMINAL
)

# --- 5. 清理目标 ---
# CMake 自动提供了 "clean" 目标，它会清除所有生成的文件。
# 如果你想清理额外的文件 (如 .map, .bin)，可以自定义，但通常不需要。
