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
    std::map<mem_addr_t, QString> commentDict;
    std::map<mem_addr_t, data_t> dataDict;

    /** used to annotate comments from lines that do not have instructions
     * \brief annotatedComment
     */
    QString annotatedComment;

    /** How many lines the program has.
     * /brief programLength
     */
    uint16_t programLength;
    /** starting address of the program being assembled
     */
    val_t startingAddress;
    /** ending address of the program being assembled
     */
    val_t endingAddress;
    /** used during assembly runs. set to true when .END is encountered
     * \brief hitDotEND
     */
    bool hitDotEND;

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

    data_t getNumberType(std::string num);
    void cleanComments();
    void assembleFileButForSomeReasonTheOtherOneDoesntWork(const char *infile, const char *outFile);
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

    /** Returns all the comments for an address as a space separated string
     * /brief commentForAddress
     * /param addr
     * /return
     */
    QString commentForAddress(mem_addr_t addr);

    /** Passes all comments from the assambled file to a computer. Replaces all existing labels in the range of the assembled file
     * /brief passCommentsToComputer
     * /param comp
     */
    void passCommentsToComputer(Computer* comp);

    data_t dataTypeForAddress(mem_addr_t addr);

    void passDataTypesToComputer(Computer* comp);
    
    val_t singleLineAssemble(QString machineCode,mem_addr_t addr);
};


#endif //LC3_ASSAMBLER_ASSAMBLER_H
