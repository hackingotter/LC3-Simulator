#include "Util.h"
#include "inttypes.h"
#include "stdio.h"
#include <stdlib.h>

#define TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)
#define TO_OCT(i) ('0'+i)
#define TO_BIN(i) ('0'+i)

QString getHexString(uint16_t val)
{
    char* out = (char *)malloc(sizeof(char) * 6);
    out[0]='x';
    out[1]=TO_HEX(((val&0xF000)>>12));
    out[2]=TO_HEX(((val&0x0F00)>>8));
    out[3]=TO_HEX(((val&0x00F0)>>4));
    out[4]=TO_HEX(((val&0x000F)));
    out[5] = '\0';

    return QString(out);
}

QString getBinString(uint16_t val)
{
    int i;
    char* out = (char *)malloc(sizeof(char) * 18);
    out[0] = 'b';
    for(i=1;i<=16;i++)out[i] = TO_BIN(((val&(0b1<<(16-i)))>>(16-i)));
    out[17] = '\0';
    return QString(out);
}
