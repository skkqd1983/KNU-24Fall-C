#include <stdio.h>
int main() {
	char a = 'A';
	int num = 356;

	char* ptr1 = &a;
	int* ptr2 = &num;

	printf("%c\n", a);
	printf("%d\n", num);

	printf("%x\n", ptr1);
	printf("%x\n", ptr2);

	printf("%c\n", *ptr1);
	printf("%d\n", *ptr2);

	char *c1, *c2;
	printf("%d\n", sizeof(c1));
	printf("%d\n", sizeof(c2));



	printf("\n\n");



	int num1 = 100, num2 = 100;
	int* pnum;

	pnum = &num1;
	(*pnum) += 30;

	pnum = &num2;
	(*pnum) -= 30;

	printf("num1 : %d\n", num1);
	printf("num2 : %d\n", num2);



	printf("\n\n");



	int arr[3] = {1, 2, 3};
	for (int i = 0; i < 3; i++) {
		printf("%d\n", *(arr+i)); // *(arr+1) => *(arr+i)
	}

	return 0;
}