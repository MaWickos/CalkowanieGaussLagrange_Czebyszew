/* Rozklad wezlow i wag : https://pomax.github.io/bezierinfo/legendre-gauss.html?fbclid=IwAR2M1n6UZetCfcVnkN1mdH9R_mJX3avA8wCdQrTvMRXhbKqdR-Gxe9DQano */
#include <iostream>
#include <cmath>
#include <sstream>      //obsluga plikow
#include <fstream>
#include <string>

#define PI 3.14159265359
#define MAX_WEZLOW 500
#define MAX_GaussLagrange 20
using namespace std;

/********************************PROTOTYPY********************************/
bool sprawdzCzyWprowadzono(bool);
void wczytajLiczbe(double&, bool&);
long double pochodna_ln(long double);

void menu_GaussLagrange(double);
long double LN_GaussLagrange(long double*, long double*, double, int);
bool wczytajDane_GaussLagrange(long double*, long  double*, int);

void menu_GaussCzebyszow(double);
long double LN_GaussCzebyszow(double, int);

/********************************MAIN********************************/
int main()
{
    //deklaracja zmiennych
    int wybor;
    double liczba;
    bool czyWprowadzono=false;

    //menu
    do
    {
        cout << "Calkowanie numeryczne metodami wyzszej precyzji." << endl;
        cout << "MENU" << endl;
        cout << "1) Wprowadz liczbe logarytmowana," << endl;
        cout << "2) Metoda Gaussa-Lagrange'a," << endl;
        cout << "3) Metoda Gaussa-Czebyszowa," << endl;
        cout << "4) Wyjscie z programu." << endl;
        cout << "Wybieram -> "; cin >> wybor;

        switch(wybor)
        {
            case 1: wczytajLiczbe(liczba, czyWprowadzono); break;
            case 2: if(sprawdzCzyWprowadzono(czyWprowadzono)) menu_GaussLagrange(liczba); break;
            case 3: if(sprawdzCzyWprowadzono(czyWprowadzono)) menu_GaussCzebyszow(liczba); break;
            case 4: exit(EXIT_SUCCESS); break;
        }

        system("cls");
    }
    while(wybor!=4);

    return 0;
}

/********************************FUNKCJE********************************/
bool sprawdzCzyWprowadzono(bool czyWprowadzono)
{
    if(czyWprowadzono)
        return true;
    else
    {
        cout << endl;
        cout << "***Sprawdzenie poprawnosci danych***" << endl;
        cout << "Nie wprowadzono liczby! Nie wiem jaki logarytm chcesz obliczyc!" << endl;
        cout << "Wprowadz liczbe a nastepnie wybierz metode." << endl;
        cout << endl;
        system("pause");
        return false;
    }
}
void wczytajLiczbe(double &liczba, bool &czyWprowadzono)
{
    system("cls");
    cout << "***MENU - Wczytywanie liczby***" << endl;
    cout << "Logarytm naturalny jakiej liczby chcesz obliczyc?" << endl;

    do
    {
        cout << "x = "; cin >> liczba;

        if(liczba<=0)
            cout << "Nie istnieje logarytm naturalny z liczby niedodatniej! \nWprowadz liczbe jeszcze raz.\n" << endl;

    }
    while(liczba<=0);

    cout << "Zakonczono wprowadzanie danych." << endl;
    czyWprowadzono=true;
    system("pause");

}

void menu_GaussLagrange(double liczba)
{
    int ileWezlow;

    system("cls");
    cout << "***MENU - Metoda Gaussa-Lagrange'a***" << endl;
    cout << "Ilu wezlow chcesz uzyc do wyliczenia calki (max " << MAX_GaussLagrange << ")?" << endl;

    //wprowadznie wezlow
    do
    {
        cout << "Liczba wezlow = "; cin >> ileWezlow;

        if(ileWezlow<=0 || ileWezlow>MAX_GaussLagrange)
            cout << "Wprowadzono niepoprawna liczbe wezlow. \nWprowadz liczbe jeszcze raz.\n" << endl;
    }
    while(ileWezlow<=0 || ileWezlow>MAX_GaussLagrange);

    cout << "Wprowadzono liczbe wezlow." << endl;
    cout << "Prosze czekac trwa wczytywanie pliku z danymi..." << endl;

    //wczytanie pliku
    long double *wezly = new long double[ileWezlow];
    long double *wartosci = new long double[ileWezlow];

    if(!wczytajDane_GaussLagrange(wezly, wartosci, ileWezlow))
    {
        cout << "Wczytanie pliku nie powiodlo sie. Wracam do menu!" << endl;
        system("pause");
    }
    else
    {
        cout << "Plik wczytany prawidlowo!" << endl;

        cout << endl;
        cout << "******" << endl;
        cout << "Wynik dzialania programu: " << endl;
            cout << "\tln(" << liczba << ")= " << LN_GaussLagrange(wezly, wartosci, liczba, ileWezlow) << endl;
        cout << "Wartosc rzeczywista:" << endl;
            cout << "\tln(" << liczba << ")= " << log(liczba) << endl;
        cout << endl;

        system("pause");
    }

    delete [] wezly;
    delete [] wartosci;
}

