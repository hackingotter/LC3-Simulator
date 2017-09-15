//
// Created by Lars Ott on 8/24/17.
//

#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include "Utility.h"

#ifndef LC3_ASSAMBLER_ASSAMBLER_H
#define LC3_ASSAMBLER_ASSAMBLER_H

typedef enum {
    numberOnly,
    labelOnly,
    numberOrLabel,
    stringOnly,
    none
} NumOrLabel;

typedef enum {
    PreRun,
    MainRun
} RunType;


class Assembler {

    std::regex parserRegex;

    std::map<std::string, uint16_t> labelDict;



    double parseNumber(std::string num);

    std::string unescapeString(std::string unescaped);

    void writeWord(std::ostream &stream, uint16_t value);

    void determineArgumentsOfOp(const std::string &instruction, bool thirdRegEmpty, int &nOfRegs, NumOrLabel &nOrL);

    uint16_t OpCodeForInstruction(std::string &op);

    uint16_t getConstantBits(const std::string &instruction, int nOfRegs, NumOrLabel nOrL);

    uint16_t getNumberOrOffset(const std::string &instruction, NumOrLabel nOrL, const std::string &opNumber, const std::string &opLabel,
                               uint16_t pc);

    std::string checkInput(const std::string &instruction, const std::string &firstReg, const std::string &secondReg, const std::string &thirdReg,
                      const std::string &opNumber, const std::string &opLabel, const std::string &opString, int nOfRegs, NumOrLabel nOrL);

    uint16_t getRegisters(std::string &instruction, int nOfRegs, std::string &firstReg, std::string &secondReg, std::string &thirdReg);

    void writeFill(std::ofstream &oStream, const std::string &opNumber, const std::string &opLabel);

// returns new pc
    uint16_t writeInstruction(RunType runType, std::string &instruction, std::string &firstReg, std::string &secondReg, std::string &thirdReg,
                              std::string &opNumber, std::string &opLabel, std::string &opString,
                              int nOfRegs, NumOrLabel nOrL,
                              std::ofstream &oStream, uint16_t pc);

// returns new pc
    uint16_t processLine(std::string &line, RunType runType, uint16_t pc, std::ofstream &oStream);

public:
    Assembler();

    void assembleFile(const char *inFile, const char *outFile);

};


#endif //LC3_ASSAMBLER_ASSAMBLER_H
