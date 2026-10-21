#pragma once

#include <string>
#include <vector>

#include "Stanowisko.hpp"
#include "Lot.hpp"
#include "Pasazer.hpp"
#include "Statystyki.hpp"

class MenedzerPlikow {
public:
    MenedzerPlikow() = default;

    std::vector<Stanowisko> odczytajKonfiguracje(const std::string& sciezka);
    std::vector<Lot> odczytajLoty(const std::string& sciezka);
    std::vector<Pasazer> odczytajZgloszenia(const std::string& sciezka);

    // Zaktualizowana sygnatura
    void zapiszWyniki(const std::string& sciezka, const Statystyki& statystyki, int calkowityCzas, const std::string& nazwaStrategii);
};