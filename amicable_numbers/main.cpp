#include <iostream>
using namespace std;

int suma_dzielnikow_wlasciwych(int a)
{
    int sum = 0;

    // dzielniki mniejsze od niej samej
    for (size_t i = 1; i < a; i++)
    {
        // jeśli a dzieli się przez i
        if (a % i == 0)
        {
            // dodaj ten dzielnik do sumy
            sum += i;
        }
    }

    return sum;
}

bool are_amicable_numbers(int a, int b)
{
    // z definicji: para różnych liczb
    if (a == b) return false;

    int suma_dzielnikow_a = suma_dzielnikow_wlasciwych(a);
    int suma_dzielnikow_b = suma_dzielnikow_wlasciwych(b);

    return (suma_dzielnikow_a == b) && (suma_dzielnikow_b == a);
}

int main()
{
    int a, b;
    cin >> a >> b;

    if (are_amicable_numbers(a, b))
    {
        cout << "Liczby są zaprzyjaźnione! Woohoo!" << endl;
    }
    else
    {
        cout << "Liczby nie są zaprzyjaźnione." << endl;
    }
}