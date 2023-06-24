#include "../vmbase.h"
#include "../math.h"
#include "../stack.h"
#include "../instructionset.h"

void byteClone(byte org[8], byte size, byte res[8]){
    for (byte i=0;i<8 && i < size;i++){
        if (i<size){
            res[i]=org[i];
        } else{
            res[i]=0;
        }
    }
}

SOC VM_ADD(SOC self, byte thread, byte args[3]){
    byte values[2][8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, values[0]);
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, values[1]);
    byteAdd(values[0], values[1], values[0]);
    self.cpu.threads[thread].stack = push(self.cpu.threads[thread].stack, values[0]);
    return self;
}

SOC VM_SUB(SOC self, byte thread, byte args[3]){
    byte values[2][8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, values[0]);
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, values[1]);
    byteSub(values[1], values[0], values[1]);
    self.cpu.threads[thread].stack = push(self.cpu.threads[thread].stack, values[1]);
    return self;
}

SOC VM_STR(SOC self, byte thread, byte args[3]){
    u64 memptr = self.cpu.threads[thread].memptr;
    byte value[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, value);
    for (u64 i = memptr; i < memptr+args[0] && i < memptr+8;i++){
        self.mem[i]=value[i-memptr];
    }
    return self;
}

SOC VM_LD(SOC self, byte thread, byte args[3]){
    u64 memptr = self.cpu.threads[thread].memptr;
    byte data[8] = {0,0,0,0,0,0,0,0};
    for (u64 i = memptr;i<memptr+8 && i < memptr+args[0];i++){
        data[i-memptr]=self.mem[i];
    }
    self.cpu.threads[thread].stack = push(self.cpu.threads[thread].stack, data);
    return self;
}

SOC VM_JMP(SOC self, byte thread, byte args[3]){
    byte val[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, val);
    self.cpu.threads[thread].pc = toInt(val);
    return self;
}

SOC VM_JIZ(SOC self, byte thread, byte args[3]){
    byte dst[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, dst);
    byte test[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, test);
    if (toInt(test) == 0){
        self.cpu.threads[thread].pc = toInt(dst);
    }
    return self;
}

SOC VM_JNZ(SOC self, byte thread, byte args[3]){
    byte dst[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, dst);
    byte test[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, test);
    if (toInt(test) != 0){
        self.cpu.threads[thread].pc = toInt(dst);
    }
    return self;
}

SOC VM_SMP(SOC self, byte thread, byte args[3]){
    byte val[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, val);
    self.cpu.threads[thread].memptr = toInt(val);
    return self;
}

SOC VM_MOV(SOC self, byte thread, byte args[3]){
    byte val[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, val);
    byteClone(val, 8, self.cpu.threads[thread].reg[args[0]]);
    return self;
}

SOC VM_PSH(SOC self, byte thread, byte args[3]){
    self.cpu.threads[thread].stack = push(self.cpu.threads[thread].stack, self.cpu.threads[thread].reg[args[0]]);
    return self;
}

SOC VM_DB(SOC self, byte thread, byte args[3]){return self;}
SOC VM_PV(SOC self, byte thread, byte args[3]){
    u64 index = self.cpu.threads[thread].pc + 4;
    byte value[8] = {0};
    for (u64 i=index;i<index+8;i++){
        value[i-index]=self.mem[i];
    }
    self.cpu.threads[thread].stack = push(self.cpu.threads[thread].stack, value);
    return self;
}

SOC VM_SPM(SOC self, byte thread, byte args[3]){
    byte val[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, val);
    self.cpu.pins[self.cpu.threads[thread].pinptr].mode = val[0];
    return self;
}

SOC VM_SPP(SOC self, byte thread, byte args[3]){
    byte val[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, val);
    self.cpu.threads[thread].pinptr = toInt(val);
    return self;
}

SOC VM_RPV(SOC self, byte thread, byte args[3]){
    byte out[8] = {0,0,0,0,0,0,0,0};
    if (self.cpu.pins[self.cpu.threads[thread].pinptr].mode == 0){
        out[0] = self.cpu.pins[self.cpu.threads[thread].pinptr].val;
    }
    self.cpu.threads[thread].stack = push(self.cpu.threads[thread].stack, out);
    return self;
}

SOC VM_WPV(SOC self, byte thread, byte args[3]){
    if (self.cpu.pins[self.cpu.threads[thread].pinptr].mode == 1){
        byte val[8];
        self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, val);
        self.cpu.pins[self.cpu.threads[thread].pinptr].val=val[0];
    }
    return self;
}

SOC VM_NTH(SOC self, byte thread, byte args[3]){
    byte threadn[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, threadn);
    byte pc[8];
    self.cpu.threads[thread].stack = pop(self.cpu.threads[thread].stack, pc);
    self.cpu.threads[threadn[0]].init=1;
    self.cpu.threads[threadn[0]].pc=toInt(pc);
    return self;
}

