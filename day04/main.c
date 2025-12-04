
#include <stdio.h>
#include <stdlib.h>

#define GRID_MAX 256
#define MAX_ADJACENT 4

void copyMap(char src[][GRID_MAX], char dest[][GRID_MAX]){
    for( int i = 0; i < GRID_MAX; i++){
        for( int j = 0; j < GRID_MAX; j++){
            dest[i][j] = src[i][j];
        }
    }
}

void updateAround(char heatMap[][GRID_MAX], int x, int y, int updateVal){
    for(int dx = -1; dx <= 1; dx++){
        if(x + dx < 0 || x + dx >= GRID_MAX) continue;
        for(int dy = -1; dy <= 1; dy++){
            if(dx == 0 && dy == 0) continue;
            if(y + dy < 0 || y + dy >= GRID_MAX) continue;
            heatMap[y + dy][x + dx] += updateVal;
        }
    }
}

 int removeAccesiblePaper(char heatMap[][GRID_MAX], char nextMap[][GRID_MAX],  int paperLocations[][2],  int paperCount){
     int count = 0;
     int x;
     int y;
    for( int i = 0; i < paperCount; i++){
        x = paperLocations[i][0];
        y = paperLocations[i][1];
        if(x == -1) continue; // already removed
        if(heatMap[y][x] < MAX_ADJACENT){
            paperLocations[i][0] = -1;
            updateAround(nextMap, x, y, -1); // adjust future map
            count++;
        }
    }
    return count;
}

int main(int argc, char *argv[]){
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;
    char c;
    char currentMap[GRID_MAX][GRID_MAX] = {0};
     int paperLocations[GRID_MAX * GRID_MAX][2] = {0};
     int paperCount = 0;
    
     long gold = 0;
     long silver = 0;
     int x = 0;
     int y = 0;
    while((c = fgetc(fp)) != EOF){
        if(c == '\n'){
            x = 0;
            y++;
        }else{
            if(c == '@'){
                updateAround(currentMap, x, y, 1);
                paperLocations[paperCount][0] = x;
                paperLocations[paperCount][1] = y;
                paperCount++;
            }
            x++;
        }
    };
    char nextMap[GRID_MAX][GRID_MAX];
    copyMap(currentMap, nextMap);

    int removedPaper = removeAccesiblePaper(currentMap, nextMap, paperLocations, paperCount);
    silver = removedPaper;
    while(removedPaper > 0){
        gold += removedPaper;
        copyMap(nextMap, currentMap);   
        removedPaper = removeAccesiblePaper(currentMap, nextMap, paperLocations, paperCount);
    }

    fclose(fp);
    printf("silver %ld\n", silver);
    printf("gold %ld\n", gold);
 
    return 0;
}