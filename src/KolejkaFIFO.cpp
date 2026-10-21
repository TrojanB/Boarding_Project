#include "../include/KolejkaFIFO.hpp"
#include <stdexcept>

// Dodaje pasażera na koniec kolejki (zgodnie z zasadą First-Come)
void KolejkaFIFO::dodajPasazera(const Pasazer& pasazer) {
    kolejka.push(pasazer);
}

// Pobiera i usuwa pierwszego pasażera z przodu kolejki
Pasazer KolejkaFIFO::pobierzNastepnegoPasazera() {
    if (czyKolejkaPusta()) {
        throw std::out_of_range("Błąd: Próba pobrania pasażera z pustej kolejki FIFO!");
    }

    Pasazer nastepny = kolejka.front(); // Pobieramy tego z samego przodu
    kolejka.pop();                      // Usuwamy go z kolejki
    
    return nastepny;
}

// Sprawdza, czy ktoś jeszcze czeka w kolejce
bool KolejkaFIFO::czyKolejkaPusta() const {
    return kolejka.empty();
}

// Zwraca aktualną liczbę oczekujących pasażerów
size_t KolejkaFIFO::pobierzRozmiar() const {
    return kolejka.size();
}