#pragma once

#include "Pasazer.hpp"
#include <cstddef>

class StrategiaObslugi {
public:
    // Wirtualny destruktor niezbędny przy dziedziczeniu
    virtual ~StrategiaObslugi() = default;

    // Czysto wirtualne metody (interfejs do zaimplementowania w klasach pochodnych)
    virtual void dodajPasazera(const Pasazer& pasazer) = 0;
    virtual Pasazer pobierzNastepnegoPasazera() = 0;
    virtual bool czyKolejkaPusta() const = 0;
    virtual size_t pobierzRozmiar() const = 0;
};