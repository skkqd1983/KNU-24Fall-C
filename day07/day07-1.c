#include <stdio.h>

int main() {
	int N = 5;

	printf("%d�� �Ƕ�̵�\n", N);
	for (int i = 0; i < N; i++) {
		for (int j = 1; j <= N * 2 - 1; j++) {
			if (N - i <= j && j <= N + i) {
				printf("*");
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	}
}