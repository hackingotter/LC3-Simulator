#ifndef OPCODES_H
#define OPCODES_H

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

#define opMask 0xF000

#endif // OPCODES_H
