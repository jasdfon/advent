#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define AOC_CHAR_BUFFER_SIZE 256

int countDigits(long n) {
    int count = 0;
    while (n > 0) {
        n /= 10;
        count++;
    }
    return count;
}

long nextInvalidId(long n, int *digits){
    long next = n + (pow(10, (*digits)/2)) + 1;
    if (next >= pow(10, (*digits))){
        *digits = *digits+2;
        next = pow(10, *digits-1) + pow(10, (*digits/2)-1);
    }
    return next;
}

long invalidIdSum(char* aStr, int aLen, char* bStr){
    //printf("finding invalid Ids from %s, len %d to %s\n", aStr, aLen, bStr);
    long result = 0;
    long i;
    long aVal = atol(aStr);
    long bVal = atol(bStr);
    int digits;
    if(aLen % 2 == 0){
        char temp;
        temp = aStr[aLen/2];
        aStr[aLen/2] = '\0';
        long firstHalf = atol(aStr);
        aStr[aLen/2] = temp;
        long secondHalf = atol(aStr + aLen/2);
        if(firstHalf < secondHalf){   // first half is higher than second half
            firstHalf += 1;
        }
        aVal = firstHalf*(pow(10, aLen/2)) + firstHalf;
        digits = countDigits(aVal);
    }else{
        digits = aLen+1;
        aVal = pow(10, digits-1) + pow(10, (digits/2)-1);
    }
    //printf("moved to first invalid Id %ld, bVal %ld\n", aVal, bVal);
    while(aVal <= bVal){
        //printf("adding %ld\n", aVal);
        result += aVal;
        aVal = nextInvalidId(aVal, &digits);
    }
    return result;
}

int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;

    char aStr[AOC_CHAR_BUFFER_SIZE];
    char bStr[AOC_CHAR_BUFFER_SIZE];
    int aLen = 0; int bLen = 0;
    char c;
    char onA = 1;
    long silver = 0;
    while((c = fgetc(fp)) != EOF){
        if(c == '-'){
            aStr[aLen] = '\0';
            onA = 0;
        }else if(c == ',' || c == '\n'){
            bStr[bLen] = '\0';
            onA = 1;
            silver += invalidIdSum(aStr, aLen, bStr);
            aLen = 0; bLen = 0;
        }else {
            if(onA){
                aStr[aLen] = c;
                aLen++;
            }else{
                bStr[bLen] = c;
                bLen++;
            };
        }
    };
    printf("silver %ld\n", silver);
    fclose(fp);
    return 0;
}