#pragma once
#include "Pasazer.hpp"

class Stanowisko {
private:
    int id;
    double sredniCzasObslugi;
    int czasDoZwolnienia;
    bool zajete;

public:
    Stanowisko(int id, double sredniCzas);
    
    int pobierzId() const;
    bool czyZajete() const;
    void przydzielPasazera(const Pasazer& pasazer);
    void aktualizujCzas();
};