long double LN_GaussLagrange(long double *wezly, long double *wartosci, double argument, int ileWezlow)
{
    long double suma=0;
    long double zMinusem=(argument-1)/2;
    long double zPlusem=(argument+1)/2;

    for(int i=0; i<ileWezlow; i++)
        suma+=pochodna_ln(zMinusem*wezly[i]+zPlusem)*wartosci[i];

    return zMinusem*suma;
}

long double pochodna_ln(long double x)
{
    return 1/x;
}

bool wczytajDane_GaussLagrange(long double *wezly, long double *wartosci, int ileWezlowWczytac)
{
    string sciezka;
    int maxLiczbaWezlow, ileWezlow;
    char sredniki;
    ifstream dane;

    sciezka="GQ_WeightsAbscissae.csv";

    //otworzenie pliku
    dane.open(sciezka);

    dane >> maxLiczbaWezlow;
    dane >> sredniki;

    if(ileWezlowWczytac<=maxLiczbaWezlow)
    {
        string temp_wezel, temp_wartosc, temp_ileWezlow, linia;
        do
        {
            dane >> temp_ileWezlow;
            ileWezlow=atof(temp_ileWezlow.c_str());

            for(int i=0; i<ileWezlow; i++)
            {
                dane >> linia;
                istringstream ss(linia);
                getline(ss, temp_wezel, ';');
                getline(ss, temp_wartosc, ';');

                wezly[i]=atof(temp_wezel.c_str());
                wartosci[i]=atof(temp_wartosc.c_str());
            }
        }
        while(ileWezlow!=ileWezlowWczytac);

        dane.close();
        return true;
    }
    else
    {
        dane.close();
        return false;
    }
}

void menu_GaussCzebyszow(double liczba)
{
    int ileWezlow;

    system("cls");
    cout << "***MENU - Metoda Gaussa Czebyszowa" << endl;
    cout << "Ilu wezlow chcesz uzyc do wyliczenia calki?" << endl;

    //wprowadznie danych
    do
    {
        cout << "Liczba wezlow = "; cin >> ileWezlow;

        if(ileWezlow<=0 || ileWezlow>MAX_WEZLOW)
            cout << "Wprowadzono niepoprawna liczbe wezlow. \nWprowadz liczbe jeszcze raz.\n" << endl;
    }
    while(ileWezlow<=0 || ileWezlow>MAX_WEZLOW);

    cout << "Wprowadzono liczbe wezlow." << endl;

    //obliczenie calki
    long double *wezly = new long double[ileWezlow];
    long double *wartosci = new long double[ileWezlow];

    cout << endl;
    cout << "******" << endl;
    cout << "Wynik dzialania programu: " << endl;
        cout << "\tln(" << liczba << ")= " << LN_GaussCzebyszow(liczba, ileWezlow) << endl;
    cout << "Wartosc rzeczywista:" << endl;
        cout << "\tln(" << liczba << ")= " << log(liczba) << endl;
    cout << endl;

        system("pause");

    delete [] wezly;
    delete [] wartosci;
}

long double LN_GaussCzebyszow(double argument, int ileWezlow)
{
    double waga=PI/ileWezlow;
    double zMinusem=(argument-1)/2;
    double zPlusem=(argument+1)/2;
    long double suma=0;
    long double t;

    for(int i=0; i<ileWezlow; i++)
    {
        t=cos(((2*i-1)*PI)/(2*ileWezlow));
        suma+=pochodna_ln(zMinusem*t+zPlusem)*sqrt(1-(t*t))*waga;
    }

    return zMinusem*suma;
}
