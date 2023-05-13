#include "../math.h"

void arrclone(byte org[8], byte cpy[8]){
    for (int i=0;i<8;i++){
        cpy[i] = org[i];
    }
}

u64 power(u64 org, u64 times){
    u64 out=1;
    for (u64 i = 0;i<times;i++){
        out*=org;
    }
    return out;
}

void byteAdd(byte first[8], byte second[8], byte res[8]){
    byte over=0;
    arrclone(first, res);
    for (int i = 0;i<8;i++){
        uint tmpres = res[i] + second[i] + over;
        over=0;
        if (tmpres > 255){
            res[i] = tmpres % 256;
            tmpres-=tmpres % 256;
            over=tmpres/256;
        }else{
            res[i] = tmpres;
        }
    } 
}

void byteSub(byte first[8], byte second[8], byte res[8]){
    byte over = 0;
    arrclone(first, res);
    for (byte i=0;i<8;i++){
        int tmpres = res[i] - second[i] - over;
        over=0;
        if (tmpres < 0){
            over=1;
            res[i]=256+tmpres;
        }else{
            res[i]=tmpres;
        }
    }
};

u64 toInt(byte num[8]){
    u64 out=0;
    for (byte i=0;i<8;i++){
        out+=num[i]*power(256, i);
    }
    return out;
};

void toBytes(u64 num, byte res[8]){
    for (byte i = 0;i<8;i++){
        res[i]=num%256;
        num-=num%256;
        num/=256;
    }
};

void printarr(byte num[8]){
    printf("[ ");
    for (byte i = 0; i<7;i++){
        printf("%u, ", num[i]);
    }
    printf("%u ](%lu)\n",num[7],toInt(num));
};
