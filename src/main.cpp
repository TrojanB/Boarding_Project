#include <iostream>
#include <memory>
#include "../include/SymulatorBoardingu.hpp"
#include "../include/KolejkaFIFO.hpp"

int main() {
    try {
        auto kolejkaFIFO = std::make_unique<KolejkaFIFO>();
        SymulatorBoardingu symulator(std::move(kolejkaFIFO));

        // Mechanizm wczytywania konfiguracyjnego
        symulator.wczytajISkonfiguruj("data/konfiguracja.txt", "data/loty.txt", "data/zgloszenia.txt");
        
        // Pętla symulacji zdarzeń dyskretnych (upływ czasu)
        symulator.uruchomSymulacje();
        
        // Zapis wyników do pliku tekstowego
        // ...
        symulator.zakonczIZapiszRaport("data/wyniki.txt", "FIFO"); 
        
        std::cout << "Symulacja FIFO zakonczona. Wygenerowano plik data/wyniki.txt\n";
    } catch (const std::exception& e) {
        std::cerr << "Blad: " << e.what() << '\n';
        return 1;
    }
    return 0;
}