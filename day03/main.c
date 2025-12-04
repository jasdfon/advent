#include <stdio.h>
#include <stdlib.h>

#define GOLD_BATTERY_SIZE 12
#define SILVER_BATTERY_SIZE 2

void updateBattery(char *bat, int batSize, char c){
    int cAvail = 1;
    for(int i = 0; i < batSize - 1 && cAvail; i++){
        if(bat[i] < bat[i+1]){
            while(i < batSize - 1){
                bat[i] = bat[i+1];
                i++;
            }
            bat[batSize - 1] = c;
            cAvail = 0;
        }
    }
    if(cAvail && c > bat[batSize - 1]){
        bat[batSize - 1] = c;
    }
}

void initBatteries(char *goldBat, char *silverBat, FILE *fp){
    int i = 0;
    for(i = 0; i < GOLD_BATTERY_SIZE; i++){
        char c = fgetc(fp);
        if(c == EOF){
            break;
        }
        goldBat[i] = c;
        if( i < SILVER_BATTERY_SIZE ){
            silverBat[i] = c;
        }else{
            updateBattery(silverBat, SILVER_BATTERY_SIZE, c);
        }        
    }
}

int main(int argc, char *argv[]){
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;
    char c;
    char goldBat[GOLD_BATTERY_SIZE+1];
    goldBat[GOLD_BATTERY_SIZE] = '\0';
    char silverBat[SILVER_BATTERY_SIZE+1];
    silverBat[SILVER_BATTERY_SIZE] = '\0';
    long gold = 0;
    long silver = 0;
    initBatteries(goldBat, silverBat, fp);
    while((c = fgetc(fp)) != EOF){
        if(c == '\n'){
            silver += atol(silverBat);
            gold += atol(goldBat);
            initBatteries(goldBat, silverBat, fp);
        }else{
            updateBattery(goldBat, GOLD_BATTERY_SIZE, c);
            updateBattery(silverBat, SILVER_BATTERY_SIZE, c);
        }
    };

    printf("silver %ld\n", silver);
    printf("gold %ld\n", gold);
    fclose(fp);
    return 0;
}