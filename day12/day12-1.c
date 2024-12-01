#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// topping_len은 배열 topping의 길이입니다.
int solution(int topping[], size_t topping_len) {
    int answer = 0;

    int left_count[10001] = { 0 };                          // 토핑 왼쪽 부분
    int right_count[10001] = { 0 };                         // 토핑 오른쪽 부분

    int unique_left = 0;                                    // 왼쪽 고유 토핑 개수
    int unique_right = 0;                                   // 오른쪽 고유 토핑 개수

    for (size_t i = 0; i < topping_len; i++) {              // 오른쪽 고유 토핑 개수 초기화
        if (right_count[topping[i]] == 0) {
            unique_right += 1;
        }
        right_count[topping[i]] += 1;
    }

    for (size_t i = 0; i < topping_len - 1; i++) {          // 왼쪽 고유 토핑 추가하며 비교

        if (left_count[topping[i]] == 0) {
            unique_left += 1;
        }
        left_count[topping[i]] += 1;

        right_count[topping[i]] -= 1;                       // 오른쪽으로 이동하며 하나씩 제거
        if (right_count[topping[i]] == 0) {                 // 제거한 토핑이 고유 토핑이라면 -1
            unique_right -= 1;
        }

        if (unique_left == unique_right) {                  // 고유 토핑의 개수가 같다면 answer +1
            answer += 1;
        }
    }

    return answer;
}