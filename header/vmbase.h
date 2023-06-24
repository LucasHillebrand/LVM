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
SOC VM_PV(SOC self, byte thread, byte args[3]);
SOC VM_SPM(SOC self, byte thread, byte args[3]);
SOC VM_SPP(SOC self, byte thread, byte args[3]);
SOC VM_RPV(SOC self, byte thread, byte args[3]);
SOC VM_WPV(SOC self, byte thread, byte args[3]);
SOC VM_HLT(SOC self, byte thread, byte args[3]);
SOC VM_NTH(SOC self, byte thread, byte args[3]);
SOC VM_STH(SOC self, byte thread, byte args[3]);
SOC VM_PI(SOC self, byte thread, byte args[3]);

void printVM(SOC self, byte extended);
SOC createVM(u64 memsize);
void cleanVM(SOC self);

SOC exec(SOC self, byte thread);
#endif