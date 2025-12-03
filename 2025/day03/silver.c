#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;

    char bat[3]; char c;
    bat[0] = fgetc(fp);
    bat[1] = fgetc(fp);
    bat[2] = '\0';
    long silver = 0;
    int changed = 1;
    while((c = fgetc(fp)) != EOF){
        if(c == '\n'){
            silver += atoi(bat);
            bat[0] = fgetc(fp);
            bat[1] = fgetc(fp);
        }
        if(changed && bat[1] > bat[0]){
            bat[0] = bat[1];
            bat[1] = c;
            changed = 1;
        }else if(c > bat[1]){
            bat[1] = c;
            changed = 1;
        }else{
            changed = 0;
        }
    };

    printf("silver %ld\n", silver);
    fclose(fp);
    return 0;
}