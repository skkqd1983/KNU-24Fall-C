#include<stdio.h>
#include<string.h>   // strlen()
#include <stdlib.h>

void Reverse(char* str);

int main() {

    char* rstr = (char*)malloc(sizeof(char) * 100);   // NULL을 포함한 크기(size)를 입력한다.

    printf("문자열을 입력하세요 : ");
    scanf_s("%s", rstr, 100);

    printf("뒤집어진 문자열 : ");
    for (int i = strlen(rstr) - 1; i >= 0; i--) {   // NULL을 제외한 문자열 길이를 초기값으로 준다.
        printf("%c", rstr[i]);
    }
    printf("\n");

    return 0;
}