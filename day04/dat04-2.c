#include <stdio.h>

void swap(int* a, int* b) {
	int c = *a;

	*a = *b;
	*b = c;
}

int main() {
	int a = 10, b = 20;

	printf("a : %d\nb : %d\n\n", a, b);


	swap(&a, &b);


	printf("a : %d\nb : %d\n", a, b);
}