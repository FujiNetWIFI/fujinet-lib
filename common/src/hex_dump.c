#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void hd(void* data, int size) {
    int i = 0;
    int j = 0;
    int p = 0;
    int start = 0;
    int padding = 0;
    unsigned char c;

    for (i = 0; i < size; i++) {
        printf("%02x ", *((unsigned char*)data + i));

        if ((i + 1) % 8 == 0 || i == size - 1) {
            padding = ((i + 1) % 8) ? (8 - (i + 1) % 8) : 0;
            for (p = 0; p < padding; p++) {
                printf("   "); // for alignment
            }
            printf(" | ");
            start = i - (i % 8);
            for (j = start; j <= i; j++) {
                c = *((unsigned char*)data + j);
                if (isprint(c)) {
                    printf("%c", c);
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
    }
}