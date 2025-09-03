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

Semaphore W;
Semaphore R;

static uint32 reader_count = 0;

void reader(void *arg)
{
    do{
        if(reader_count == 0)
        {
            W.P();
        }
        printf("%s is reading\n",programManager.running->name);
        R.P();
        reader_count++;
        printf("There are %d reader(s) now\n",reader_count);
        R.V();
        unsigned int readTime = randLCG();
        while(readTime){
            readTime--;
        }
        printf("%s stops reading\n",programManager.running->name);
        R.P();
        reader_count--;
        printf("There are %d reader(s) now\n",reader_count);
        R.V();
        if(reader_count == 0)
        W.V();
        unsigned int stopTime = randLCG();
        while(stopTime){
            stopTime--;
        }
    }while(true);
}

void writer(void *arg)
{
    do{
        W.P();
        printf("%s is writing\n",programManager.running->name);
        unsigned int writeTime = randLCG();
        while(writeTime){
            writeTime--;
        }
        printf("%s stops writing\n",programManager.running->name);
        W.V();
        unsigned int stopTime = randLCG();
        while(stopTime){
            stopTime--;
        }
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

    W.initialize(1);
    R.initialize(1);

    programManager.executeThread(reader, nullptr, "reader_1", 1);
    programManager.executeThread(reader, nullptr, "reader_2", 1);
    programManager.executeThread(writer, nullptr, "writer_1", 1);
    programManager.executeThread(reader, nullptr, "reader_3", 1);
    programManager.executeThread(reader, nullptr, "reader_4", 1);
    programManager.executeThread(reader, nullptr, "reader_5", 1);
    programManager.executeThread(writer, nullptr, "writer_2", 1);

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
