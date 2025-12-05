#include <stdio.h>
#include <stdlib.h>

#define AOC_MAX_LINE_LENGTH 32
#define AOC_HALF_MAX_LINES 1024

int inRange(long val, long ranges[][2], int rangesCount, int skipIndex){
    for(int i = 0; i < rangesCount; i++){
        if(i == skipIndex) continue;
        if(ranges[i][0] > -1 && val >= ranges[i][0] && val <= ranges[i][1]){
            return i;
        }
    }
    return -1;
}

long countRangesCovered(long ranges[][2], int rangesCount){
    long count = 0;
    for(int i = 0; i < rangesCount; i++){
        if(ranges[i][0] > -1){
            count += (ranges[i][1] - ranges[i][0] + 1);
        }
    }
    return count;
}

void mergeRanges(long ranges[][2], int rangesCount){
    for(int i = 0; i < rangesCount; i++){
        int startIndex = inRange(ranges[i][0], ranges, rangesCount, i);
        int endIndex = inRange(ranges[i][1], ranges, rangesCount, i);
        if(startIndex < 0 && endIndex < 0){
            continue;
        }
        if(startIndex == endIndex){
            ranges[i][0] = -1;
            continue;
        } 
        if(startIndex > -1){
            ranges[startIndex][1] = ranges[i][1]; // move other end up
            ranges[i][0] = -1;
        }else if(endIndex > -1){
            ranges[endIndex][0] = ranges[i][0]; // move other start down
            ranges[i][0] = -1;
        }
    }
}

int unspoiledCount(long *spoiled, int spoiledLen, long ranges[][2], int rangesCount){
    int count = 0;
    long seen[spoiledLen];
    int seenLen = 0;
    for(int i = 0; i < spoiledLen; i++){
        if(inRange(spoiled[i], ranges, rangesCount, -1) > -1){
            int j;
            for(j = 0; j < seenLen; j++){
                if(seen[j] == spoiled[i]){
                    break;
                }
            }
            if(j == seenLen){
                count++;
                seen[seenLen++] = spoiled[i];
            }
        }
    }
    return count;
}

int main(int argc, char *argv[]){
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;

    char line[AOC_MAX_LINE_LENGTH];
    long ranges[AOC_HALF_MAX_LINES][2];
    long spoiled[AOC_HALF_MAX_LINES];
    char c;
    char rangeInput = 1;
    int rangesCount = 0;
    int spoiledCount = 0;
    int characters = 0; 
    while((c = fgetc(fp)) != EOF){
        if(c == '-'){
            line[characters] = '\0';
            characters = 0;
            ranges[rangesCount][0] = atol(line);
            rangeInput = 2;
        }else if(c == '\n'){
            line[characters] = '\0';
            characters = 0;
            if(rangeInput == 1){
                rangeInput = -1; // input part 2
            }else if(rangeInput == 2){
                ranges[rangesCount][1] = atol(line);
                rangeInput = 1;
                rangesCount++;
            }else{
                spoiled[spoiledCount] = atol(line);
                spoiledCount++;
            }
        }else{
            line[characters] = c;
            characters++;
        }
    };

    mergeRanges(ranges, rangesCount);
    long silver = unspoiledCount(spoiled, spoiledCount, ranges, rangesCount);
    long gold = countRangesCovered(ranges, rangesCount);

    printf("silver %ld\n", silver);
    printf("gold %ld\n", gold);
    fclose(fp);
    return 0;
}