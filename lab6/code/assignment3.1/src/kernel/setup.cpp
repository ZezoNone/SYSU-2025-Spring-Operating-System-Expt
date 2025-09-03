#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;

Semaphore chopsticks[5];

void philosopher(void *arg)
{
    int i = programManager.running->pid - 1;
    do{
        chopsticks[i].P();
        printf("%s picks up one chopstick\n",programManager.running->name);
        /*unsigned long deadlockTime = randLCG();
        while(deadlockTime){
            deadlockTime--;
        }*/
        chopsticks[(i+1)%5].P();
        printf("%s picks up tow chopsticks\n",programManager.running->name);
        printf("%s start to eat\n",programManager.running->name);
        unsigned long eatTime = randLCG();
        while(eatTime){
            eatTime--;
        }
        chopsticks[i].V();
        chopsticks[(i+1)%5].V();
        printf("%s have finished eating and drops two chopsticks\n",programManager.running->name);
        printf("%s start to think \n",programManager.running->name);
        unsigned long thinkTime = randLCG();
        while(thinkTime){
            thinkTime--;
        }
        printf("%s stops thinking\n",programManager.running->name);

    }while(true);
}


void first_thread(void *arg)
{
    // 第1个线程不可以返回
    stdio.moveCursor(0);
    for (int i = 0; i < 25 * 80; ++i)
    {
        stdio.print(' ');
    }
    stdio.moveCursor(0);

    for(int i = 0;i < 5;i++)
    {
        chopsticks[i].initialize(1);
    }

    programManager.executeThread(philosopher,nullptr,"philosopher_1",1);
    programManager.executeThread(philosopher,nullptr,"philosopher_2",1);
    programManager.executeThread(philosopher,nullptr,"philosopher_3",1);
    programManager.executeThread(philosopher,nullptr,"philosopher_4",1);
    programManager.executeThread(philosopher,nullptr,"philosopher_5",1);

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
