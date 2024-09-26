#include <stdio.h>
#include <stdlib.h>

struct Student {
	int snum;
	int score;
	char name[100];
};

int main() {
	struct Student *st;

	int N = 0, max = 0, min = 100, sum = 0;

	printf("�л� �� �Է�: ");
	scanf_s("%d", &N);

	st = (struct Student*)malloc(sizeof(struct Student) * N);

	for (int i = 0; i < N; i++) {
		printf("�й�: ");
		scanf_s("%d", &st[i].snum);
		printf("�̸�: ");
		scanf_s("%s", st[i].name, 100);
		printf("����: ");
		scanf_s("%d", &st[i].score);

		if (st[i].score > max) {
			max = st[i].score;
		}
		if (st[i].score < min) {
			min = st[i].score;
		}
		sum += st[i].score;
	}

	printf("�ִ�: %d / �ּ�: %d / ���: %.2f", max, min, (float)sum / N);

	free(st);
	st = NULL;

	return 0;
}