#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"
#include "memory.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;
// 内存管理器
MemoryManager memoryManager;

void first_thread(void *arg)
{
    // 第1个线程不可以返回
    // stdio.moveCursor(0);
    // for (int i = 0; i < 25 * 80; ++i)
    // {
    //     stdio.print(' ');
    // }
    // stdio.moveCursor(0);
    char *p1 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 1);
    char *p2 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 1);
    char *p3 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 1);
    char *p4 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 1);
    char *p5 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 1);

    memoryManager.LRU_kernel.printLRU();
    p3[5] = '8';
    printf("visit p3:0x%x\n",(int)p3);
    int timedelay = 10000000;
    while(timedelay--){}
    memoryManager.LRU_kernel.printLRU();
    p2[1] = '3';
    printf("visit p2:0x%x\n",(int)p2);
    timedelay = 10000000;
    while(timedelay--){}
    memoryManager.LRU_kernel.printLRU();
    p3[3] = '2';
    printf("visit p3:0x%x\n",(int)p3);
    timedelay = 10000000;
    while(timedelay--){}
    memoryManager.LRU_kernel.printLRU();

    timedelay = 1000000000;
    while(timedelay--){}

    for(int i = 0;i < 1600;i++)
    {
        memoryManager.allocatePages(AddressPoolType::KERNEL, 10);
        if(i == 1598 || i == 1599 || i == 1597 || i == 1596)
        {
            timedelay = 1000000000;
            while(timedelay--){}
        }

    }

    asm_halt();
}

extern "C" void setup_kernel()
{

    // 中断管理器
    interruptManager.initialize();
    interruptManager.enableTimeInterrupt();
    interruptManager.setTimeInterrupt((void *)asm_time_interrupt_handler);

    // 输出管理器
    stdio.initialize();

    // 进程/线程管理器
    programManager.initialize();

    // 内存管理器
    memoryManager.openPageMechanism();
    memoryManager.initialize();

    // 创建第一个线程
    int pid = programManager.executeThread(first_thread, nullptr, "first thread", 1);
    if (pid == -1)
    {
        printf("can not execute thread\n");
        asm_halt();
    }

    ListItem *item = programManager.readyPrograms.front();
    PCB *firstThread = ListItem2PCB(item, tagInGeneralList);
    firstThread->status = RUNNING;
    programManager.readyPrograms.pop_front();
    programManager.running = firstThread;
    asm_switch_thread(0, firstThread);

    asm_halt();
}
