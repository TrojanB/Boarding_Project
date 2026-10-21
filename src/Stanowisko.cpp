#include "../include/Stanowisko.hpp"

Stanowisko::Stanowisko(int id, double sredniCzas)
    : id(id), sredniCzasObslugi(sredniCzas), czasDoZwolnienia(0), zajete(false) {}

int Stanowisko::pobierzId() const {
    return id;
}

bool Stanowisko::czyZajete() const {
    return zajete;
}

void Stanowisko::przydzielPasazera(const Pasazer& pasazer) {
    zajete = true;
    czasDoZwolnienia = static_cast<int>(sredniCzasObslugi);
}

void Stanowisko::aktualizujCzas() {
    if (zajete) {
        czasDoZwolnienia--;
        if (czasDoZwolnienia <= 0) {
            zajete = false;
            czasDoZwolnienia = 0;
        }
    }
}