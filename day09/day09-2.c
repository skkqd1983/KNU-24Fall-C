#include <stdio.h>
struct Product {
	int ID;
	char name[10];
	int price;
};
struct Product product[5];

void printProduct() {
	printf("\n<<�Էµ� ��ǰ ���>>\n");
	for (int i = 0; i < 5; i++) {
		if (product[i].ID == 0) {
			break;
		}
		else {
			printf("��ǰ ID : %d\n", product[i].ID);
			printf("��ǰ�� : %s\n", product[i].name);
			printf("���� : %d��\n\n", product[i].price);
		}
	}
}

void main() {
	int count = 0;
	for (int i = 0; i < 5; i++) {
		printf("��ǰ ������ �Է��ϼ��� (�Է� �ߴ��� id�� 0 �Է�)\n");
		printf("��ǰ ID : ");
		scanf_s("%d", &product[i].ID);
		if (product[i].ID == 0) {
			printProduct();
			return;
		}
		printf("��ǰ�� : ");
		scanf_s("%s", &product[i].name, 10);
		printf("���� : ");
		scanf_s("%d", &product[i].price);
		printf("\n");
	}
	printProduct();
}

