#include <stdio.h>

extern int sumar(int a, int b);

int main() {
int numero;
int x=1;
int y=1;
int fib=1;
printf("Introduce un número: ");
scanf("%d", &numero);
printf("Fibonacci 1: 1\n");
for (int i = 0; i < (numero-1); i++) {
    printf("Fibonacci %d: %d\n", i + 2, fib);
fib = sumar(x, y);
x=y;
y=fib;
}

return 0;
}