#include <stdio.h>
#include <math.h>

void main() {
	double a, b , c, x1, x2, root;

	scanf_s("%lf %lf %lf", &a, &b, &c);						// �� �Է�

	root = pow(b, 2) - 4 * a * c;							// �Ǻ���

	if (root > 0) {
		x1 = (-b + sqrt(root)) / (2 * a);
		x2 = (-b - sqrt(root)) / (2 * a);
		printf("�� ��: %.2f, %.2f\n", x1, x2);
	}
	else if (root == 0) {									// �߱� ���
		x1 = -b / (2 * a);
		printf("�߱�: %.2f\n", x1);
	}
	else {													// ���
		printf("����̱⿡ ���� ������� �ʽ��ϴ�.\n");
	}
}