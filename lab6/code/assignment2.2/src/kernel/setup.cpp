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

Semaphore RW;//if reader is read,writer can't enter
Semaphore R;//ensure reader_count is in critical section
Semaphore S;//if a writer comes, stop reader to enter
Semaphore W;//ensure writer_count is in critical section

static uint32 reader_count = 0;
static uint32 write_count = 0;

void reader(void *arg)
{
    do{
        S.P();
        R.P();
        if(reader_count == 0)
        {
            RW.P();
        }
        printf("%s is reading\n",programManager.running->name);
        reader_count++;
        printf("There are %d reader(s) now\n",reader_count);
        R.V();
        S.V();
        unsigned int readTime = randLCG();
        while(readTime){
            readTime--;
        }
        printf("%s stops reading\n",programManager.running->name);
        R.P();
        reader_count--;
        printf("There are %d reader(s) now\n",reader_count);
        if(reader_count == 0)
        RW.V();
        R.V();
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
        if(write_count == 0)
        {
            S.P();
        }
        write_count++;
        W.V();
        RW.P();
        printf("%s is writing\n",programManager.running->name);
        unsigned int writeTime = randLCG();
        while(writeTime){
            writeTime--;
        }
        printf("%s stops writing\n",programManager.running->name);
        RW.V();
        W.P();
        write_count--;
        if(write_count == 0)
        {
            S.V();
        }
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
    RW.initialize(1);
    S.initialize(1);

    programManager.executeThread(reader, nullptr, "reader_1", 1);
    programManager.executeThread(reader, nullptr, "reader_2", 2);
    programManager.executeThread(writer, nullptr, "writer_1", 1);
    programManager.executeThread(reader, nullptr, "reader_3", 3);
    programManager.executeThread(reader, nullptr, "reader_4", 4);
    programManager.executeThread(reader, nullptr, "reader_5", 5);
    programManager.executeThread(writer, nullptr, "writer_2", 2);

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
