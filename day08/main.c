#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define AOC_MAX_JUNCTIONS 1024
#define AOC_CONNECTIONS 1000

struct pair {
    int i;
    int j;
    long long distanceSq;
};

static inline int cmpPair(const void* a, const void* b){
    struct pair p1 = *(struct pair *)a;
    struct pair p2 = *(struct pair *)b;
    if(p1.distanceSq < p2.distanceSq) return -1;
    if(p1.distanceSq > p2.distanceSq) return 1;
    return 0;
}


int getSet(int *circuits, int n){
    if (circuits[n] < 0){
        return n;
    }
    return circuits[n] = getSet(circuits, circuits[n]);
}

int main(int argc, char *argv[]) {
    long silver = 0;
    unsigned long long gold = 0;
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;
    char c; int junctionCount; int dim;
    junctionCount = dim = 0;    
    // junctions[i] = {x, y, z}
    long (*junctions)[3] = calloc(AOC_MAX_JUNCTIONS, sizeof(*junctions));
    while((c = fgetc(fp)) != EOF){
        if(c == ','){
            dim++;
        }else if(c == '\n'){
            dim = 0;
            junctionCount++;
        }else{
            junctions[junctionCount][dim] = junctions[junctionCount][dim]*10 + (c - '0');
        }
    }
    fclose(fp);
    struct pair* sqDistancePairs = calloc(((junctionCount*(junctionCount+1))/2) - junctionCount, sizeof(*sqDistancePairs));
    // printf("expected pairCount: %d\n", ((junctionCount*(junctionCount+1))/2) - junctionCount);
    int pairCount = 0;
    for(int i = 0; i < junctionCount; i++){
        // printf("junction %d: %d,%d,%d\n", i, junctions[i][0], junctions[i][1], junctions[i][2]);
        for(int j = i + 1; j < junctionCount; j++){
            long long dx = junctions[i][0] - junctions[j][0];
            long long dy = junctions[i][1] - junctions[j][1];
            long long dz = junctions[i][2] - junctions[j][2];
            sqDistancePairs[pairCount] = (struct pair){i, j, dx*dx + dy*dy + dz*dz};
            // printf("  to junction %d: %d,%d,%d = %ld\n", j, junctions[j][0], junctions[j][1], junctions[j][2], sqDistancePairs[pairCount].distanceSq);
            pairCount++;
        }
    }
    // printf("actual pairCount: %d\n", pairCount);

    qsort(sqDistancePairs, pairCount, sizeof(struct pair), cmpPair);
    int circuits[junctionCount];
    int remainingCircuits = junctionCount;
    int k;
    for(k = 0; k < junctionCount; k++){
        circuits[k] = -1;
    }
    for(k = 0; k < AOC_CONNECTIONS; k++){
        int iSet = getSet(circuits, sqDistancePairs[k].i);
        int jSet = getSet(circuits, sqDistancePairs[k].j);
        if(iSet != jSet){
            remainingCircuits--;
            circuits[jSet] = iSet;        
            // printf("connecting %d and %d with sets %d and %d\n", sqDistancePairs[k].i, sqDistancePairs[k].j, iSet, jSet);
        }
    };

    int circuitSizes[AOC_MAX_JUNCTIONS] = { 0 };
    for(k = 0; k < junctionCount; k++){
        int circ = getSet(circuits, k);
        circuitSizes[circ]++;
    }

    int bigThree[3] = { 1, 1, 1 };
    for(k = 0; k<junctionCount; k++){
        if(circuitSizes[k] > bigThree[0]){
            bigThree[2] = bigThree[1];
            bigThree[1] = bigThree[0];
            bigThree[0] = circuitSizes[k];
        }else if(circuitSizes[k] > bigThree[1]){
            bigThree[2] = bigThree[1];
            bigThree[1] = circuitSizes[k];
        }else if(circuitSizes[k] > bigThree[2]){
            bigThree[2] = circuitSizes[k];
        }
    }
    //printf("big three: %d, %d, %d\n", bigThree[0], bigThree[1], bigThree[2]);
    
    silver = bigThree[0]* bigThree[1]* bigThree[2];
    printf("silver %ld\n", silver);

    k = AOC_CONNECTIONS;
    while(remainingCircuits > 1){
        int iSet = getSet(circuits, sqDistancePairs[k].i);
        int jSet = getSet(circuits, sqDistancePairs[k].j);
        if(iSet != jSet){
            remainingCircuits--;
            circuits[jSet] = iSet;    
        }
        int numSets = 0;
        k++;
    }
    k--;
    gold = (junctions[sqDistancePairs[k].i][0]) * (junctions[sqDistancePairs[k].j][0]);
    printf("gold %llu\n", gold);
    return 0;
}