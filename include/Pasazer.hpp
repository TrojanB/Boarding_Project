#pragma once
#include "Enums.hpp"

class Pasazer {
private:
    int id;
    int idLotu;
    int czasPrzybycia;
    int czasObslugi;
    int priorytet;
    TypPasazera typ;

public:
    Pasazer(int id, int idLotu, int czasPrzybycia, int czasObslugi, int priorytet, TypPasazera typ);

    // Gettery
    int pobierzId() const { return id; }
    int pobierzIdLotu() const { return idLotu; }
    int pobierzCzasPrzybycia() const { return czasPrzybycia; }
    int pobierzCzasObslugi() const { return czasObslugi; }
    int pobierzPriorytet() const { return priorytet; }
    TypPasazera pobierzTyp() const { return typ; }
};