SOC VM_STH(SOC self, byte thread, byte args[3]){
    byte threadn[8];
    self.cpu.threads[thread].stack=pop(self.cpu.threads[thread].stack, threadn);
    self.cpu.threads[threadn[0]].init=0;
    return self;
}
SOC VM_HLT(SOC self, byte thread, byte args[3]){
    self.cpu.active=0;
    for (byte i=0;i<8;i++){
        self.cpu.threads[i].init=0;
    }
    return self;
}

SOC VM_PI(SOC self, byte thread, byte args[3]){
    byte val[8] = {0};
    toBytes(self.cpu.threads[thread].pc, val);
    self.cpu.threads[thread].stack = push(self.cpu.threads[thread].stack, val);
    return self;
}

void printVM(SOC self, byte extended){
    if (self.memsize>0 && extended){
        printf("[ ");
        for (u64 i = 0;i<self.memsize-1;i++){
            printf("%u, ",self.mem[i]);
        }
        printf("%u ]\n\n", self.mem[self.memsize-1]);
    }
    for (byte i=0;i<8;i++){
        printf("\n --- thread: %u --- \n\n", i);
        printf("init: %u\n",self.cpu.threads[i].init);
        printf("index: %lu\n",self.cpu.threads[i].pc);
        printf("memptr: %lu\n", self.cpu.threads[i].memptr);
        printf("pinptr: %u\n\n",self.cpu.threads[i].pinptr);

        printf("stack: [top(%u)]\n",self.cpu.threads[i].stack.top);
        for (byte j=0;j<8;j++){
            printf("%u: ",j);
            printarr(self.cpu.threads[i].stack.mem[j]);
        }
        printf("\nRegister:\n");
        for (byte j=0;j<8;j++){
            printf("%u: ",j);
            printarr(self.cpu.threads[i].reg[j]);
        }
    }
}

SOC createVM(u64 memsize){
    SOC out;
    out.memsize=memsize;
    out.mem=malloc(memsize);
    out.cpu.active=1;

    for (byte i=0;i<8;i++){
        out.cpu.threads[i].init=0;
        out.cpu.threads[i].memptr=0;
        out.cpu.threads[i].pc=0;
        out.cpu.threads[i].pinptr=0;
        for (byte j = 0;j<8;j++){
            out.cpu.threads[i].stack.top=0;
            byteClone((byte[8]){0,0,0,0,0,0,0,0}, 8, out.cpu.threads[i].stack.mem[j]);
            byteClone((byte[8]){0,0,0,0,0,0,0,0}, 8, out.cpu.threads[i].reg[j]);
        }
    }

    for (uint i = 0; i<256;i++){
        out.cpu.pins[i].mode=0;
        out.cpu.pins[i].val=0;
    }

    return out;
}

void cleanVM(SOC self){
    free(self.mem);
}

SOC exec(SOC self, byte thread){
    u64 index = self.cpu.threads[thread].pc;
    u64 org = self.cpu.threads[thread].pc;
    byte args[4];
    for (byte i=0;i<4;i++){
        args[i]=self.mem[index+i];
    }

    byte sysargs[3];
    for (byte i = 1;i<4;i++){
        sysargs[i-1]=args[i];
    }
    index+=4;
    switch (args[0]) {
    case CMD_ADD:
        self = VM_ADD(self, thread, sysargs);
        break;
    case CMD_SUB:
        self = VM_SUB(self, thread, sysargs);
        break;
    case CMD_STORE:
        self = VM_STR(self, thread, sysargs);
        break;
    case CMD_LOAD:
        self = VM_LD(self, thread, sysargs);
        break;
    case CMD_JUMP:
        self = VM_JMP(self, thread, sysargs);
        break;
    case CMD_JIZ:
        self = VM_JIZ(self, thread, sysargs);
        break;
    case CMD_JNZ:
        self = VM_JNZ(self, thread, sysargs);
        break;
    case CMD_SMP:
        self = VM_SMP(self, thread, sysargs);
        break;
    case CMD_MOV:
        self = VM_MOV(self, thread, sysargs);
        break;
    case CMD_PSH:
        self = VM_PSH(self, thread, sysargs);
        break;
    case CMD_DB:
        self = VM_DB(self, thread, sysargs);
        index+=sysargs[0];
        break;
    case CMD_PV:
        self = VM_PV(self, thread, sysargs);
        index+=8;
        break;
    case CMD_SPM:
        self = VM_SPM(self, thread, sysargs);
        break;
    case CMD_SPP:
        self = VM_SPP(self, thread, sysargs);
        break;
    case CMD_RPV:
        self = VM_RPV(self, thread, sysargs);
        break;
    case CMD_WPV:
        self = VM_WPV(self, thread, sysargs);
        break;
    case CMD_NTH:
        self = VM_NTH(self, thread, sysargs);
        break;
    case CMD_STH:
        self = VM_STH(self, thread, sysargs);
        break;
    case CMD_HLT:
        self = VM_HLT(self, thread, sysargs);
        break;
    case CMD_PI:
        self = VM_PI(self, thread, sysargs);
        break;
    }
        
    if (self.cpu.threads[thread].pc == org){
        self.cpu.threads[thread].pc=index;
    }
    return self;
}