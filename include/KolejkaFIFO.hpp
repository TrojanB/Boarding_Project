#pragma once

#include "StrategiaObslugi.hpp"
#include "Pasazer.hpp"
#include <queue>

class KolejkaFIFO : public StrategiaObslugi {
private:
    std::queue<Pasazer> kolejka;

public:
    KolejkaFIFO() = default;
    ~KolejkaFIFO() override = default;

    void dodajPasazera(const Pasazer& pasazer) override;
    Pasazer pobierzNastepnegoPasazera() override;
    bool czyKolejkaPusta() const override;
    size_t pobierzRozmiar() const override;
};