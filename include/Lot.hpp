#pragma once
#include <string>

class Lot {
private:
    int idLotu;
    std::string numerLotu;
    int idBramki;
    std::string kierunek;
    int czasOtwarcia;
    int czasZamkniecia;

public:
    Lot(int id, std::string numer, int bramka, std::string kierunek, int otwarcie, int zamkniecie)
        : idLotu(id), numerLotu(std::move(numer)), idBramki(bramka),
          kierunek(std::move(kierunek)), czasOtwarcia(otwarcie), czasZamkniecia(zamkniecie) {}

    // Gettery
    int pobierzId() const { return idLotu; }
    int pobierzIdBramki() const { return idBramki; }
    std::string pobierzNrLotu() const {return numerLotu;}
    std::string pobierzKierunek() const {return kierunek;}
    int pobierzCzasOtwarcia() const { return czasOtwarcia; }
    int pobierzCzasZamkniecia() const { return czasZamkniecia; }

    // Metoda pomocnicza
    bool czyBramkaOtwarta(int aktualnyCzas) const {
        return aktualnyCzas >= czasOtwarcia && aktualnyCzas <= czasZamkniecia;
    }   
};