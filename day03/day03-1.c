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
	printf("�Ҽ����� Ȯ���� ���� �Է��ϼ��� : ");
	scanf_s("%d", &num);
	if (p_num_test(num) == 1) {
		printf("�ش� ���� �Ҽ��Դϴ�.");
	}
	else {
		printf("�ش� ���� �Ҽ��� �ƴմϴ�.");
	}
	return 0;
}