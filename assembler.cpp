//
// Created by Lars Ott on 8/24/17.
//

#include "Assembler.h"
#include <iostream>
using namespace std;


#define  addOpCode 0x1000
#define  andOpCode 0x5000
#define   brOpCode 0x0000
#define  jmpOpCode 0xC000
#define  jsrOpCode 0x4000
#define jsrrOpCode 0x4000 //special version of jsr
#define   ldOpCode 0x2000
#define  ldiOpCode 0xA000
#define  ldrOpCode 0x6000
#define  leaOpCode 0xE000
#define  notOpCode 0x9000
#define  retOpCode 0xC000 // same as jmp
#define  rtiOpCode 0x8000
#define   stOpCode 0x3000
#define  stiOpCode 0xB000
#define  strOpCode 0x7000
#define trapOpCode 0xF000
#define  mulOpCode 0xD000

#define  putsTrap 0xF022
#define  getcTrap 0xF020
#define   outTrap 0xF021
#define    inTrap 0xF023
#define putspTrap 0xF024
#define  haltTrap 0xF025

Assembler::Assembler() {

    parserRegex = regex(
            R"abc([ \t]*((?!(?:ADD|SUB|AND|(?:BR[nzp]{0,3})|JMP|JSRR|JSR|LDI|LDR|LD|LEA|NOT|RET|RTI|STI|STR|ST|TRAP|MUL|GETC|OUT|IN|PUTSP|PUTS|HALT)(?:\W|$))\w*)?[ \t]*(?:(?:(ADD|SUB|AND|(?:BR[nzp]{0,3})|JMP|JSRR|JSR|LDI|LDR|LD|LEA|NOT|RET|RTI|STI|STR|ST|TRAP|MUL|\.FILL|\.STRINGZ|\.ORIG|\.END|\.BLKW|PUTS|GETC|OUT|IN|PUTSP|HALT)(?:\W|$))[ \t]*(?:r(\d),?)?[ \t]*(?:r(\d),?)?[ \t]*(?:(?:r(\d))|((?:#|x|b|o|-(?!-))?-?[0-9A-F]+\.?[0-9]*)|(\w*)|((?:".*")|(?:'.*')))?)?[ \t]*(?:;+([\S \t]*))?[ \t]*$)abc",
            std::regex_constants::icase);

    labelDict = map<string, uint16_t> ();

}

void Assembler::assembleFile(const char *inFile, const char *outFile) {
    std::ifstream iStream(inFile, std::ios_base::in);
    std::ofstream oStream(outFile, std::ios_base::out | std::ios_base::binary);

    for (int runNum = 1; runNum <= 2; runNum++) {

        iStream.clear();
        iStream.seekg(0,ios::beg);

        if (!iStream.is_open()) {
            throw "Could not open in File";
        }
        if (!oStream.is_open()) {
            throw "Could not open out file";
        }

        std::string line;

        RunType runType = MainRun;
        uint16_t pc = 0xFFFF;

        switch (runNum) {
            case 1:
                runType = PreRun;
                break;
            case 2:
                runType = MainRun;
                break;
            default:
                break;
        }

        while (std::getline(iStream, line)) {
            pc = processLine(line, runType, pc, oStream);
        }
    }

    iStream.close();
    oStream.close();
}

uint16_t Assembler::processLine(string &line, RunType runType, uint16_t pc, ofstream &oStream) {
    std::smatch match;

    if (!regex_match(line, match, parserRegex)) {
        throw "Could not parse line: " + getHexString(pc).toStdString()+ ":"+ line;
    }

    std::string label = match[1];
    std::string instruction = match[2];
    transform(instruction.begin(), instruction.end(), instruction.begin(), ::toupper);
    std::string firstReg = match[3];
    std::string secondReg = match[4];
    std::string thirdReg = match[5];
    std::string opNumber = match[6];
    string opLabel = match[7];
    string opString = match[8];
    if (!opString.empty()) {
        opString = opString.substr(1, opString.length() - 2);
    }
    string comment = match[9];

    int nOfRegs = 0;
    NumOrLabel nOrL = none;

    determineArgumentsOfOp(instruction, thirdReg.empty(), nOfRegs, nOrL);

    if (runType == PreRun) {

        // input checking
        // .ORIG not set yet
        if (pc == 0xFFFF && !instruction.empty() && instruction != ".ORIG") {
            throw "ERROR: .ORIG must be the first instruction >> " + line;
        }
        if (pc != 0xFFFF && instruction == ".ORIG") {
            throw "ERROR: .ORIG cannot appear twice >> " + line;
        }

        string e = checkInput(instruction, firstReg, secondReg, thirdReg, opNumber, opLabel, opString, nOfRegs,
                              nOrL);
        if (!e.empty()) {
            throw  e + " >> " + line;
        }
    }

    // .END
    if (instruction == ".END") {
        return pc;
    }

    // .ORIG
    if (instruction == ".ORIG") {
        pc = (uint16_t) parseNumber(opNumber);
        cout << "Origin set to x" << hex << pc << endl;
        if (runType == MainRun) {
            writeWord(oStream, pc);
        }

    }

    // set label
    if (runType == PreRun && !label.empty()) {
        labelDict[label] = pc;
    }

    // if no op -> skip
    if (!instruction.empty()) {
        pc = writeInstruction(runType, instruction, firstReg, secondReg, thirdReg, opNumber, opLabel, opString,
                              nOfRegs, nOrL, oStream, pc);
    }

    return pc;
}


