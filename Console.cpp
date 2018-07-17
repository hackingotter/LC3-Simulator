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

#include <string.h>
#include <stdlib.h>

#define CASE(QUICK,FULL,SOURCE,INDEX) if (!strcmp(QUICK,SOURCE[INDEX])||!strcmp(FULL,SOURCE[INDEX]))

void printUsage();
console_err_t stepCommand(console_arg_t args);
console_err_t continueCommand(console_arg_t args);
console_err_t traceCommand(console_arg_t args);
console_err_t assembleCommand(console_arg_t args);
console_err_t runCommand(console_arg_t args);

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

    char* argv[16];
    int argc;
    int i;

    while (true) {

        char* line = getConsoleLine();

        if (line == NULL || line[0] == '\0') {
            printUsage();
            continue;
        }

        for (int j = 0; j < 16; j ++) {
            argv[j] = 0;
        }

        // tokenize string
        // TODO allow paths with spaces, quotes, escapes, that stuff.
        argv[0] = strtok(line, " ");
        argc = 0;
        while (argv[i]) {
            if (argc == 15) {
                printf("Too many token.\r\n");
                break;
            }
            argv[++argc] = strtok(NULL, " ");
        }
        if (argc == 15)
            continue;


        if (argc == 0) {
            printUsage();
            continue;
        }

        if (!strcmp("asm", argv[1])||!strcmp("as", argv[1])) {
            // assembler
            // TODO assembleFile((char *)argv[2]);
        }
        if (!strcmp("b", argv[1])||!strcmp("break",argv[1]))
        {

        }
        if (!strcmp("c", argv[1])||!strcmp("continue",argv[1]))
        {

        }
        if (!strcmp("ch",argv[1])||!strcmp("check",argv[1]))
        {

        }
        if (!strcmp("cl",argv[1])||!strcmp("clear",argv[1]))
        {

        }
        if (!strcmp("d", argv[1])||!strcmp("dump", argv[1]))
        {

        }
        if (!strcmp("i", argv[1])||!strcmp("input",argv[1]))
        {

        }
        CASE("l","list",argv,1)
        {

        }
        CASE("ld","load",argv,1)
        {

        }
        CASE("n","next",argv,1)
        {

        }
        CASE("p","print",argv,1)
        {

        }
        CASE("q","quit",argv,1)
        {

        }
        CASE("r","reset",argv,1)
        {

        }
        CASE("s","step",argv,1)
        {

        }
        CASE("script","script",argv,1)
        {

        }
        CASE("set","set",argv,1)
        {
            printf("setting");
        }
        CASE("stop","stop",argv,1)
        {

        }
        CASE("tr","trace",argv,1)
        {

        }
    }

}


void printUsage() {
    printf("Usage: cmd [args...]\n");
    printf("Possible commands:\n");
    printf("asm inputFile\t\tAssembles the given file. puts the output in the same directory.\n");
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
