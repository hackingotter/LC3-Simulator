//
//  Console.c
//  LC3-Sim-Core
//
//  Created by Lars Ott on 7/11/17.
//  Copyright © 2017 EIU. All rights reserved.
//

#include <stdio.h>
//#include "Simulator.h"
#include <string.h>
#include <stdlib.h>



void printUsage();


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

        if (!strcmp("asm", argv[1])) {
            // assembler
            // TODO assembleFile((char *)argv[2]);
        }
    }

}


void printUsage() {
    printf("Usage: cmd [args...]\n");
    printf("Possible commands:\n");
    printf("asm inputFile\t\tAssembles the given file. puts the output in the same directory.\n");
}
