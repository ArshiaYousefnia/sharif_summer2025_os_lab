#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *fp = fopen("/proc/interrupts", "r");

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    fclose(fp);
    return 0;
}

