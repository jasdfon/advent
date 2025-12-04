#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define AOC_CHAR_BUFFER_SIZE 64

long MAX_VALUES[16] = {0, 9, 99, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999, 9999999999, 99999999999, 999999999999, 9999999999999, 99999999999999, 999999999999999};
long TEN_MULTIPLIERS[16] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000, 1000000000000000};

long expandId(long val, int partLen, int n){
    long newVal = val;
    for(int i = partLen; i < n*partLen; i+=partLen){
        newVal += val*TEN_MULTIPLIERS[i];
    }
    return newVal;
}

int countDigits(long n){
    int count = 0;
    while (n > 0) {
        n /= 10;
        count++;
    }
    return count;
}

long dedupeSum(long *ids, int length){
    long seen[length];
    int seenLen = 0;
    long sum = 0;

    for(int i = 0; i < length; i++){
        long x = ids[i];
        int dupe = 0;
        for(int j = 0; j < seenLen; j++){
            if (seen[j] == x) { dupe = 1; break; }
        }
        if(!dupe){
            seen[seenLen++] = x;
            sum += x;
        }
    }
    return sum;
}

long nextInvalidId(long current, int *digits, int n){
    //printf("nextInvalidId current: %ld, digits: %d, n: %d\n", current, *digits, n);
    if(*digits % n != 0){
        int partLen = ceil(*digits/(double)n);
        *digits = partLen * n;    
        return expandId(TEN_MULTIPLIERS[partLen-1], partLen, n);
    }
    long parts[n];
    int partLen = (*digits)/n;
    for(int i = n-1; i >= 0; i--){
        parts[i] = current % TEN_MULTIPLIERS[partLen];
        current = current / TEN_MULTIPLIERS[partLen];
        //printf("part %d: %ld\n", i, parts[i]);
    }
    int invalidId = 1;
    int i = 1;
    for(i = 1; i < n; i++){
        if(parts[i] != parts[0]){
            invalidId = 0;
            break;
        }
    }
    if(invalidId && parts[0] == MAX_VALUES[partLen]){ // if its 9999 and n=2 expand to 100100 etc
        *digits = (partLen+1) * n;
        return expandId(parts[0]+1, partLen+1, n);
    }
    if(i < n && parts[i] < parts[0]){
        return expandId(parts[0], partLen, n);
    }
    return expandId(parts[0]+1, partLen, n);
}

void invalidIds(char *aStr, int aLen, char *bStr, int n, long *resArray, int *resLen){
    int i;
    long result = 0;
    long aVal = atol(aStr);
    long bVal = atol(bStr);
    int digits = countDigits(aVal);
    if(aLen % n == 0){
        char first[16];
        for(i = 0; i < aLen/n; i++){
            first[i] = aStr[i];
        }
        first[aLen/n] = '\0';
        char temp;
        int partLen = aLen/n;
        int addId = 1;
        for(i = 0; i < n && addId; i++){
            temp = aStr[(i+1)*partLen];
            aStr[(i+1)*partLen] = '\0';
            if(strcmp(first, aStr + i*partLen) != 0){
                addId = 0;
            }
            aStr[(i+1)*partLen] = temp;
        }
        if(addId){
            resArray[(*resLen)++] = aVal;
        }
    }
    aVal = nextInvalidId(aVal, &digits, n);
    while(aVal <= bVal){
        /*printf("digits: %d, n: %d\n", digits, n);
        printf("aVal: %ld\n", aVal);
        printf("bVal: %ld\n", bVal);*/
        resArray[(*resLen)++] = aVal;
        aVal = nextInvalidId(aVal, &digits, n);
    }
}

int main(int argc, char *argv[]){
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;

    char aStr[AOC_CHAR_BUFFER_SIZE];
    char bStr[AOC_CHAR_BUFFER_SIZE];
    int aLen = 0; int bLen = 0;
    char c;
    char onA = 1;
    long silver = 0;
    long gold = 0;        
    long resArray[4096];
    int resLen = 0;
    while((c = fgetc(fp)) != EOF){
        if(c == '-'){
            aStr[aLen] = '\0';
            onA = 0;
        }else if(c == ',' || c == '\n'){
            bStr[bLen] = '\0';
            onA = 1;
            invalidIds(aStr, aLen, bStr, 2, resArray, &resLen);
            silver += dedupeSum(resArray, resLen);
            for(int n = bLen; n > 2; n--){
                invalidIds(aStr, aLen, bStr, n, resArray, &resLen);
            }
            gold += dedupeSum(resArray, resLen);
            aLen = 0; bLen = 0; resLen = 0;
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
    printf("gold %ld\n", gold);
    fclose(fp);
    return 0;
}