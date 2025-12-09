#include <stdio.h>
#include <stdlib.h>

#define AOC_MAX_RECTS 512

int main(int argc, char *argv[]) {
    long silver = 0;
    int rects[AOC_MAX_RECTS][2] = {0};
    int rectCount = 0; int dim = 0; int c;
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;
    while((c = fgetc(fp)) != EOF){
        if(c == ','){
            dim++;
        }else if(c == '\n'){
            if(dim == 1){
                rectCount++;
            }
            dim = 0;
        }else{
            rects[rectCount][dim] = rects[rectCount][dim]*10 + (c - '0');
        }
    }
    fclose(fp);

    for(int i = 0; i < rectCount; i++){
        printf("Rect %d: (%d, %d)\n", i, rects[i][0], rects[i][1]);
    }

    long maxArea = 0;
    for(int i = 0; i < rectCount; i++){
        int farthest[2] = { 0, 1 };
        for(int j = i; j < rectCount; j++){
            int width = rects[j][0] - rects[i][0];
            int height = rects[j][1] - rects[i][1];
            long area = abs(width * height);
            if(area > maxArea){
                maxArea = area;
                farthest[0] = j;
                farthest[1] = i;
            }
        }
    }
    printf("silver %ld\n", maxArea);
    return 0;
}