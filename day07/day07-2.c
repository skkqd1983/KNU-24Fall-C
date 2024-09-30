#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double x = 0, y = 0, pie = 0;
int allcount = 0, count = 0, circle = 0;
int bar[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

double Test();

int main() {
	srand((unsigned int)time(NULL));

	for (int i = 1; i <= 100; i++) {
		if (i >= 5 && i % 5 == 0) {
			bar[i / 5-1] = 1;
		}

		if (i < 10) {
			printf("  ");
		}
		else if (i < 100) {
			printf(" ");
		}

		pie = Test();

		printf(" %d%%진행.. 원주율 : %lf ", i, pie);

		for (int j = 0; j < 20; j++) {
			if (bar[j] == 0) {
				printf("□");
			}
			else {
				printf("■");
			}
		}
		printf("\n");
	}
	printf("원주율 : %lf", pie);
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
	//printf("circle: %d, count: %d\n", circle, allcount);
	return 4 * ((double)circle / (double)allcount);
}