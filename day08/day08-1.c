#include<stdio.h>
#include<string.h>
#include <stdlib.h>

int main() {

    char* rstr = (char*)malloc(sizeof(char) * 100); 
    char str;

    printf("���ڿ��� �Է��ϼ��� : ");
    scanf_s("%s", rstr, 100);

    printf("�������� ���ڿ� : ");
    for (int i = 0; i < strlen(rstr) / 2; i++) {
        str = rstr[strlen(rstr) - i -1];
        rstr[strlen(rstr) - i - 1] = rstr[i];
        rstr[i] = str;
    }
    printf("%s\n", rstr);

    return 0;
}