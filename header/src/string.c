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
    for (i64 i = len-1;i>=0;i--){
        switch (str[i]) {
        case '0':
            out+=0;
            break;
        case '1':
            out+=1*power(10, len-i-1);
            break;
        case '2':
            out+=2*power(10, len-i-1);
            break;
        case '3':
            out+=3*power(10, len-i-1);
            break;
        case '4':
            out+=4*power(10, len-i-1);
            break;
        case '5':
            out+=5*power(10, len-i-1);
            break;
        case '6':
            out+=6*power(10, len-i-1);
            break;
        case '7':
            out+=7*power(10, len-i-1);
            break;
        case '8':
            out+=8*power(10, len-i-1);
            break;
        case '9':
            out+=9*power(10, len-i-1);
            break;
        default:
            break;
        }
    }
    return out;
}