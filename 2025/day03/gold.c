#include <stdio.h>
#include <stdlib.h>

#define BATTERY_SIZE 12

void updateBattery(char* bat, char c){
    int cAvail = 1;
    for(int i = 0; i < BATTERY_SIZE - 1 && cAvail; i++){
        if(bat[i] < bat[i+1]){
            while(i < BATTERY_SIZE - 1){
                bat[i] = bat[i+1];
                i++;
            }
            bat[BATTERY_SIZE - 1] = c;
            cAvail = 0;
        }
    }
    if(cAvail && c > bat[BATTERY_SIZE - 1]){
        bat[BATTERY_SIZE - 1] = c;
    }
}

void initBattery(char* bat, FILE *fp){
    for(int i = 0; i < BATTERY_SIZE; i++){
        bat[i] = fgetc(fp);
    }
}

int main(int argc, char *argv[]){
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;

    char bat[BATTERY_SIZE+1]; char c;
    bat[BATTERY_SIZE] = '\0';
    long gold = 0;
    initBattery(bat, fp);
    while((c = fgetc(fp)) != EOF){
        if(c == '\n'){
            gold += atol(bat);
            initBattery(bat, fp);
        }else{
            updateBattery(bat, c);
        }
    };

    printf("gold %ld\n", gold);
    fclose(fp);
    return 0;
}