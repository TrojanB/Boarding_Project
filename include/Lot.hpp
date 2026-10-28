#pragma once

#include <string>

class Lot {
private:
    int id;
    std::string numer;
    int bramka;
    std::string kierunek;
    int czasOtwarcia;
    int czasZamkniecia;

public:
    Lot() : id(0), bramka(0), czasOtwarcia(0), czasZamkniecia(0) {}

    // Konstruktor dwuargumentowy dla prostego formatu pliku
    Lot(int id, int czasZamkniecia)
        : id(id), numer("N/A"), bramka(0), kierunek("N/A"), czasOtwarcia(0), czasZamkniecia(czasZamkniecia) {}

    // Konstruktor pełny
    Lot(int id, std::string numer, int bramka, std::string kierunek, int otwarcie, int zamkniecie)
        : id(id), numer(numer), bramka(bramka), kierunek(kierunek), czasOtwarcia(otwarcie), czasZamkniecia(zamkniecie) {}

    int pobierzId() const { return id; }
    int pobierzCzasZamkniecia() const { return czasZamkniecia; }
};