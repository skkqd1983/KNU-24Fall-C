#include <stdio.h>
struct vector {
	int x;
	int y;
	int z;
};

void sum(struct vector v1, struct vector v2) {
	struct vector v3;
	v3.x = v1.x + v2.x;
	v3.y = v1.y + v2.y;
	v3.z = v1.z + v2.z;
	printf("벡터의 합은\tx:%d\ty%d\tz:%d\n", v3.x, v3.y, v3.z);
}

void sub(struct vector v1, struct vector v2) {
	struct vector v3;
	v3.x = v1.x - v2.x;
	v3.y = v1.y - v2.y;
	v3.z = v1.z - v2.z;
	printf("벡터의 차는\tx:%d\ty%d\tz:%d\n", v3.x, v3.y, v3.z);
}

void cross(struct vector v1, struct vector v2) {
	struct vector v3;
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;
	printf("벡터의 외적은\tx:%d\ty%d\tz:%d\n", v3.x, v3.y, v3.z);
}

void dot(struct vector v1, struct vector v2) {
	int num;
	num = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	printf("벡터의 내적은\tvec1 * vec2=%d\n", num);
}

void main() {
	struct vector v1;
	struct vector v2;
	int num = 0;

	printf("첫번째 벡터(x,y,z) : ");
	scanf_s("%d, %d, %d", &v1.x, &v1.y, &v1.z);
	printf("두번째 벡터(x,y,z) : ");
	scanf_s("%d, %d, %d", &v2.x, &v2.y, &v2.z);

	printf("입력된 첫번째 벡터\t%d,\t%d,\t%d\n", v1.x, v1.y, v1.z);
	printf("입력된 두번째 벡터\t%d,\t%d,\t%d\n\n\n", v2.x, v2.y, v2.z);
	
	while (1) {
		printf("------------------------\n");
		printf("1. 벡터의 합\n2. 벡터의 차\n3. 벡터의 외적\n4. 벡터의 내적\n5. 종료\n");
		printf("------------------------\n명령 입력 : ");
		scanf_s("%d", &num);

		if (num == 1) {
			sum(v1, v2);
		}
		else if (num == 2) {
			sub(v1, v2);
		}
		else if (num == 3) {
			cross(v1, v2);
		}
		else if (num == 4) {
			dot(v1, v2);
		}
		else if (num == 5) {
			break;
		}
	}
}