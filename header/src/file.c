#include "../file.h"
#include <stdio.h>
#include <stdlib.h>

LINE* nextLine(FILE* file){
    LINE* out=malloc(sizeof(LINE));
    u64 i = 0;
    for (int curr=fgetc(file); curr != '\n' && curr!=EOF;i++,curr=fgetc(file)){
        out->string[i]=(char)curr;
    }
    out->string[i]=0;
    out->pos=0;
    out->size=i+1;
    return out;
}

u64 wpl(LINE* line){
    u64 out=0;
    for(uint i=0;i<line->size;i++){
        if(line->string[i] == ' '){out++;}
    }
    return out+1;
}

void nextWord(LINE* line){
    uint i = 0;
    for (i=0;i<1024&&line->string[line->pos]!=' '&&line->string[line->pos]!=0;i++,line->pos++){
        line->word[i]=line->string[line->pos];
    }
    line->word[i]=0;
    line->pos++;
}

char* getFILE(FILE* file){
    u64 fsize=getFileSize(file);
    rewind(file);
    char* out = malloc(fsize);
    u64 i = 0;
    for (i=0;i<fsize;i++){
        out[i]=fgetc(file);
    }
    out[i]=0;
    return out;
}
u64 getFileSize(FILE* file){
    u64 out=0;
    for (int curr=fgetc(file);curr!=EOF;out++,curr=fgetc(file)){}
    return out;
}
u64 getLineCount(FILE* file){
    u64 out=1;
    for (int curr=fgetc(file);curr!=EOF;curr=fgetc(file)){
        if(curr == '\n'){out++;}
    }
    return out;
}
void lrewind(LINE* line){line->pos=0;}