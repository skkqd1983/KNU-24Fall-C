#include <stdio.h>
#include <math.h>

double f(double x) {                                    // 함수 f(x)
    return -log10(1/x) + sin(x);
}

double test(double a, double b, int n) {                // 적분 계산
    double num= (b - a) / n;
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        double x = a + num * (i + 0.5);
        sum += f(x);
    }

    return sum * num;                                   // 면적 반환
}

int main() {
    double a, b;
    int max;

    printf("적분할 시작 값을 입력하세요: ");
    scanf_s("%lf", &a);
    printf("적분할 끝 값을 입력하세요: ");
    scanf_s("%lf", &b);
    printf("실행할 최대 구간을 입력하세요(2^n): ");
    scanf_s("%d", &max);

    for (int i = 0; i <= max; i++) {
        int count = 0;
        int n = pow(2, i);                              // 2^i로 증가하며 적분
        int n2 = n;
        double result = test(a, b, n);
        printf("\t구간 ");

        while (n2 >= 10) {                              // n의 길이를 판별
            n2 = n2 / 10;
            count += 1;
        }

        for (int i = 9-count; i > 0; i--) {             // n의 길이에 따라 " "을 출력해 출력 위치를 정렬
            printf(" ");
        }
        printf("%d 적분 결과: %.6f\n", n, result);      // 결과 출력
    }
    return 0;
}