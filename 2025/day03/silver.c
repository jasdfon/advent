#include <stdio.h>
#include <stdlib.h>

#define AOC_CHAR_BUFFER_SIZE 128

int main(int argc, char *argv[]){
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;

    char battery[3]; char c;
    battery[0] = fgetc(fp);
    battery[1] = fgetc(fp);
    battery[2] = '\0';
    long silver = 0;
    int changed = 1;
    while((c = fgetc(fp)) != EOF){
        if(c == '\n'){
            silver += atoi(battery);
            battery[0] = fgetc(fp);
            battery[1] = fgetc(fp);
        }
        if(changed && battery[1] > battery[0]){
            battery[0] = battery[1];
            battery[1] = c;
            changed = 1;
        }else if(c > battery[1]){
            battery[1] = c;
            changed = 1;
        }else{
            changed = 0;
        }
    };

    printf("silver %ld\n", silver);
    fclose(fp);
    return 0;
}