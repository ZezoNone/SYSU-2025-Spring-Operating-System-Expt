#ifndef PROGRAM_H
#define PROGRAM_H

#include "os_type.h"

class Program
{
private:
    uint direction;
    uint8 color;
    uint charIndex;
    uint row;
    uint col;
    uint left;
    uint right;       
    uint top;
    uint bottom;  

    void draw();

public:
    Program();
    void initialize();

    void pro_start();
};

#endif