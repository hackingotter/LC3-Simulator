//
// Created by Lars Ott on 8/24/17.
//

#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include "Utility.h"
#include "GlobalTypes.h"
#include "computer.h"
#include <QString>

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

private:


    const std::regex parserRegex;


    std::map<QString, uint16_t> labelDict;
    /** How many lines the program has.
     * /brief programLength
     */
    uint16_t programLength;
    /** starting address fo the program being assembled
     */
    val_t startingAddress;
    /** ending address fo the program being assembled
     */
    val_t endingAddress;

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

    std::map<QString, uint16_t>* labelDictCopy();

    /** Returns all the labels for an address as a space separated string
     * /brief labelForAddress
     * /param addr
     * /return
     */
    QString labelForAddress(mem_addr_t addr);

    /** Passes all labels from the assambled file to a computer. Replaces all existing labels in the range of the assembled file
     * /brief passLabelsToComputer
     * /param comp
     */
    void passLabelsToComputer(Computer* comp);
};


#endif //LC3_ASSAMBLER_ASSAMBLER_H
