#include <stdio.h>
#include <stdlib.h>

#define AOC_GRID_SIZE 1024

long performOps(int grid[AOC_GRID_SIZE][AOC_GRID_SIZE], int gridXLen, int gridYLen){
    long total = 0;
    long current;
    int opLocation = gridYLen -1;
    for(int i = 0; i < gridXLen; i++){
        current = grid[i][0];
        for(int j = 1; j < opLocation; j++){
            if(grid[i][opLocation] == '*'){
                current *= grid[i][j];
            }else{
                current += grid[i][j];
            }
        }
        total += current;
    }
    return total;
}

long performCephaOps(int cepha[AOC_GRID_SIZE], int cephaLen, int grid[AOC_GRID_SIZE][AOC_GRID_SIZE], int gridYLen){
    long total = 0;
    long current; 
    int opLocation = gridYLen -1;
    int currentOp = 0;
    int j;
    for(int i = 0; i < cephaLen; i++ && currentOp++){
        current = cepha[i];
        for(j = 1; i+j < cephaLen && cepha[i+j] > 0; j++){
            if(grid[currentOp][opLocation] == '*'){
                current *= cepha[i+j];
            }else{
                current += cepha[i+j];
            }
        }
        i += j;
        total += current;
    }
    return total;
}

int main(int argc, char *argv[]) {
    long silver = 0;
    long gold = 0;
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;
    char c; int charCount = 0;
    char current[16];
    int gridX = 0; int gridY = 0;
    int grid[AOC_GRID_SIZE][AOC_GRID_SIZE];
    int cephaX = 0; int cephaLen = 0;
    int cepha[AOC_GRID_SIZE*8];
    char readingOps = 0;
    while((c = fgetc(fp)) != EOF){
        if(c == ' ' || c == '\n'){
            if(charCount > 0){
                    current[charCount] = '\0';
                    grid[gridX++][gridY] = atoi(current);
                    charCount = 0;
            }
            if( c == '\n'){
                gridY++; 
                if(cephaX > cephaLen) cephaLen = cephaX;
                if(!readingOps){
                    gridX = 0; cephaX = 0;
                }
            }else{
                if(!readingOps){
                    cephaX++;
                }
            }
        }else if(!readingOps){
            if(gridX == 0 && (c < '0' || c > '9')){
                readingOps = 1;
                grid[gridX++][gridY] = c;
            }else{
                cepha[cephaX++] = cepha[cephaX]*10 + (c - '0');
                current[charCount++] = c;
            }
        }else{
            grid[gridX++][gridY] = c;
        }
    }
    
    // printf("-elf grid-\n");
    // for(int i = 0; i < gridY; i++){
    //     for(int j = 0; j < gridX; j++){
    //         if(i < gridY -1){
    //             printf("%d ", grid[j][i]);
    //         }else{
    //             printf("%c ", grid[j][i]);
    //         }
    //     }
    //     printf("\n");
    // }
    // printf("-cepha nums-\n");
    // for(int i = 0; i < cephaLen; i++){
    //     printf("%d ", cepha[i]);
    // }
    // printf("\n");

    silver = performOps(grid, gridX, gridY);
    printf("silver %ld\n", silver);
    gold = performCephaOps(cepha, cephaLen, grid, gridY);
    printf("gold %ld\n", gold);
    return 0;
}