#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int position = 50;
    long silver = 0;
    long gold = 0;

    FILE *fp = fopen(argv[1], "r");
    if (!fp) return 1;

    char line[64];

    while (fgets(line, sizeof(line), fp)) {
        int len = strlen(line);
        if (line[len-1] == '\n'){ 
            line[len-1] = '\0';
            len--;
        }
        if(len < 3){
            line[2] = line[1];
            line[1] = '0';
            line[3] = '\0';
            len = 3;
        }
        int negative = (line[0] == 'L');
        int positionMove = atoi(line + (len - 2));
        line[len - 2] = '\0';
        int fullRotationCount = atoi(line + 1);
        int initialPos = position;
        gold += fullRotationCount;

        if (negative) {
            position -= positionMove;
            if (position < 0) {
                position += 100;
                gold += 1;
            }
            if (position == 0) {
                gold += 1;
            }
            if (initialPos == 0) {
                gold -= 1;
            }
        } else {
            position += positionMove;
            if (position > 99) {
                position -= 100;
                gold += 1;
            }
        }
        if (position == 0 && (fullRotationCount > 0 || positionMove > 0)) {
            silver += 1;
        }
    }

    printf("silver %ld\n", silver);
    printf("gold %ld\n", gold);
    return 0;
}