#include <stdio.h> 
int main() {
	int A;
	double num1, num2;

	printf("������ ����� �Է��Ͻÿ�.\n1.���ϱ� 2.���� 3.���ϱ� 4.������\n");

	printf("��� : _\b");
	scanf_s("%d", &A);

	printf("���� 1�� �Է��ϼ��� : __\b\b");
	scanf_s("%lf", &num1);
	getchar;
	printf("���� 2�� �Է��ϼ��� : __\b\b");
	scanf_s("%lf", &num2);

	if (A == 1) {
		printf("%lf + %lf = %lf", num1, num2, num1 + num2);
	}
	else if (A == 2) {
		printf("%lf - %lf = %lf", num1, num2, num1 - num2);
	}
	else if (A == 3) {
		printf("%lf * %lf = %lf", num1, num2, num1 * num2);
	}
	else if (A == 4) {
		printf("%lf / %lf = %lf", num1, num2, num1 / num2);
	}

	return 0;
}