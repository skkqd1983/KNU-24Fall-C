#include <stdio.h>

int p_num_test(int num) {
	for (int i = 2; i < num; i++) {
		if (num % i == 0) {
			return 0;
		}
	}
	return 1;
}

int main() {
	int num = 0;
	printf("소수인지 확인할 수를 입력하세요 : ");
	scanf_s("%d", &num);
	if (p_num_test(num) == 1) {
		printf("해당 수는 소수입니다.");
	}
	else {
		printf("해당 수는 소수가 아닙니다.");
	}
	return 0;
}