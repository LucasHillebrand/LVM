#ifndef instructionset_h
#define instructionset_h

#define CMD_ADD 1 // adds two numbers
#define CMD_SUB 2 // subtracts two numbers

#define CMD_STORE 3 // stores curr val in mem
#define CMD_LOAD 4 // loads value from mem
#define CMD_SMP 5 // set mem ptr

#define CMD_JUMP 6 // jumps to an specific instruction
#define CMD_JIZ 7 // jump if zero
#define CMD_JNZ 8 // jump if not zero

#define CMD_MOV 9 // move curr to register
#define CMD_PSH 10 // push register to curr

#define CMD_DB 11 // define byte
#define CMD_PV 12 // define register
#define CMD_PI 13 // push pc/index

#define CMD_SPM 14 // set pinmode
#define CMD_SPP 15 // set pin ptr
#define CMD_RPV 16 // read pin value
#define CMD_WPV 17 // write pin value

#define CMD_NTH 18 // new thread
#define CMD_STH 19 // stop thread

#define CMD_HLT 255 // halt machine

#endif