uint16_t Assembler::writeInstruction(RunType runType, string &instruction, string &firstReg, string &secondReg, string &thirdReg,
                          string &opNumber, string &opLabel, string &opString,
                          int nOfRegs, NumOrLabel nOrL,
                          ofstream &oStream, uint16_t pc) {

    // .FILL
    if (instruction == ".FILL") {
        if (runType == MainRun) {
            writeFill(oStream, opNumber, opLabel);
        }
        pc += 1;
    }

        // .BLKW
    else if (instruction == ".BLKW") {
        uint16_t size = (uint16_t) parseNumber(opNumber);
        if (runType == MainRun) {
            for (int i = 0; i < size; i++) {
                writeWord(oStream, 0);
            }
        }
        pc += size;
    }

        // .STRINGZ
    else if (instruction == ".STRINGZ") {
        string unescaped = unescapeString(opString);
        auto strLen = (uint16_t) unescaped.length();
        if (runType == MainRun) {
            for (int i = 0; i < strLen; i++) {
                char c = unescaped[i];
                writeWord(oStream, (uint16_t) c);
            }
            writeWord(oStream, 0);
        }
        pc += strLen + 1;
    }

        // main instructions
    else if (instruction != ".ORIG") {

        pc += 1;

        if (runType == MainRun) {
            // everything left but .ORIG goes here
            uint16_t op = OpCodeForInstruction(instruction);
            op |= getConstantBits(instruction, nOfRegs, nOrL);
            op |= getRegisters(instruction, nOfRegs, firstReg, secondReg, thirdReg);
            op |= getNumberOrOffset(instruction, nOrL, opNumber, opLabel, pc);

            writeWord(oStream, op);
        }
    }
    return pc;
}

void Assembler::writeFill(ofstream &oStream, const string &opNumber, const string &opLabel) {
    if (!opNumber.empty()) {
        double value = parseNumber(opNumber);
        if (value < 0) {
            writeWord(oStream, static_cast<uint16_t>((int16_t) value));
        } else {
            writeWord(oStream, (uint16_t) value);
        }
    } else {
        // label
        uint16_t labelPos = labelDict[opLabel];
        writeWord(oStream, labelPos);
    }
}


uint16_t Assembler::getRegisters(string &instruction, int nOfRegs, string &firstReg, string &secondReg, string &thirdReg) {
    uint16_t op = 0;

    uint16_t regOne;
    uint16_t regTwo;
    uint16_t regThree;
    switch (nOfRegs) {
        case 3:
            regThree = (uint16_t) stoi(thirdReg);
            op |= regThree;
        case 2:
            regTwo = (uint16_t) stoi(secondReg);
            regTwo <<= 6;
            op |= regTwo;
        case 1:
            regOne = (uint16_t) stoi(firstReg);
            if (instruction == "JMP" || instruction == "JSRR") {
                regOne <<= 6;
            } else {
                regOne <<= 9;
            }
            op |= regOne;
            break;
        case 0:
            break;
        default:
            throw "ERROR: invalid number of registers";
    }

    return op;
}

