#include "../string.h"
#include "../math.h"

byte strcomp(char* first, char* second){
    byte out = 1;

    if (strlength(first) != strlength(second)){
        out = 0;
    }else {
        for (u64 i = 0; i<strlength(first);i++){
            if (first[i] != second[i]){
                out = 0;
            }
        }
    }

    return out;
}

u64 strlength(char* str){
    u64 out = 0;
    byte curr=1;
    while (curr != 0){
        curr = str[out];
        out++;
    }
    return out - 1;
}

u64 strToInt(byte* str){
    u64 out = 0;
    u64 len = strlength((char*)str);
    byte rem = 1;
    for (i64 i = len-1;i>=0;i--){
        switch (str[i]) {
        case '0':
            out+=0;
            break;
        case '1':
            out+=1*power(10, len-i-rem);
            break;
        case '2':
            out+=2*power(10, len-i-rem);
            break;
        case '3':
            out+=3*power(10, len-i-rem);
            break;
        case '4':
            out+=4*power(10, len-i-rem);
            break;
        case '5':
            out+=5*power(10, len-i-rem);
            break;
        case '6':
            out+=6*power(10, len-i-rem);
            break;
        case '7':
            out+=7*power(10, len-i-rem);
            break;
        case '8':
            out+=8*power(10, len-i-rem);
            break;
        case '9':
            out+=9*power(10, len-i-rem);
            break;
        default:
            rem++;
            break;
        }
    }
    return out;
}

void strcopy(char* org, char* res, u64 bufsize){
    u64 i;
    for (i=0;i<strlength(org)&&i<bufsize-1;i++){
        res[i]=org[i];
    }
    res[i]=0;
}

u64 nextchars(u64 startpos, char keychar,char* orgstr, char* res, u64 buffsize){
    u64 endpos=0;
    for (u64 i=startpos;i<strlength(orgstr) && i-startpos<buffsize;i++){
        if(orgstr[i] != keychar){
            res[i-startpos]=orgstr[i];
        }else {
            endpos=i;
            i=strlength(orgstr);
        }
    }
    return endpos;
}