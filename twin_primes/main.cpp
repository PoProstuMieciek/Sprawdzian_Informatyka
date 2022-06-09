#include <iostream>
using namespace std;

// maksymalna wartość, bo deklarujemy tablicę, musimy mieć jakieś ograniczenie
const int MAX_N = 1e6;

// początkowo wypełniona false
// true - liczby złożone, wykreślone
// false - liczby pierwsze, niewykreślone
bool sito[MAX_N];

void generuj_sito(int n = MAX_N)
{
    // do pierwiastka
    for (int i = 2; i * i < n; i++)
    {
        // jeśli niewykreślone
        if (sito[i] == false)
        {
            // wykreśl to i wszystkie wielokrotności
            for (int j = 2 * i; j < n; j += i)
            {
                sito[j] = true;
            }
        }
    }

    // liczby pierwsze to takie które są zaznaczone w tablicy sito[] jako false
}

bool jest_pierwsza(int liczba)
{
    return sito[liczba] == false;
}

void liczby_blizniacze(int max)
{
    for (int i = 2; i < max; i++)
    {
        int liczba_1 = i;
        int liczba_2 = i + 2;

        if (jest_pierwsza(liczba_1) && jest_pierwsza(liczba_2))
        {
            cout << liczba_1 << " i " << liczba_2 << " to liczby bliźniacze" << endl;
        }
    }
}

int main()
{
    int n;
    cin >> n;

    // dodatkowe sprawdzenie dla chętnych, można wywalić
    if (n > MAX_N)
    {
        cout << "za duża liczba" << endl;
        return 1;
    }
    // koniec: dodatkowe sprawdzenie dla chętnych, można wywalić

    // generujemy sito trochę większe, just because
    generuj_sito(n + 10);

    liczby_blizniacze(n);

    return 0;
}