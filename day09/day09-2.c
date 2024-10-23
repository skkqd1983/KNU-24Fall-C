#include <stdio.h>
struct Product {
	int ID;
	char name[10];
	int price;
};
struct Product product[5];

void printProduct() {
	printf("\n<<입력된 상품 목록>>\n");
	for (int i = 0; i < 5; i++) {
		if (product[i].ID == 0) {
			break;
		}
		else {
			printf("상품 ID : %d\n", product[i].ID);
			printf("상품명 : %s\n", product[i].name);
			printf("가격 : %d원\n\n", product[i].price);
		}
	}
}

void main() {
	int count = 0;
	for (int i = 0; i < 5; i++) {
		printf("상품 정보를 입력하세요 (입력 중단은 id에 0 입력)\n");
		printf("상품 ID : ");
		scanf_s("%d", &product[i].ID);
		if (product[i].ID == 0) {
			printProduct();
			return;
		}
		printf("상품명 : ");
		scanf_s("%s", &product[i].name, 10);
		printf("가격 : ");
		scanf_s("%d", &product[i].price);
		printf("\n");
	}
	printProduct();
}

