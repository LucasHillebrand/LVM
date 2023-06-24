#include "header/default.h"
#include "header/file.h"
#include "header/asambler.h"
#include "header/math.h"
#include "header/string.h"
#include <stdio.h>

void clear(char str[1024]){
    for (uint i=0;i<1024;i++){
        str[i]=0;
    }
}

void compile(char dstname[1024],char src[1024][1024], uint srcsize){
    //reserve 16 bytes
    FILE* dst=fopen(dstname, "w");
    for (uint i=0;i<16;i++){
        fputc(0, dst);
    }

    sysvar* vars=vinit(1);
    
    vars->vpc+=16; // bytes are reseved for the jump to the main func

    // compile every given file
    for(uint i=0;i<srcsize;i++){
        char* curr=(char*)src[i];
        FILE* src=fopen(curr, "r");
        u64 lc=getLineCount(src);
        rewind(src);
        for (uint j=0;j<lc;j++){
            vars->vpc+=parse(dst, lex(nextLine(src),vars));
        }
        vrewind(vars);
        
        fclose(src);
    }

    // begin jump to main public variable if the var is there
    char main[1024]={0};
    strcopy("main", main, 1024);
    if (search(vars, pub, main).found) {
        byte buff[8]={0};
        vsearch(vars, pub, main, buff);
        rewind(dst);
        fputc(CMD_PV, dst);
        for (uint i=0;i<3;i++){
            fputc(0, dst);
        }
        for (uint i=0;i<8;i++){
            fputc(buff[i], dst);
        }
        fputc(CMD_JUMP, dst);
    }
    //end
    fclose(dst);
}
void fuse(char dstname[1024],char src[1024][1024], uint srcsize){
    FILE *dst=fopen(dstname, "w");
    char* defbeg = "\n// file >> \"";
    u64 defbegsize = strlength(defbeg);
    char* defend = "\"\n\n";
    u64 defendsize = strlength(defend);
    for (uint i=0;i<srcsize;i++){
        char* curr=(char*)src[i];
        FILE* src=fopen(curr, "r");
        for (uint j=0;j<defbegsize;j++){
            fputc(defbeg[j], dst);
        }
        for (uint j=0;j<strlength(curr);j++){
            fputc(curr[j], dst);
        }
        for (uint j=0;j<defendsize;j++){
            fputc(defend[j], dst);
        }

        u64 filesize=getFileSize(src);
        rewind(src);
        for (u64 j=0;j<filesize;j++){
            fputc(fgetc(src),dst);
        }
        fclose(src);
    }
    fclose(dst);
}

int main(int argc, char** argv){
    char srcnames[1024][1024]={0};
    char dstname[1024]={0};
    strcopy("main.vbin", dstname, 1024);
    byte mode; // 0=compile, 1=fuse together
    
    uint srccurr=0;
    for (uint i=1;i<argc;i++){
        if (strcomp(argv[i], "-f")){
            clear(dstname);
            strcopy("main.lasm", dstname, 1024);
            mode=1;
        }else if(strcomp(argv[i], "-o")){
            clear(dstname);
            i++;
            strcopy(argv[i], dstname, 1024);
        }else if (strcomp(argv[i], "--help")){
            printf("modes:\n-f < fuse files together\n-o < define output filename, default = main.vbin(for compile), main.lasm(for fuse)\n");
            mode=2;
        }else{
            if (srccurr<1024){
                strcopy(argv[i], srcnames[srccurr], 1024);
                srccurr++;
            }
        }
    }

    switch (mode) {
    case 0:
        compile(dstname, srcnames, srccurr);
        break;
    case 1:
        fuse(dstname, srcnames, srccurr);
        break;
    }
}