// returns null for no error, else error string
string Assembler::checkInput(const string &instruction, const string &firstReg, const string &secondReg, const string &thirdReg,
                  const string &opNumber, const string &opLabel, const string &opString, int nOfRegs, NumOrLabel nOrL) {
    // no op but inputs
    if (instruction.empty() &&
        !(firstReg.empty() || secondReg.empty() || thirdReg.empty() || opNumber.empty() ||
          opLabel.empty())) {
        return "ERROR: Missing op";
    }

    // check number of registers
    switch (nOfRegs) {
        case 0:
            if (!firstReg.empty() || !secondReg.empty() || !thirdReg.empty()) {
                return instruction + " needs " + to_string(nOfRegs) + " registers";
            }
            break;
        case 1:
            if (firstReg.empty() || !secondReg.empty() || !thirdReg.empty()) {
                return instruction + " needs " + to_string(nOfRegs) + " registers";
            }
            break;
        case 2:
            if (firstReg.empty() || secondReg.empty() || !thirdReg.empty()) {
                return instruction + " needs " + to_string(nOfRegs) + " registers";
            }
            break;
        case 3:
            if (firstReg.empty() || secondReg.empty() || thirdReg.empty()) {
                return instruction + " needs " + to_string(nOfRegs) + " registers";
            }
            break;
        default:
            return "ERROR: Invalid number of registers";
    }

    // check number or label
    if (nOrL == stringOnly) {
        if (opString.empty()) {
            return instruction + " needs a string argument";
        }
        if (!firstReg.empty() || !secondReg.empty() || !thirdReg.empty() || !opLabel.empty() ||
            !opNumber.empty()) {
            return instruction + " should have only a string argument";
        }

    } else {
        if (!opString.empty()) {
            return instruction + " should not have a string argument";
        }


        switch (nOrL) {
            case none:
                if (!opLabel.empty() || !opNumber.empty()) {
                    return instruction + " should have no label and no number";
                }
                break;
            case numberOnly:
                if (!opLabel.empty() || opNumber.empty()) {
                    return instruction + " should have no label but a number";
                }
                break;
            case labelOnly:
                if (opLabel.empty() || !opNumber.empty()) {
                    return instruction + " should have a label but no number";
                }
                break;
            case numberOrLabel:
                if (opLabel.empty() == opNumber.empty()) {
                    return instruction + " should have either label or number";
                }
                break;
            default:
                return "Default should never be reached";
        }
    }

    return string();
}

uint16_t Assembler::getNumberOrOffset(const string &instruction, NumOrLabel nOrL, const string &opNumber, const string &opLabel,
                           uint16_t pc) {
    uint16_t op = 0;
    if (nOrL == numberOnly) {
        int16_t num = (int16_t) parseNumber(opNumber);
        if (instruction == "TRAP") {
            // trap8
            if (num > 0x00FF) {
                throw "ERROR: trap vector exceeds 8 bit";
            }
            op |= num;
        } else if (instruction == "STR" || instruction == "LDR") {
            // offset6
            if (num > 0x001F || (~num + 1) > 0x0020) {
                // boundary check for offset6
                throw "ERROR: offset6 out of bounds";
            }
            op |= (num & 0x003F);
        } else {
            if (instruction == "SUB") {
                num = -num; // flip sign since we add the negative to subtract...
            }
            // imm5
            if (num > 0x000F || (~num + 1) > 0x0010) {
                // boundary check for imm5
                throw "ERROR: imm5 out of bounds";
            }
            op |= (num & 0x001F);
        }
    } else if (nOrL == numberOrLabel) {
        int16_t offset = 0;
        if (!opNumber.empty()) {
            offset = (int16_t) parseNumber(opNumber);
        } else {
            try {
                uint16_t labelAddr = labelDict[opLabel];
                if (labelAddr == 0)
                    throw "Empty label exception";
                offset = labelAddr - pc;
            } catch (...) {
                throw "ERROR: label '" + opLabel + "' not found";
            }
        }

        if (instruction == "JSR") {
            // PCoffset11
            if (offset > 0x03FF || (~offset + 1) > 0x0400) {
                // boundary check for PCoffset11
                throw "ERROR: PCoffset11 out of bounds";
            }
            op |= (offset & 0x07FF);
        } else {
            // PCoffset9
            if (offset > 0x00FF || (~offset + 1) > 0x0100) {
                // boundary check for PCoffset9
                throw "ERROR: PCoffset9 out of bounds (" + to_string(offset) + ")";
            } else {
                op |= (offset & 0x01FF);
            }
        }
    } else if (nOrL == labelOnly) {
        throw "ERROR: labelOnly should never happen";
    }

    return op;
}

