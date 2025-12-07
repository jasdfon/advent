#include <stdio.h>
#include <stdlib.h>

#define AOC_LINE_MAX 256

int main(int argc, char *argv[]) {
    long silver = 0;
    long gold = 0;
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;
    char c;
    long* state = calloc(AOC_LINE_MAX, sizeof(long));
    long* nextState = calloc(AOC_LINE_MAX, sizeof(long));
    int stateMax = 0;
    
    int currentPos = 0;
    while((c = fgetc(fp)) != 'S'){
        currentPos++;
    };
    state[currentPos] = 1;
    while((fgetc(fp)) != '\n')
    currentPos=0;

    while((c = fgetc(fp)) != EOF){
        nextState[currentPos+2] = 0; // clear the buffer ahead
        if(c == '\n'){
            stateMax = currentPos > stateMax ? currentPos : stateMax;
            currentPos = 0;
            void * tmp = state;
            state = nextState;
            nextState = tmp;
            nextState[0] = 0; // clear start of the buffer
            nextState[1] = 0;
        }else{
            if(c == '^'){
                if(state[currentPos] > 0){ // split beam (or timeline)
                    silver += 1;
                    nextState[currentPos-1] += state[currentPos];
                    nextState[currentPos+1] += state[currentPos];
                    nextState[currentPos] = 0;
                }
            }else{
                nextState[currentPos] += state[currentPos];
            }
            currentPos++;
        }
    }
    fclose(fp);
    printf("silver %ld\n", silver);
    for(int i = 0; i < stateMax; i++){ // sum timelines
        gold += state[i];
    }
    printf("gold %ld\n", gold);
    return 0;
}