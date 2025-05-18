#include <stdio.h>

void printandflush(char text[]) {
    printf("%s", text);
    fflush(stdout);
}
