#include "Memory.h"
#include "stdlib.h"

mem_loc_t _memory[0xFFFF];

mem_loc_t getMemLocation(mem_addr_t addr)
{
    return _memory[addr];
}

void setMemValue(mem_addr_t addr, val_t val)
{
    _memory[addr].value = val;
}

void setMemValuesBlock(mem_addr_t addr, size_t blockSize, val_t *vals)
{
    //Implemented for compiler's compliance.
}

val_t getMemValue(mem_addr_t addr)
{
    return _memory[addr].value;
}

val_t* getAllMemValues()
{
    mem_addr_t addr;
    val_t * ret = (val_t*)malloc(65535 * sizeof(val_t));

    if (!ret)
    {
        throw "Malloc error";
        return ret;
    }
    for(addr =0;addr<0xFFF;addr++)
    {
        ret[addr] = _memory[addr].value;
    }

    return ret;
}

void setMemLabel(mem_addr_t addr,label_t* newLabel)
{
    _memory[addr].label=newLabel;
}
void setMemLabelChar(mem_addr_t addr,char* labelString)
{
    label_t* label = (label_t*)malloc(sizeof (label_t));
    label->addr = addr;
    label->name = labelString;
    _memory[addr].label = label;
}

label_t* getMemLabel(mem_addr_t addr)
{
    return _memory[addr].label;
}

void setMemBreakPoint(mem_addr_t addr,breakpoint_t* breakpt){
    _memory[addr].breakpt = breakpt;
}

breakpoint_t* getMemBreakPoint(mem_addr_t addr)
{
    return _memory[addr].breakpt;
}

void setMemComment(mem_addr_t addr, char *comment)
{
    _memory[addr].comment=comment;
}

char* getMemComment(mem_addr_t addr)
{
    return _memory[addr].comment;
}


size_t loadProgramFile(char* path) {

    FILE *file = fopen(path, "r");

    if (!file)
        return 0;

    fseek(file, 0, SEEK_END);
    size_t fileLen = ftell(file);
    rewind(file);

    mem_addr_t startingAddr;
    val_t *buffer = (val_t *)malloc((fileLen/2 - 1)*sizeof(val_t));

    fread(&startingAddr, sizeof(mem_addr_t), 1, file);
    fread(buffer, sizeof(val_t), fileLen/2 - 1, file);

    setMemValuesBlock(startingAddr, fileLen/2 - 1, buffer);

    fclose(file);

    return fileLen/2 - 1;
}

