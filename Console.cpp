//
//  Console.c
//  LC3-Sim-Core
//
//  Created by Lars Ott on 7/11/17.
//  Copyright © 2017 EIU. All rights reserved.
//

#include <stdio.h>
//#include "Simulator.h"
#include "Console.h"
#include "Saver.h"
#include <string.h>
#include <stdlib.h>

#define CASE(QUICK,FULL,SOURCE,INDEX) if (!strcmp(QUICK,SOURCE[INDEX])||!strcmp(FULL,SOURCE[INDEX]))

void printUsage();
console_err_t stepCommand(console_arg_t args);
console_err_t continueCommand(console_arg_t args);
console_err_t traceCommand(console_arg_t args);
console_err_t assembleCommand(console_arg_t args);
console_err_t runCommand(console_arg_t args);
console_err_t checkCommand(console_arg_t args);
char * getConsoleLine() {
    char * line = (char*)malloc(100);
    char * linep = (char*)line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = (char*)realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}


void startConsole() {


    while (true) {

        char* line = getConsoleLine();
        handleConsoleIn(line);
    }

}
enum CommandFlags{
    ONE_ARG = 1,
    TWO_ARG,
    COUNT,
    RESET,
    PC_COMMAND
};
bool handleConsoleIn(char * line)
{
    char* argv[16];
    int argc;
    int i;
    if (line == NULL || line[0] == '\0') {
        printUsage();
        return false;
    }

    for (int j = 0; j < 16; j ++) {
        argv[j] = 0;
    }

    // tokenize string
    // TODO allow paths with spaces, quotes, escapes, that stuff.
    argv[0] = strtok(line, " ");
    argc = 0;
    while (argv[argc]) {
        if (argc == 15) {
            printf("Too many tokens.\r\n");
            break;
        }
        argv[++argc] = strtok(NULL, " ");
    }
    if (argc == 15)
    {
        return false;
    }


    if (argc == 0) {
        printUsage();
        return false;
    }

    CASE("asm","as", argv,0) {
        // assembler
        // TODO assembleFile((char *)argv[2]);
    }
    CASE("b","break",argv,0)
    {
        CASE("-s","--set",argv,1)
        {
//            bool b = false;
//            mem_addr_t addr = Utility::unifiedInput2Val(QString(argv[2]),&b);
//            if(b)
//            {
//                breakpoint_t bre;
//                Computer::getDefault()->setMemBreakPoint(addr,bre);
//                return true;
//            }
            return false;
        }
        return false;
    }
    CASE("c","continue",argv,0)
    {

    }
    CASE("check","check",argv,0)
    {
        CASE("count","count",argv,1)
        {

        }
    }
    CASE("cl","clear",argv,0)
    {

    }
    CASE("d","dump", argv,0)
    {

    }
    CASE("h","help",argv,0)
    {
        printf("For information, type \"help --command\"");
        return true;
    }
    CASE("i","input",argv,0)
    {

    }
    CASE("l","list",argv ,0)
    {

    }
    CASE("ld","load",argv,0)
    {

    }
    CASE("n","next",argv ,0)
    {

    }
    CASE("p","print",argv,0)
    {

    }
    CASE("q","quit",argv ,0)
    {

    }
    CASE("r","reset",argv,0)
    {

    }
    CASE("s","step",argv ,0)
    {

    }
    CASE("save","save",argv,0)
    {
        bool a = 0;
        mem_addr_t start = Utility::unifiedInput2Val(QString(argv[1]),&a);
        if(!a){return false;}
        mem_addr_t stop  = Utility::unifiedInput2Val(QString(argv[2]),&a);
        if(!a){return false;}
        Saver::savePortable(start,stop,true);
        return true;
    }
    CASE("script","script",argv,0)
    {

    }
    CASE("set","set",argv,0)
    {

    }
    CASE("stop","stop",argv,0)
    {

    }
    CASE("tr","trace",argv,0)
    {

    }
    CASE("test","testing",argv,0)
    {

    }
    return false;
}
bool handleSaveCommand(char ** argv, int remaining)
{
    if(remaining <= 0 )
        return false;

    CASE("-f","--file",argv,0)
    {
        if(remaining<2)
        {
            return false;
        }

    }
    return false;
}


void printUsage() {
    printf("Usage: cmd [args...]\n");
    printf("Possible commands:\n");
    printf("asm inputFile\t\tAssembles the given file. puts the output in the same directory.\n");
    printf("save outputFile start stop \t Saves the program in plaintext.\n");
}

//console_err_t parseAndRunCommand(char *str)
//{

//}

console_err_t executeCommand(console_cmd_t cmd, console_arg_t args)
{
    console_err_t out;
    switch(cmd)
    {
        case run: return runCommand(args); break;
        case step: return stepCommand(args);break;
        case _continue: return continueCommand(args);break;
        case trace: return traceCommand(args);break;
        case assemble: return assembleCommand(args);break;
    default:
        return parseErr;
    }

}
console_err_t stepCommand(console_arg_t args)
{
    if(args == nullptr)
    {
        Computer::getDefault()->executeSingleInstruction();
        return consoleNoErr;
    }
    return parseErr;
}
console_err_t continueCommand(console_arg_t args)
{
    if(args == nullptr)
    {
        Computer::getDefault()->continueExecution();
        return consoleNoErr;
    }
    return parseErr;
}
console_err_t assembleCommand(console_arg_t args)
{

}
console_err_t traceCommand(console_arg_t args)
{

}
console_err_t runCommand(console_arg_t args)
{
//    if(!strcmp("-s",args[0]))
//    {
//        return stepCommand(nullptr);
//    }
//    return parseErr;
}
console_err_t testCommand(console_arg_t args)
{

}
console_err_t saveCommand(console_arg_t args)
{

}
console_err_t checkCommand(console_arg_t args)
{
    qDebug("attempting to check");
    static int checkPassCounter = 0;
    static int checkFailCounter = 0;
    CommandFlags * argv = (CommandFlags*)args;
    switch(argv[0])
    {
    case 1:
    {
        switch(argv[0])
        {
        case CommandFlags::COUNT:
        {
            printf("Here I will print out the counts\n");
        }break;

        }
    }break;
    default:
        break;
    }
    return consoleNoErr;
}

