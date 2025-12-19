#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define AOC_MAX_REDS 512

struct tilePair {
    int ax, bx, ay, by;
    long long area;
};

static inline int cmpTilePair(const void* a, const void* b){
    const struct tilePair t1 = *(const struct tilePair *)a;
    const struct tilePair t2 = *(const struct tilePair *)b;
    if(t1.area < t2.area) return -1;
    if(t1.area > t2.area) return 1;
    return 0;
}

static inline int cmpPointsX(const void* a, const void* b){
    const int* p1 = (const int *)a;
    const int* p2 = (const int *)b;
    if(p1[0] < p2[0]) return -1;
    if(p1[0] > p2[0]) return 1;
    return 0;
}

static inline int cmpPointsY(const void* a, const void* b){
    const int* p1 = (const int *)a;
    const int* p2 = (const int *)b;
    if(p1[1] < p2[1]) return -1;
    if(p1[1] > p2[1]) return 1;
    return 0;
}

static inline int internal(struct tilePair p, int x, int y){
    return (x > p.ax && x < p.bx && y > p.ay && y < p.by); // fix to determin which x or y is min/max
}

int main(int argc, char *argv[]){
    int redTiles[AOC_MAX_REDS][2] = {0}; // [ x , y ] 
    int ySorted[AOC_MAX_REDS][2] = {0}; // [ x , y ] 
    int rectCount = 0, dim = 0, c = 0;
    FILE *fp = NULL;
    fp = fopen(argv[1], "r");
    while((c = fgetc(fp)) != EOF){
        if(c == ','){
            dim++;
        }else if(c == '\n'){
            rectCount++;
            dim = 0;
        }else if (c >= '0' && c <= '9'){
            redTiles[rectCount][dim] = redTiles[rectCount][dim]*10 + (c - '0');
            ySorted[rectCount][dim] = ySorted[rectCount][dim]*10 + (c - '0');
        }
    }
    fclose(fp);
    const int numPairs = (rectCount*(rectCount+1))/2 - rectCount;
    struct tilePair* areaPairs = calloc(numPairs, sizeof(*areaPairs));
    int currentIndex = 0;
    for(int i = 0; i < rectCount; i++){
        for(int j = i+1; j < rectCount; j++){
            if(redTiles[i][0] == redTiles[j][0] || redTiles[i][1] == redTiles[j][1]){
                continue; // skip same x or y
            }
            if(redTiles[i][0] > redTiles[j][0]){ // small x always ax
                areaPairs[currentIndex].ax = redTiles[i][0];
                areaPairs[currentIndex].bx = redTiles[j][0]; 
                areaPairs[currentIndex].ay = redTiles[i][1];
                areaPairs[currentIndex].by = redTiles[j][1]; 
            }else{
                areaPairs[currentIndex].ax = redTiles[j][0];
                areaPairs[currentIndex].bx = redTiles[i][0]; 
                areaPairs[currentIndex].ay = redTiles[i][1];
                areaPairs[currentIndex].by = redTiles[j][1]; 
            }
            areaPairs[currentIndex].area = (llabs(areaPairs[currentIndex].bx - areaPairs[currentIndex].ax) + 1) 
                * (llabs(areaPairs[currentIndex].by - areaPairs[currentIndex].ay )+ 1);
            printf("pair %d: %d,%d to %d,%d area %lld\n", currentIndex, areaPairs[currentIndex].ax, areaPairs[currentIndex].ay, areaPairs[currentIndex].bx, areaPairs[currentIndex].by, areaPairs[currentIndex].area);
            currentIndex++;
        }
    }
    printf("%d\n", currentIndex);

    qsort(areaPairs, numPairs, sizeof(*areaPairs), cmpTilePair);
    printf("silver %lld\n", areaPairs[numPairs-1].area);

    int (*xSorted)[2] = redTiles;
    qsort(xSorted, rectCount, sizeof(redTiles[0]), cmpPointsX);
    qsort(ySorted, rectCount, sizeof(redTiles[0]), cmpPointsY);

    int i;
    for(i = numPairs - 1; i > 0; i--){
        int j;
        for(j = 0; j < rectCount; j++){
            if(internal(areaPairs[i], redTiles[j][0], redTiles[j][1])){
                printf("found internal tile %d,%d in rect %d,%d to %d,%d\n", redTiles[j][0], redTiles[j][1], areaPairs[i].ax, areaPairs[i].ay, areaPairs[i].bx, areaPairs[i].by);
                break;
            }
        }
        if(j == rectCount){
            printf("testing area pair %d,%d to %d,%d - area %lld\n", areaPairs[i].ax, areaPairs[i].ay, areaPairs[i].bx, areaPairs[i].by, areaPairs[i].area);
            struct tilePair candidate = areaPairs[i];
            int k;
            for(k = 0; xSorted[k][0] <= candidate.bx; k++){
                if(xSorted[k][1] == candidate.ay){
                    printf("found left edge blocking tile %d,%d\n", xSorted[k][0], xSorted[k][1]);
                    break;
                }
            }
            if(xSorted[k][0] == candidate.ax) continue;
            for(k = rectCount - 1; xSorted[k][0] >= candidate.ax; k--){
                if(xSorted[k][1] == candidate.by){
                    printf("found right edge blocking tile %d,%d\n", xSorted[k][0], xSorted[k][1]);
                    break;
                }
            }
            if(xSorted[k][0] == candidate.bx) continue;
            const int maxY = candidate.ay > candidate.by ? candidate.ay : candidate.by;
            const int minY = candidate.ay > candidate.by ? candidate.by : candidate.ay;
            for(k = 0; ySorted[k][1] < minY; k++){
                if(ySorted[k][0] == candidate.bx){
                    printf("found top edge blocking tile %d,%d\n", ySorted[k][0], ySorted[k][1]);
                    break;
                }
            }
            if(ySorted[k][1] == minY) continue;
            for(k = rectCount - 1; ySorted[k][1] > maxY; k--){
                if(ySorted[k][0] == candidate.ax){
                    printf("found bottom edge blocking tile %d,%d\n", ySorted[k][0], ySorted[k][1]);
                    break;
                }
            }
            if(ySorted[k][1] == maxY) continue;
            printf("breaking\n");
            break;
        }
    }
    printf("gold %lld\n", areaPairs[i].area);

    return 1;
}