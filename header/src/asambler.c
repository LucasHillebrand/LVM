#include "../asambler.h"
#include <stdio.h>
#include <stdlib.h>

sysvar* vinit(uint basesize){
    sysvar* out = malloc(sizeof(sysvar));
    for (uint i = 0;i<2;i++){
        out->values[i].names=malloc((u64)64*basesize);
        out->values[i].value=malloc((u64)8*basesize);
        out->values[i].size=basesize;
        out->values[i].curr=0;
    }
    out->vpc=0;
    return out;
}
void vclr(sysvar* self){
    for (uint i=0;i<2;i++){
        free(self->values[i].names);
        free(self->values[i].value);
    }
    free(self);
}

struct result search(sysvar* self, byte type, char keyword[64]){
    struct result out;
    out.found=0;
    for (u64 i=0;i<self->values[type].curr*64;i+=64){
        if (strcomp(&self->values[type].names[i], keyword)){
            uint curr=(i/64);
            out.found=1;
            out.where=curr;
            i=self->values[type].curr*64;
        }
    }
    return out;
}

void vadd(sysvar* self, byte type /*0=priv, 1=pub*/, char name[64], byte value[8]){
    u64 nameptr=self->values[type].curr*64;
    u64 byteptr=self->values[type].curr*8;
    struct result res = search(self, type, name);
    if(res.found){
        nameptr=res.where*64;
        byteptr=res.where*8;
    }

    for (u64 i=nameptr;i<nameptr+64;i++){
        self->values[type].names[i]=name[i-nameptr];
    }
    for (u64 i=byteptr;i<byteptr+8;i++){
        self->values[type].value[i]=value[i-byteptr];
    }
    if (!res.found){
    self->values[type].curr++;
    }
    if (self->values[type].curr>=self->values[type].size){
        self->values[type].names=realloc(self->values[type].names, (u64)(self->values[type].size*2)*64);
        self->values[type].value=realloc(self->values[type].value, (u64)(self->values[type].size*2)*8);
        self->values[type].size*=2;
    }
}

void vsearch(sysvar* self, byte type /*0=priv, 1=pub*/, char keyword[64], byte buff[8]){
    for (u64 i=0;i<self->values[type].curr*64;i+=64){
        if (strcomp(&self->values[type].names[i], keyword)){
            uint curr=(i/64)*8;
            for (uint i = 0; i<8;i++){
                buff[i]=self->values[type].value[curr+i];
            }
            i=self->values[type].size*64;
        }
    }
}

void vrewind(sysvar* self){self->values[priv].curr=0;}

command lex(LINE* ln, sysvar* vars){
    //printf("command: \"%s\" recognized\n\n",ln->string);
    command out = {"",{0}};
    u64 argc = wpl(ln);
    char args[4][1024];
    u64 iargs[3];
    byte bargs[3][8];
    for (uint i=0;i<argc&&i<4;i++){
        nextWord(ln);
        strcopy(ln->word, args[i], 1024);
    }
    for (uint i=1;i<4;i++){
        if (strcomp(args[i], ".")){
            iargs[i-1]=vars->vpc;
            toBytes(iargs[i-1], bargs[i-1]);
        }else if(args[i][0]=='$'){
            vsearch(vars, priv, &args[i][1], bargs[i-1]);
        }else if(args[i][0]=='#'){
            vsearch(vars, pub, &args[i][1], bargs[i-1]);
        }else{
            iargs[i-1]=strToInt((byte*)args[i]);
            toBytes(iargs[i-1], bargs[i-1]);
        }
    }

    if (strcomp(args[0], "var")&&argc>=3){
        vadd(vars,priv,args[1],bargs[1]);
    }else if (strcomp(args[0], "pub")&&argc>=3){
        vadd(vars,pub,args[1],bargs[1]);
    } else{
        strcopy(args[0], out.instruction, 1024);
        for (int i=0;i<3;i++){
            for (int j=0;j<8;j++){
                out.args[i][j]=bargs[i][j];
            }
        }
    }

    return out;
}

byte parse(FILE* dst, command cmd){
    byte tmparr[12] = {0};
    byte size=0;
    if (strcomp(cmd.instruction, "add")){
        size=4;
        tmparr[0]=CMD_ADD;
    }else if (strcomp(cmd.instruction, "sub")){
        size=4;
        tmparr[0]=CMD_SUB;
    }else if (strcomp(cmd.instruction, "str")){
        size=4;
        tmparr[0]=CMD_STORE;
        tmparr[1]=cmd.args[0][0];
    }else if (strcomp(cmd.instruction, "ld")){
        size=4;
        tmparr[0]=CMD_LOAD;
        tmparr[1]=cmd.args[0][0];
    }else if (strcomp(cmd.instruction, "smp")){
        size=4;
        tmparr[0]=CMD_SMP;
    }else if (strcomp(cmd.instruction, "jmp")){
        size=4;
        tmparr[0]=CMD_JUMP;
    }else if (strcomp(cmd.instruction, "jiz")){
        size=4;
        tmparr[0]=CMD_JIZ;
    }else if (strcomp(cmd.instruction, "jnz")){
        size=4;
        tmparr[0]=CMD_JNZ;
    }else if (strcomp(cmd.instruction, "mov")){
        size=4;
        tmparr[0]=CMD_MOV;
        tmparr[1]=cmd.args[0][0];
    }else if (strcomp(cmd.instruction, "psh")){
        size=4;
        tmparr[0]=CMD_PSH;
        tmparr[1]=cmd.args[0][0];
    }else if (strcomp(cmd.instruction, "pv")){
        size=12;
        tmparr[0]=CMD_PV;
        for (uint i=4;i<12;i++){
            tmparr[i]=cmd.args[0][i-4];
        }
    }else if (strcomp(cmd.instruction, "pi")){
        size=4;
        tmparr[0]=CMD_PI;
    }else if (strcomp(cmd.instruction, "spm")){
        size=4;
        tmparr[0]=CMD_SPM;
    }else if (strcomp(cmd.instruction, "spp")){
        size=4;
        tmparr[0]=CMD_SPP;
    }else if (strcomp(cmd.instruction, "rpv")){
        size=4;
        tmparr[0]=CMD_RPV;
    }else if (strcomp(cmd.instruction, "wpv")){
        size=4;
        tmparr[0]=CMD_WPV;
    }else if (strcomp(cmd.instruction, "nth")){
        size=4;
        tmparr[0]=CMD_NTH;
    }else if (strcomp(cmd.instruction, "sth")){
        size=4;
        tmparr[0]=CMD_STH;
    }else if (strcomp(cmd.instruction, "hlt")){
        size=4;
        tmparr[0]=CMD_HLT;
    }

    for (int i=0;i<size;i++){
        fputc(tmparr[i], dst);
    }

    return size;
}