uint16_t Assembler::getConstantBits(const string &instruction, int nOfRegs, NumOrLabel nOrL) {
    uint16_t op = 0;
    switch (nOfRegs) {
        case 0:
            if (instruction == "RET") {
                op |= 0x01C0;
            } else if (instruction == "JSR") {
                op |= 0x0800;
            } else if (instruction.compare(0, 2, "BR") == 0) {
                string modeFlags = instruction.substr(2, instruction.length() - 2);
                sort(modeFlags.begin(), modeFlags.end());
                if (modeFlags.length() == 0) {
                    op |= 0x0E00;
                } else if (modeFlags.length() >= 1) {
                    if (modeFlags[0] == 'N') {
                        op |= 0x0800;
                    }
                    if (modeFlags[0] == 'Z') {
                        op |= 0x0400;
                    }
                    if (modeFlags[0] == 'P') {
                        op |= 0x0200;
                    }
                }
                if (modeFlags.length() >= 2) {
                    if (modeFlags[1] == 'N') {
                        throw "ERROR: duplicate flag";
                    }
                    if (modeFlags[1] == 'Z') {
                        op |= 0x0400;
                    }
                    if (modeFlags[1] == 'P') {
                        op |= 0x0200;
                    }
                }
                if (modeFlags.length() == 3) {
                    if (modeFlags[2] == 'N') {
                        throw "ERROR: duplicate flag";
                    }
                    if (modeFlags[2] == 'Z') {
                        op |= 0x0400;
                    }
                    if (modeFlags[2] == 'P') {
                        throw "ERROR: duplicate flag";
                    }
                }
            }
            break;
        case 2:
            if (instruction == "NOT") {
                op |= 0x007F;
            } else if (instruction == "AND") {
                if (nOrL != none) {
                    // add, and, mul with number not register
                    op |= 0x0020;
                }
            } else {
                if (nOrL != none &&
                    (instruction == "ADD" ||
                     instruction == "MUL")) {
                    // add, and, mul with number not register
                    op |= 0x0020;
                }
            }
            break;
        case 3:
            if (instruction == "SUB") {
                op |= 0x0010; // bit 4 set makes an add on 3 registers a sub
            }
        default:
            break;
    }
    return op;
}

uint16_t Assembler::OpCodeForInstruction(string &op) {

    if (op == "ADD") {
        return addOpCode;
    }
    if (op == "SUB") {
        return addOpCode;
    }
    if (op == "AND") {
        return andOpCode;
    }
    if (op.compare(0, 2, "BR") == 0) {
        return brOpCode;
    }
    if (op == "JMP") {
        return jmpOpCode;
    }
    if (op == "JSR") {
        return jsrOpCode;
    }
    if (op == "JSRR") {
        return jsrrOpCode;
    }
    if (op == "LD") {
        return ldOpCode;
    }
    if (op == "LDI") {
        return ldiOpCode;
    }
    if (op == "LDR") {
        return ldrOpCode;
    }
    if (op == "LEA") {
        return leaOpCode;
    }
    if (op == "MUL") {
        return mulOpCode;
    }
    if (op == "NOT") {
        return notOpCode;
    }
    if (op == "RET") {
        return retOpCode;
    }
    if (op == "RTI") {
        return rtiOpCode;
    }
    if (op == "ST") {
        return stOpCode;
    }
    if (op == "STI") {
        return stiOpCode;
    }
    if (op == "STR") {
        return strOpCode;
    }
    if (op == "TRAP") {
        return trapOpCode;
    }
    if (op == "PUTS") {
        return putsTrap;
    }
    if (op == "GETC") {
        return getcTrap;
    }
    if (op == "OUT") {
        return outTrap;
    }
    if (op == "IN") {
        return inTrap;
    }
    if (op == "PUTSP") {
        return putspTrap;
    }
    if (op == "HALT") {
        return haltTrap;
    }

    throw "NOT AN OP: " + op;
    return 0xFFFF;
}

void Assembler::writeWord(ostream &stream, uint16_t value) {

    auto *p = reinterpret_cast<char *>(&value);

    // we want big endian
    stream.write(&p[1], sizeof(uint8_t));
    stream.write(&p[0], sizeof(uint8_t));
}

