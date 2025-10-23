#include <stdio.h>

void print_hello(void) {
    printf("xd");
}

extern int suma(int, int);

int main(void) {
    printf("Suma: %d\n", suma(5, 3));
    print_hello();
    return 0;
}
