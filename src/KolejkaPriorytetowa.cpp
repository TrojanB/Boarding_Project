#include "../include/KolejkaPriorytetowa.hpp"
#include <stdexcept>

int KolejkaPriorytetowa::pobierzWartoscPriorytetu(const Pasazer& pasazer) {
    switch (pasazer.pobierzTyp()) {
        case TypPasazera::NIEPELNOSPRAWNY: return 4;
        case TypPasazera::W_CIAZY:         return 3;
        case TypPasazera::VIP:             return 2;
        case TypPasazera::STANDARDOWY:     return 1;
        default:                           return pasazer.pobierzPriorytet();
    }
}

void KolejkaPriorytetowa::dodajPasazera(const Pasazer& pasazer) {
    kolejka.push_back(pasazer);
}

Pasazer KolejkaPriorytetowa::pobierzNastepnegoPasazera() {
    if (czyKolejkaPusta()) {
        throw std::out_of_range("Błąd: Próba pobrania pasażera z pustej kolejki priorytetowej!");
    }

    // Znajduje pasażera z najwyższą wagą priorytetu (zachowując kolejność FIFO dla równych)
    auto najlepszyIt = kolejka.begin();
    int maxPriorytet = pobierzWartoscPriorytetu(*najlepszyIt);

    for (auto it = kolejka.begin() + 1; it != kolejka.end(); ++it) {
        int priorytet = pobierzWartoscPriorytetu(*it);
        if (priorytet > maxPriorytet) {
            maxPriorytet = priorytet;
            najlepszyIt = it;
        }
    }

    Pasazer wybrany = *najlepszyIt;
    kolejka.erase(najlepszyIt);
    return wybrany;
}

bool KolejkaPriorytetowa::czyKolejkaPusta() const {
    return kolejka.empty();
}

size_t KolejkaPriorytetowa::pobierzRozmiar() const {
    return kolejka.size();
}