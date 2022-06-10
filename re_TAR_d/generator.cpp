#include <iostream>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <ctime>

using namespace std;

int main()
{
    char typ = 'M';


    do
    {
        cout << "Rozmiar w (M)ega czy (K)ilo Bajtach?\n";
        cin >> typ;
    }
    while (toupper(typ) != 'M' && toupper(typ) != 'K');

    int licznik;
    if (toupper(typ) == 'K')
        licznik = 1024;
    else
        licznik = 1024 * 1024;

    int rozmiar = -1;

    if (toupper(typ) == 'K')
    {
        do
        {
            cout << "Podaj rozmiar (max 512 000): ";
            cin >> rozmiar;

        }
        while (rozmiar < 1 || rozmiar > 512000);
    }
    else if (toupper(typ) == 'M')
    {
        do
        {
            cout << "Podaj rozmiar (max 500): ";
            cin >> rozmiar;

        }
        while (rozmiar < 1 || rozmiar > 500);
    }


    int liczba_znakow = -1;
    do
    {
        cout << "Podaj liczbe roznych znakow (2 do 128): ";
        cin >> liczba_znakow;

    }
    while (liczba_znakow < 2 || liczba_znakow > 128);

    string maska = "";
    int tab[128] = {NULL};
    srand( time(NULL) );
    for (int i = 32; i < 127; i++)
        tab[i - 32] = i;

    cout << "test";
    for (int i = 0; i < 127; i++)
    {
        for (int j = 32; j < 127; j++)
        {
            int a = rand() % (128 - 32);
            int b = rand() % (128 - 32);

            swap(tab[a], tab[b]);
        }
    }

    for (int i = 32; i < 127; i++)
        maska += char(tab[i]);


    ofstream plik("do_kompresji.txt", ios::out);

    for (int i = 0; i < licznik; i++)
    {
        for (int j = 0; j < rozmiar; j++)
            plik << maska.at( rand() % liczba_znakow);

    }

    plik.close();
    return 0;
}