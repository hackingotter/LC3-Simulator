.orig x3000
ADD R1,R5,#-1
ADD R0, R3,R4
AND R1,R2,#4
AND R4,R5,R6
BRnzp #5
JMP R6
JSR x03
JSRR R2
LD R7, #-5
LDI R0, 2
LDR R1,R7,x14
LEA R5,34
NOT R1,R7
RET
ST R4,56
STI R7,54
STR R1,R2,x33
