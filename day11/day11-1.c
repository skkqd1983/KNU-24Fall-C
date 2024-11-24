#include <stdio.h>
#include <math.h>

void main() {
	double a, b , c, x1, x2, root;

	scanf_s("%lf %lf %lf", &a, &b, &c);						// 값 입력

	root = pow(b, 2) - 4 * a * c;							// 판별식

	if (root > 0) {
		x1 = (-b + sqrt(root)) / (2 * a);
		x2 = (-b - sqrt(root)) / (2 * a);
		printf("두 근: %.2f, %.2f\n", x1, x2);
	}
	else if (root == 0) {									// 중근 경우
		x1 = -b / (2 * a);
		printf("중근: %.2f\n", x1);
	}
	else {													// 허근
		printf("허근이기에 값을 출력하지 않습니다.\n");
	}
}