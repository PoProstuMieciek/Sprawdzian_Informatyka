#include <iostream>
using namespace std;

// calculates a to the power of n
float power(float a, int n)
{
    if (n < 0) return power(a, n + 1) / a;
    if (n > 0) return power(a, n - 1) * a;
    return 1;
}

int main()
{
    float a;
    int n;

    cin >> a >> n;
    cout << power(a, n) << endl;
}