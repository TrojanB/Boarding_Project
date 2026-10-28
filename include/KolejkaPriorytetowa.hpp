#pragma once

#include "StrategiaObslugi.hpp"
#include "Pasazer.hpp"
#include <vector>

class KolejkaPriorytetowa : public StrategiaObslugi {
private:
    std::vector<Pasazer> kolejka;

    static int pobierzWartoscPriorytetu(const Pasazer& pasazer);

public:
    KolejkaPriorytetowa() = default;
    ~KolejkaPriorytetowa() override = default;

    void dodajPasazera(const Pasazer& pasazer) override;
    Pasazer pobierzNastepnegoPasazera() override;
    bool czyKolejkaPusta() const override;
    size_t pobierzRozmiar() const override;
};