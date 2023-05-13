#ifndef vmstruct_h
#define vmstruct_h

#include "./default.h"
#include "./stack.h"

typedef struct SOC {
    byte* mem;
    u64 memsize;

    struct {
        struct {
            stack stack;
            byte reg[8][8];
            byte init;
            u64 pc;
            u64 memptr;
            byte pinptr;
        }threads[8];
        
        struct {
            byte val;
            byte mode; // 0 = read, 1 = write
        }pins[256];

        byte active; // 0 = false, 1 = true
    }cpu;
} SOC ;


#define CMD_ADD 1 // adds two numbers
#define CMD_SUB 2 // subtracts two numbers

#define CMD_STORE 3 // stores curr val in mem
#define CMD_LOAD 4 // loads value from mem

#define CMD_JUMP 5 // jumps to an specific instruction
#define CMD_JIZ 6 // jump if zero
#define CMD_JNZ 7 // jump if not zero

#define CMD_SMP 8 // set mem ptr

#define CMD_MOV 9 // move curr to register
#define CMD_PSH 10 // push register to curr

#define CMD_DB 11 // define byte
#define CMD_DR 12 // define register

#define CMD_SPM 13 // set pinmode
#define CMD_SPP 14 // set pin ptr
#define CMD_RPV 15 // read pin value
#define CMD_WPV 16 // write pin value

#define CMD_NTH 17 // new thread
#define CMD_STH 18 // stop thread
#define CMD_HLT 19 // halt machine

SOC VM_ADD(SOC self, byte thread, byte args[3]);
SOC VM_SUB(SOC self, byte thread, byte args[3]);
SOC VM_STR(SOC self, byte thread, byte args[3]);
SOC VM_LD(SOC self, byte thread, byte args[3]);
SOC VM_JMP(SOC self, byte thread, byte args[3]);
SOC VM_JIZ(SOC self, byte thread, byte args[3]);
SOC VM_JNZ(SOC self, byte thread, byte args[3]);
SOC VM_SMP(SOC self, byte thread, byte args[3]);
SOC VM_MOV(SOC self, byte thread, byte args[3]);
SOC VM_PSH(SOC self, byte thread, byte args[3]);
SOC VM_DB(SOC self, byte thread, byte args[3]);
SOC VM_DR(SOC self, byte thread, byte args[3]);
SOC VM_SPM(SOC self, byte thread, byte args[3]);
SOC VM_SPP(SOC self, byte thread, byte args[3]);
SOC VM_RPV(SOC self, byte thread, byte args[3]);
SOC VM_WPV(SOC self, byte thread, byte args[3]);
SOC VM_HLT(SOC self, byte thread, byte args[3]);
SOC VM_NTH(SOC self, byte thread, byte args[3]);
SOC VM_STH(SOC self, byte thread, byte args[3]);

void printVM(SOC self);
SOC createVM(u64 memsize);
void cleanVM(SOC self);

SOC exec(SOC self, byte thread);
#endif