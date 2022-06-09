#include <iostream>
using namespace std;

int nwd(int a, int b)
{
    // nwd(a, 0) = a
    if (b == 0) return a;

    // nwd(0, b) = b
    if (a == 0) return b;

    // nwd(a, b) = nwd((b mod a), a)
    return nwd(b % a, a);
}

int main()
{
    int a, b;
    cin >> a >> b;

    cout << nwd(a, b);
}