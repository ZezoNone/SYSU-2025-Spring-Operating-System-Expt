#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "memory.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 内存管理器
MemoryManager memoryManager;


extern "C" void setup_kernel()
{
    // 中断管理器
    interruptManager.initialize();
    for (uint i = 11; i < 15; ++i)
    {
        interruptManager.setInterruptDescriptor(i, (uint32)asm_segment_interrupt, 0);
    }
    // 输出管理器
    stdio.initialize();
    // 内存管理器
    memoryManager.openPageMechanism();
    // 除零错误
    // int t = 1 / 0;
    // 段错误触发
    //*(int*)0x100000 = 1;

    asm_cause_segment_fault();

    asm_halt();
}