void Assembler::determineArgumentsOfOp(const string &instruction, bool thirdRegEmpty, int &nOfRegs, NumOrLabel &nOrL) {
    // no args at all
    if (instruction == "RET" || instruction == "RTI" || instruction == ".END" || instruction == "PUTS" || instruction == "GETC" ||
        instruction == "OUT" || instruction == "IN" || instruction == "PUTSP" || instruction == "HALT") {
        nOfRegs = 0;
        nOrL = none;
    }

        // one register instructions
    else if (instruction == "JMP" || instruction == "JSRR") {
        nOfRegs = 1;
        nOrL = none;
    }

        // one number instruction
    else if (instruction == "TRAP" || instruction == ".ORIG" || instruction == ".BLKW") {
        nOfRegs = 0;
        nOrL = numberOnly;
    }

        // num or label instructions
    else if (instruction.compare(0, 2, "BR") == 0 || instruction == "JSR" || instruction == ".FILL") {
        nOfRegs = 0;
        nOrL = numberOrLabel;
    }

        // two register instructions
    else if (instruction == "NOT") {
        nOfRegs = 2;
        nOrL = none;
    }

        // one register plus number or label
    else if (instruction == "ST" || instruction == "STI" || instruction == "LEA" || instruction == "LD" || instruction == "LDI") {
        nOfRegs = 1;
        nOrL = numberOrLabel;
    }

        // two reg + num or label
    else if (instruction == "LDR" || instruction == "STR") {
        nOfRegs = 2;
        nOrL = numberOnly;
    }

        // two + num or three
    else if (instruction == "ADD" || instruction == "AND" || instruction == "MUL") {
        if (thirdRegEmpty) {
            nOfRegs = 2;
            nOrL = numberOnly;
        } else {
            nOfRegs = 3;
            nOrL = none;
        }
    }

        // string only
    else if (instruction == ".STRINGZ") {
        nOfRegs = 0;
        nOrL = stringOnly;
    }
}

// might throw exception
double Assembler::parseNumber(string num) {

    switch (num[0]) {
        case '#':
            return stod((string) num.substr(1, num.length() - 1), (size_t *) nullptr);
        case 'b':
            return stoul(num.substr(1, num.length() - 1), nullptr, 2);
        case 'o':
            return stoul(num.substr(1, num.length() - 1), nullptr, 8);
        case 'x':
            return stoul(num.substr(1, num.length() - 1), nullptr, 16);
        default:
            return stod(num, nullptr);
    }
}


string Assembler::unescapeString(string unescaped) {
    string original = unescaped;
    unescaped = regex_replace(unescaped, regex(R"((^|[^\\])(\\a))"), "$1\a");
    unescaped = regex_replace(unescaped, regex(R"((^|[^\\])(\\b))"), "$1\b");
    unescaped = regex_replace(unescaped, regex(R"((^|[^\\])(\\f))"), "$1\f");
    unescaped = regex_replace(unescaped, regex(R"((^|[^\\])(\\n))"), "$1\n");
    unescaped = regex_replace(unescaped, regex(R"((^|[^\\])(\\r))"), "$1\r");
    unescaped = regex_replace(unescaped, regex(R"((^|[^\\])(\\t))"), "$1\t");
    unescaped = regex_replace(unescaped, regex(R"((^|[^\\])(\\v))"), "$1\v");
    unescaped = regex_replace(unescaped, regex(R"((^|[^\\])(\\'))"), "$1\'");
    unescaped = regex_replace(unescaped, regex(R"((^|[^\\])(\\\"))"), "$1\"");
    unescaped = regex_replace(unescaped, regex(R"((^|[^\\])(\\\?))"), "$1\?");

    // octal and hex
    smatch match;
    while (regex_search(unescaped, match, regex(R"((^|[^\\])\\([0-7]{1,3}))"))) {
        char octa = (char) stoi(match[2], nullptr, 8);
        string r = R"((^|[^\\])(\\)" + (string) match[2] + ")";
        string x = (string) match[1] + octa;
        unescaped = regex_replace(unescaped, regex(r), x);
    }
    while (regex_search(unescaped, match, regex(R"((^|[^\\])\\x([0-9A-Fa-f]{2}))"))) {
        char hexa = (char) stoi(match[2], nullptr, 16);
        string r = R"((^|[^\\])(\\x)" + (string) match[2] + ")";
        string x = (string) match[1] + hexa;
        unescaped = regex_replace(unescaped, regex(r), x);
    }

    // check if there are any escapes left
    if (regex_search(unescaped, regex(R"((?:^|[^\\])(?:\\\\)*\\(?=[^\\]|$))"))) {
        cerr << "ERROR: invalid escape sequence: " << original << endl;
        return "";
    }

    unescaped = regex_replace(unescaped, regex(R"(\\\\)"), "\\");
    return unescaped;
}
