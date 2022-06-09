#include <iostream>
#include <fstream> // obsługa plików
using namespace std;

long long silnia(long long a)
{
    if (a == 0) return 1;

    return a * silnia(a - 1);
}

long long symbol_newtona(int n, int k)
{
    return ( silnia(n) ) / ( silnia(k) * silnia(n - k) );
}

void zapisz_do_pliku(int wiersze)
{
    ofstream plik("trojkat.txt");

    for (int i = 0; i < wiersze; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            plik << symbol_newtona(i, j) << " ";
        }
        plik << endl;
    }

    // pamiętamy, aby koniecznie zamknąć plik
    plik.close();
}

int main()
{
    int wiersze;
    cin >> wiersze;

    zapisz_do_pliku(wiersze);
}