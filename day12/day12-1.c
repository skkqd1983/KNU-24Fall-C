#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// topping_len�� �迭 topping�� �����Դϴ�.
int solution(int topping[], size_t topping_len) {
    int answer = 0;

    int left_count[10001] = { 0 };                          // ���� ���� �κ�
    int right_count[10001] = { 0 };                         // ���� ������ �κ�

    int unique_left = 0;                                    // ���� ���� ���� ����
    int unique_right = 0;                                   // ������ ���� ���� ����

    for (size_t i = 0; i < topping_len; i++) {              // ������ ���� ���� ���� �ʱ�ȭ
        if (right_count[topping[i]] == 0) {
            unique_right += 1;
        }
        right_count[topping[i]] += 1;
    }

    for (size_t i = 0; i < topping_len - 1; i++) {          // ���� ���� ���� �߰��ϸ� ��

        if (left_count[topping[i]] == 0) {
            unique_left += 1;
        }
        left_count[topping[i]] += 1;

        right_count[topping[i]] -= 1;                       // ���������� �̵��ϸ� �ϳ��� ����
        if (right_count[topping[i]] == 0) {                 // ������ ������ ���� �����̶�� -1
            unique_right -= 1;
        }

        if (unique_left == unique_right) {                  // ���� ������ ������ ���ٸ� answer +1
            answer += 1;
        }
    }

    return answer;
}