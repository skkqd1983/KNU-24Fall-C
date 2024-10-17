#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

void fillRandom(int array[SIZE][SIZE]);
void printArray(int array[SIZE][SIZE]);
void movePointer(void* array);

int main() {
	int array[SIZE][SIZE];

	fillRandom(array);
	printArray(array);
	movePointer(array);

	return 0;
}

void fillRandom(int array[SIZE][SIZE]) {
	srand((unsigned int)time(NULL));

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			array[i][j] = rand() % 20 + 1;
		}
	}
}

void printArray(int array[SIZE][SIZE]) {
	printf("�迭 ���:\n");
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (array[i][j] < 10) {
				printf(" %d", array[i][j]);
			}
			else {
				printf("%d", array[i][j]);
			}
			printf(" ");
		}
		printf("\n");
	}
}

void movePointer(void* array) {
	int num = 0;
	while (1) {
		printf("���� ��ġ : (%d, %d), �迭�� �� : %d\n", num / 10, num % 10, *((int*)array + num));
		if (num + *((int*)array + num) < 100) {
			num += *((int*)array + num);
		}
		else {
			printf("�� �̻� �̵��� �� �����ϴ�.\n");
			break;
		}
	}
	printf("���� ��ġ : (%d, %d), �迭�� �� : %d", num / 10, num % 10, *((int*)array + num));
}