#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double x = 0, y = 0, pie = 0;
int allcount = 0, count = 0, circle = 0;

double Test();

int main() {
	srand((unsigned int)time(NULL));

	for (int i = 1; i <= 100; i++) {
		if (i < 10) {
			printf("  ");
		}
		else if (i < 100) {
			printf(" ");
		}

		pie = Test();

		printf("%d%%����.. ������ : %lf ", i, pie);

		for (int j = 0; j < 20; j++) {
			if (j < i / 5) {
				printf("��");
			}
			else {
				printf("��");
			}
		}
		printf("\n");
	}
	printf("������ : %lf", pie);
}

double Test() {
	count = 0;
	while (count < 10000000) {
		x = (double)rand() / (double)RAND_MAX;
		y = (double)rand() / (double)RAND_MAX;
		if (sqrt(pow(x, 2) + pow(y, 2)) < 1) {
			circle++;
		}
		count++;
		allcount++;
	}
	return 4 * ((double)circle / (double)allcount);
}