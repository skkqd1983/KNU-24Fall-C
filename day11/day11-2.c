#include <stdio.h>
#include <math.h>

double f(double x) {                                    // �Լ� f(x)
    return -log10(1/x) + sin(x);
}

double test(double a, double b, int n) {                // ���� ���
    double num= (b - a) / n;
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        double x = a + num * (i + 0.5);
        sum += f(x);
    }

    return sum * num;                                   // ���� ��ȯ
}

int main() {
    double a, b;
    int max;

    printf("������ ���� ���� �Է��ϼ���: ");
    scanf_s("%lf", &a);
    printf("������ �� ���� �Է��ϼ���: ");
    scanf_s("%lf", &b);
    printf("������ �ִ� ������ �Է��ϼ���(2^n): ");
    scanf_s("%d", &max);

    for (int i = 0; i <= max; i++) {
        int count = 0;
        int n = pow(2, i);                              // 2^i�� �����ϸ� ����
        int n2 = n;
        double result = test(a, b, n);
        printf("\t���� ");

        while (n2 >= 10) {                              // n�� ���̸� �Ǻ�
            n2 = n2 / 10;
            count += 1;
        }

        for (int i = 9-count; i > 0; i--) {             // n�� ���̿� ���� " "�� ����� ��� ��ġ�� ����
            printf(" ");
        }
        printf("%d ���� ���: %.6f\n", n, result);      // ��� ���
    }
    return 0;
}