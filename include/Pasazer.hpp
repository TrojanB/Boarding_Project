#pragma once
#include "./Enums.hpp"

class Pasazer{
    private:
    int id;
    int idLotu;
    int czasPrzybycia;
    int czasSkanowania;
    TypPasazera typ;

    public:
    Pasazer(int id, int idLotu, int czasPrzybycia, int czasSkanowania, TypPasazera typ)
        : id(id), idLotu(idLotu), czasPrzybycia(czasPrzybycia), 
        czasSkanowania(czasSkanowania), typ(typ) {}

    int pobierzIdLotu() const { return idLotu; }
    int pobierzCzasPrzybycia() const { return czasPrzybycia; }
    int pobierzCzasSkanowania() const {return czasSkanowania;}
    TypPasazera pobierzTyp() const { return typ; }
};