#include<stdio.h>
#include<string.h>   // strlen()
#include <stdlib.h>

void Reverse(char* str);

int main() {

    char* rstr = (char*)malloc(sizeof(char) * 100);   // NULL�� ������ ũ��(size)�� �Է��Ѵ�.

    printf("���ڿ��� �Է��ϼ��� : ");
    scanf_s("%s", rstr, 100);

    printf("�������� ���ڿ� : ");
    for (int i = strlen(rstr) - 1; i >= 0; i--) {   // NULL�� ������ ���ڿ� ���̸� �ʱⰪ���� �ش�.
        printf("%c", rstr[i]);
    }
    printf("\n");

    return 0;
}