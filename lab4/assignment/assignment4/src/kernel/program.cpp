#include "interrupt.h"
#include "os_type.h"
#include "os_constant.h"
#include "asm_utils.h"
#include "stdio.h"
#include "program.h"

extern STDIO stdio;

Program::Program()
{
    initialize();
}
void Program::initialize()
{
    direction = 0;
    color = 0x1f;
    charIndex = 0;
    row = 0;
    col = 0;
    left = 0;
    right = 79;       
    top = 0;
    bottom = 24;  

    for (int i = 0; i < 25; ++i)
        {
            for(int j = 0; j < 80; ++j)
            stdio.print(i, j, ' ', 0x07);
        }

        char str[] = "23336342-ZWX";

        stdio.moveCursor(995);
        for(int i = 0; str[i]; ++i ) {
            stdio.print(str[i],0x74);
        }
}

void Program::pro_start()
{
    if(direction == 0)
    {
        col++;
        if(col >= right)
        {
            direction = 1;
        } 
        draw();
    }
    else if(direction == 1)
    {
        row++;
        if(row >= bottom)
        {
            direction = 2;
        }       
        draw();
    }
    else if(direction == 2)
    {
        col--;
        if(col <= left)
        {
            direction = 3;
        }  
        draw();
    }
    else
    {
        row--;
        if(row <= top)
        {
            direction = 0;
        }    
        draw();
    }
}

void Program::draw()
{
    uint8 chars[] = "23336342";
    stdio.print(row,col,chars[charIndex],color);
    color++;
    charIndex++;
    if(charIndex >= 8)
    {
        charIndex = 0;
    }
}