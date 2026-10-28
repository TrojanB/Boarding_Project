#pragma once

#include <vector>
#include <map>
#include <memory>
#include <string>

#include "Stanowisko.hpp"
#include "Lot.hpp"
#include "Pasazer.hpp"
#include "StrategiaObslugi.hpp"
#include "Statystyki.hpp"

class SymulatorBoardingu {
private:
    int aktualnyCzas;
    std::vector<Stanowisko> stanowiska;
    std::map<int, Lot> lotyMap;
    std::vector<Pasazer> nadchodzacyPasazerowie;
    std::unique_ptr<StrategiaObslugi> strategia;
    Statystyki statystyki;

    void wykonajKrok();
    bool czyJakiesStanowiskoZajete() const;

public:
    explicit SymulatorBoardingu(std::unique_ptr<StrategiaObslugi> wybranaStrategia);

    void wczytajISkonfiguruj(const std::string& plikKonfig, const std::string& plikLoty, const std::string& plikZgloszenia);
    void uruchomSymulacje();
    void zakonczIZapiszRaport(const std::string& plikWyjsciowy, const std::string& nazwaStrategii);

    // Gettery potrzebne do raportu porównawczego
    const Statystyki& pobierzStatystyki() const { return statystyki; }
    int pobierzAktualnyCzas() const { return aktualnyCzas; }
};