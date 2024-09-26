#include <stdio.h>
#include <stdlib.h>

int main() {
	int N = 0;
	int max = 0;
	int min = 100;
	int sum = 0;

	int* snum;
	int* score;
	char** name;

	printf("�л� �� �Է�: ");
	scanf_s("%d", &N);

	snum = (int*)malloc(sizeof(int) * N);
	score = (int*)malloc(sizeof(int) * N);
	name = (char**)malloc(sizeof(char*) * N);

	snum[0] = 1;

	for (int i = 0; i < N; i++) {
		name[i] = (char*)malloc(sizeof(char) * 100);
	}

	for (int i = 0; i < N; i++) {
		printf("�й�: ");
		scanf_s("%d", &snum[i]);
		printf("�̸�: ");
		scanf_s("%s", name[i], 100);
		printf("����: ");
		scanf_s("%d", &score[i]);

		if (score[i] > max) {
			max = score[i];
		}
		if (score[i] < min) {
			min = score[i];
		}

		sum += score[i];
	}

	printf("�ִ�: %d / �ּ�: %d / ���: %.2f", max, min, (float)sum / N);

	free(snum);
	free(score);
	for (int i = 0; i < N; i++) {
		free(name[i]);
	}
	snum = NULL;
	name = NULL;
	score = NULL;

	return 0;
}