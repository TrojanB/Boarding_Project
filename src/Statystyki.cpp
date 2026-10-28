#include "Statystyki.hpp"

Statystyki::Statystyki() : iloscObsluzonych(0), iloscSpoznionych(0) {}

void Statystyki::zanotujObsluge(const Pasazer& pasazer, int czasRozpoczeciaObslugi) {
    iloscObsluzonych++;
    int czasCzekania = czasRozpoczeciaObslugi - pasazer.pobierzCzasPrzybycia();
    if (czasCzekania < 0) czasCzekania = 0;

    historiaPasazerow.push_back({
        pasazer.pobierzId(),
        pasazer.pobierzTyp(),
        pasazer.pobierzIdLotu(),
        pasazer.pobierzCzasPrzybycia(),
        czasRozpoczeciaObslugi,
        czasCzekania,
        true,
        false
    });
}

void Statystyki::zanotujSpoznienie(const Pasazer& pasazer) {
    iloscSpoznionych++;
    historiaPasazerow.push_back({
        pasazer.pobierzId(),
        pasazer.pobierzTyp(),
        pasazer.pobierzIdLotu(),
        pasazer.pobierzCzasPrzybycia(),
        -1,
        0,
        false,
        true
    });
}

void Statystyki::zanotujPraceStanowiska(int idStanowiska) {
    czasPracyStanowisk[idStanowiska]++;
}

int Statystyki::pobierzIloscObsluzonych() const { return iloscObsluzonych; }
int Statystyki::pobierzIloscSpoznionych() const { return iloscSpoznionych; }
const std::vector<LogPasazera>& Statystyki::pobierzHistorie() const { return historiaPasazerow; }

double Statystyki::obliczSredniCzasCzekaniaDlaTypu(TypPasazera typ) const {
    int suma = 0;
    int ilosc = 0;
    for (const auto& log : historiaPasazerow) {
        if (log.typ == typ && log.czyObsluzony) {
            suma += log.czasCzekania;
            ilosc++;
        }
    }
    return ilosc > 0 ? static_cast<double>(suma) / ilosc : 0.0;
}

double Statystyki::obliczSredniCzasCzekania() const {
    int suma = 0;
    int ilosc = 0;
    for (const auto& log : historiaPasazerow) {
        if (log.czyObsluzony) {
            suma += log.czasCzekania;
            ilosc++;
        }
    }
    return ilosc > 0 ? static_cast<double>(suma) / ilosc : 0.0;
}

int Statystyki::pobierzNajdluzszyCzasCzekania() const {
    int maxCzas = 0;
    for (const auto& log : historiaPasazerow) {
        if (log.czyObsluzony && log.czasCzekania > maxCzas) {
            maxCzas = log.czasCzekania;
        }
    }
    return maxCzas;
}

const std::map<int, int>& Statystyki::pobierzCzasPracyStanowisk() const {
    return czasPracyStanowisk;
}

double Statystyki::obliczWykorzystanieStanowiska(int idStanowiska, int calkowityCzas) const {
    if (calkowityCzas == 0) return 0.0;
    auto it = czasPracyStanowisk.find(idStanowiska);
    if (it != czasPracyStanowisk.end()) {
        return (static_cast<double>(it->second) / calkowityCzas) * 100.0;
    }
    return 0.0;
}

int Statystyki::pobierzIloscDlaTypu(TypPasazera typ) const {
    int licznik = 0;
    for (const auto& log : historiaPasazerow) {
        if (log.typ == typ) {
            licznik++;
        }
    }
    return licznik;
}