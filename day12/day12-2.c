#include <iostream>
using namespace std;

int solution(int n)
{
    int ans = 0;

    while (n > 0) {               // n이 0이 될 때까지 반복
        if (n % 2 == 1) {         // n이 홀수라면 점프
            ans += 1;
            n -= 1;
        }
        else {                    // 짝수라면 순간이동
            n /= 2;
        }
    }
        
    return ans;
}