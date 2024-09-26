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

	printf("학생 수 입력: ");
	scanf_s("%d", &N);

	st = (struct Student*)malloc(sizeof(struct Student) * N);

	for (int i = 0; i < N; i++) {
		printf("학번: ");
		scanf_s("%d", &st[i].snum);
		printf("이름: ");
		scanf_s("%s", st[i].name, 100);
		printf("점수: ");
		scanf_s("%d", &st[i].score);

		if (st[i].score > max) {
			max = st[i].score;
		}
		if (st[i].score < min) {
			min = st[i].score;
		}
		sum += st[i].score;
	}

	printf("최대: %d / 최소: %d / 평균: %.2f", max, min, (float)sum / N);

	free(st);
	st = NULL;

	return 0;
}