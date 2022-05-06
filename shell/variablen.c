#include <stdio.h>

int a = 1;
int b = 2;

int summe(int p1, int p2)
{
     printf("[summe]: a=%d, b=%d\n", a, b);
     return p1 + p2;
}

void diff(int p1, int p2, int* d)
{
    int a = 4;
    int b = 3;

    printf("[diff] : a=%d, b=%d\n", a, b);
    *d = p1 - p2;
}

int main(int argc, char** argv)
{
    int a = 5;
    int b = 6;
    int c = 3;

    printf("[main] : a=%d, b=%d\n", a, b);
    printf("a+b=%d\n", summe(a, b));

    diff(a, b, &c);
    printf("a-b=%d\n", c);

    return 0;
}
