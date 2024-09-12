#include <stdio.h> 
int main() {
	int A;
	double num1, num2;

	printf("원히는 기능을 입력하시오.\n1.더하기 2.빼기 3.곱하기 4.나누기\n");

	printf("기능 : _\b");
	scanf_s("%d", &A);

	printf("숫자 1을 입력하세요 : __\b\b");
	scanf_s("%lf", &num1);
	getchar;
	printf("숫자 2를 입력하세요 : __\b\b");
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