#pragma once
#include "Pasazer.hpp"
#include "Enums.hpp"
#include <vector>
#include <map>

struct LogPasazera {
    int idPasazera;
    TypPasazera typ;
    int idLotu;
    int czasPrzybycia;
    int czasRozpoczeciaObslugi;
    int czasCzekania;
    bool czyObsluzony;
    bool czySpozniony;
};

class Statystyki {
private:
    int iloscObsluzonych;
    int iloscSpoznionych;
    std::vector<LogPasazera> historiaPasazerow;
    std::map<int, int> czasPracyStanowisk;

public:
    Statystyki();
    
    void zanotujObsluge(const Pasazer& pasazer, int czasRozpoczeciaObslugi);
    void zanotujSpoznienie(const Pasazer& pasazer);
    void zanotujPraceStanowiska(int idStanowiska);

    int pobierzIloscObsluzonych() const;
    int pobierzIloscSpoznionych() const;
    const std::vector<LogPasazera>& pobierzHistorie() const;

    // Metody analityczne wywoływane przez MenedzerPlikow
    double obliczSredniCzasCzekaniaDlaTypu(TypPasazera typ) const;
    double obliczSredniCzasCzekania() const;
    int pobierzNajdluzszyCzasCzekania() const;
    const std::map<int, int>& pobierzCzasPracyStanowisk() const;
    double obliczWykorzystanieStanowiska(int idStanowiska, int calkowityCzas) const;
};