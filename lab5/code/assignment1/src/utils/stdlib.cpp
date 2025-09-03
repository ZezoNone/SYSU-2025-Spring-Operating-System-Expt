#include "os_type.h"

template<typename T>
void swap(T &x, T &y) {
    T z = x;
    x = y;
    y = z;
}

void stoi(char *numStr, uint32 &num)
{
    for(uint32 i = 0;numStr[i]; ++i) {
        num *= 10;
        num += numStr[i] - '0';
    }
}

void itos(char *numStr, uint32 num, uint32 mod) {
    // 只能转换2~26进制的整数
    if (mod < 2 || mod > 26 || num < 0) {
        return;
    }

    uint32 length, temp;

    // 进制转换
    length = 0;
    while(num) {
        temp = num % mod;
        num /= mod;
        numStr[length] = temp > 9 ? temp - 10 + 'A' : temp + '0';
        ++length;
    }

    // 特别处理num=0的情况
    if(!length) {
        numStr[0] = '0';
        ++length;
    }

    // 将字符串倒转，使得numStr[0]保存的是num的高位数字
    for(int i = 0, j = length - 1; i < j; ++i, --j) {
        swap(numStr[i], numStr[j]);
    }
    
    numStr[length] = '\0';
}

void ftos(char *numStr, float32 num,uint32 precision = 6)
{
    uint32 length,digits,tempi,temp;
    float32 tempf;

    length = 0;
    digits = 0;
    tempf = num;

    while(tempf - (int)tempf != 0)
    {
        digits += 1;
        tempf = tempf - (int)tempf;
        tempf *= 10;
    }
    
    tempf = num;

    for(uint32 i = 0;i < precision;i++)
    {
        tempf *= 10;
    }
    if(digits > precision)
    {
        if((tempf - (int)tempf) * 10 > 5)
        {
            tempf += 1;
        }
    }

    tempi = tempf;

    while(tempi) {
        temp = tempi % 10;
        tempi /= 10;
        numStr[length] = temp + '0';
        ++length;
        if(length == precision)
        {
            numStr[length] = '.';
            ++length;
        }
    }
    if((int)num == 0)
        numStr[length++] = '0';

    for(uint32 i = 0, j = length - 1; i < j; ++i, --j) {
        swap(numStr[i], numStr[j]);
    }
    numStr[length] = '\0';
}

void ftoes(char *numStr, float32 num, uint32 precision = 6)
{
    uint32 length,digits,tempi,temp;
    float32 tempf;
    int index = 0;
    length = 0;
    digits = 0;

    if((int)num == 0)
    {
        while((int)num == 0)
        {
            num *= 10;
            index--;
        }
    }
    else
    {
        while((int)num / 10 != 0 && num != 10)
        {
            num /= 10;
            index++;
        }
    }
    tempf = num;
    while(num - (int)num != 0)
    {
    digits += 1;
    num = num - (int)num;
    num *= 10;
    }   

    for(uint32 i = 0;i < precision;i++)
    {
        tempf *= 10;
    }
    if(digits > precision)
    {
        if((tempf - (int)tempf) * 10 > 5)
        {
            tempf += 1;
        }
    }

    tempi = tempf;

    while(tempi) {
        temp = tempi % 10;
        tempi /= 10;
        numStr[length] = temp + '0';
        ++length;
        if(length == precision)
        {
            numStr[length] = '.';
            ++length;
        }
    }
    for(uint32 i = 0, j = length - 1; i < j; ++i, --j) {
        swap(numStr[i], numStr[j]);
    }

    numStr[length++] = 'e';
    if(index < 0)
    {
        numStr[length++] = '-';
        index = -index;
    }
    char indexnum[33];
    itos(indexnum,index,10);
    for (int j = 0; indexnum[j]; ++j)
    {
        numStr[length++] = indexnum[j];
    }

    numStr[length] = '\0';
}
