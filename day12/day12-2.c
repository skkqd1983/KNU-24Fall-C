#include <iostream>
using namespace std;

int solution(int n)
{
    int ans = 0;

    while (n > 0) {               // n�� 0�� �� ������ �ݺ�
        if (n % 2 == 1) {         // n�� Ȧ����� ����
            ans += 1;
            n -= 1;
        }
        else {                    // ¦����� �����̵�
            n /= 2;
        }
    }
        
    return ans;
}