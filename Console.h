//
//  Console.h
//  LC3-Sim-Core
//
//  Created by Lars Ott on 3/29/17.
//  Copyright © 2017 EIU. All rights reserved.
//

#ifndef Console_h
#define Console_h
#include "computer.h"

typedef enum console_err_t {
    consoleNoErr,
    invalidCmd,
    parseErr
} console_err_t;

typedef enum console_cmd_t {
    help,
    assemble,
    _break,
    _continue,
    check,
    clear,
    dump,
    input,
    list,
    load,
    next,
    print,
    quit,
    _reset,
    step,
    set,
    stop,
    trace,
    run, // TODO console commands
    assembleNLoad

} console_cmd_t;

typedef void* console_arg_t; // TODO improve this

static bool traceState;

static FILE tracefile;

void startConsole();

/** runs a console command.
 * \param cmd the command to run.
 * \param args arguments for that command 
 * \return an error if applicable.
 */
console_err_t executeCommand(console_cmd_t cmd, console_arg_t args);

/** pareses and runs a command for the console including arguments
 * \param str the string to parse and run
 * \return an error if applicable
 */
console_err_t parseAndRunCommand(char *str);



/** gives a description for each of the console errors
 * \param err the error to be described
 * \return a string containing the description.
 */
char * getConsoleErrorDescription(console_err_t err);

// TODO printing

#endif /* Console_h */
