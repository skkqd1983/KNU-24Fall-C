#include <stdio.h>
int fac(int num) {
	int sum = 0;
	if (num == 1) {
		return 1;
	}
	sum = num * fac(num - 1);
	return sum;
}
int main() {
	int num = 0;
	printf("����� ���丮�� �� �Է� : ");
	scanf_s("%d", &num);
	if (num == 0) {
		printf("%d! = %d", num, 1);
	}
	else {
		printf("%d! = %d", num, fac(num));
	} 
}