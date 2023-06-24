#ifndef file_h
#define file_h

#include "./default.h"

typedef struct{
    char string[1024];
    uint pos;
    uint size;
    char word[1024];
}LINE;

u64 wpl(LINE* line);
void nextWord(LINE* line);
void lrewind(LINE* line);

char* getFILE(FILE* file);
LINE* nextLine(FILE* file);
u64 getFileSize(FILE* file);
u64 getLineCount(FILE* file);

#endif