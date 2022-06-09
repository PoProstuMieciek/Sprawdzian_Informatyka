#include <iostream>
#include <cmath> // sqrt, acos
using namespace std;

// radiany = stopnie * π / 180
// stopnie = radiany * 180 / π
float radiany_na_stopnie(float radiany)
{
    return radiany * 180 / M_PI;
}

void rozwiaz_trojkat(float a, float b, float c)
{
    // warunek istnienia trójkąta
    bool trojkat_istnieje = (a + b > c) && (b + c > a) && (a + c > b);
    if (!trojkat_istnieje)
    {
        cout << "Trójkąt o podanych bokach nie istnieje!" << endl;
        return;
    }

    // obwód
    float obwod = a + b + c;

    cout << "Obwód: " << obwod << endl;

    // pole (ze wzoru Herona)
    float polowa_obwodu = obwod / 2;
    float pole = sqrt(polowa_obwodu * (polowa_obwodu - a) * (polowa_obwodu - b) * (polowa_obwodu - c));

    cout << "Pole: " << pole << endl;

    // kąty (z twierdzenia cosinusów)
    /*
        przekształcenia wzorów:

        a² = b² + c² - 2bccosɑ
        b² = a² + c² - 2accosβ
        c² = a² + b² - 2abcosɣ

        2bccosɑ = b² + c² - a²
        2accosβ = a² + c² - b²
        2abcosɣ = a² + b² - c²

        cosɑ = (b² + c² - a²) / 2bc
        cosβ = (a² + c² - b²) / 2ac
        cosɣ = (a² + b² - c²) / 2ab

        ɑ = arccos(cosɑ)
        β = arccos(cosβ)
        ɣ = arccos(cosɣ)
    */

    // zmienne możemy nazywać jak chcemy, możemy używać znaków specjalnych
    // SYMBOL ² TO NIE OPERATOR POTĘGOWANIA, TO TYLKO CZĘŚĆ NAZWY ZMIENNEJ
    // W C++ NIE ISTNIEJE OPERATOR POTĘGOWANIA
    float a² = a * a;
    float b² = b * b;
    float c² = c * c;

    float cos_alpha = (b² + c² - a²) / (2 * b * c);
    float cos_beta = (a² + c² - b²) / (2 * a * c);
    float cos_gamma = (a² + b² - c²) / (2 * a * b);

    // arccos to funkcja acos z <cmath>, funkcje zwracają wynik w radianach
    float alpha_rad = acos(cos_alpha);
    float beta_rad = acos(cos_beta);
    float gamma_rad = acos(cos_gamma);

    float alpha = radiany_na_stopnie(alpha_rad);
    float beta = radiany_na_stopnie(beta_rad);
    float gamma = radiany_na_stopnie(gamma_rad);

    cout << "alpha: " << alpha << endl;
    cout << "beta: " << beta << endl;
    cout << "gamma: " << gamma << endl;

    // rodzaj trójkąta
    // musimy zastosować zaokrąglenie, ponieważ porównywanie liczb zmiennoprzecinkowych nie jest takie proste w programowaniu
    if (round(alpha) == 90 || round(beta) == 90 || round(gamma) == 90)
    {
        cout << "Trójkąt prostokątny" << endl;
    }
    else if (alpha < 90 || beta < 90 || gamma < 90)
    {
        cout << "Trójkąt ostokątny" << endl;
    }
    else
    {
        cout << "Trójkąt rozwarty" << endl;
    }
}

int main()
{
    float a, b, c;
    cin >> a >> b >> c;

    rozwiaz_trojkat(a, b, c);
}