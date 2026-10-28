#pragma once

#include <string>
#include <vector>
#include "Stanowisko.hpp"
#include "Lot.hpp"
#include "Pasazer.hpp"
#include "Statystyki.hpp"

class MenedzerPlikow {
public:
    std::vector<Stanowisko> odczytajKonfiguracje(const std::string& sciezka);
    std::vector<Lot> odczytajLoty(const std::string& sciezka);
    std::vector<Pasazer> odczytajZgloszenia(const std::string& sciezka);

    void zapiszWyniki(const std::string& sciezka, const Statystyki& statystyki, int calkowityCzas, const std::string& nazwaStrategii);
    
    // Deklaracja metody do raportu porównawczego
    void zapiszWynikiPorownawcze(const std::string& sciezka, 
                                 const Statystyki& statFifo, int czasFifo,
                                 const Statystyki& statPrio, int czasPrio);
};