#include "stdlib.h"

int main() {
    int *i = malloc(1);
    free(i);
    free(i);
    return 0;
}
