#include <iostream>
#include <memory>

#include "../include/SymulatorBoardingu.hpp"
#include "../include/KolejkaFIFO.hpp"
#include "../include/KolejkaPriorytetowa.hpp"
#include "../include/MenedzerPlikow.hpp"

int main() {
    const std::string plikKonfig = "data/konfiguracja.txt";
    const std::string plikLoty = "data/loty.txt";
    const std::string plikZgloszenia = "data/zgloszenia_duze.txt";
    const std::string plikWynikiZbiorcze = "data/wyniki_duze.txt";

    try {
        // ---------------------------------------------------------------------
        // SYMULACJA 1: Strategia FIFO
        // ---------------------------------------------------------------------
        std::cout << "--- Uruchamianie symulacji: Strategia FIFO ---" << std::endl;
        auto strategiaFifo = std::make_unique<KolejkaFIFO>();
        SymulatorBoardingu symulatorFifo(std::move(strategiaFifo));

        symulatorFifo.wczytajISkonfiguruj(plikKonfig, plikLoty, plikZgloszenia);
        symulatorFifo.uruchomSymulacje();
        symulatorFifo.zakonczIZapiszRaport("data/wyniki_fifo.txt", "FIFO");
        std::cout << "-> Zapisano szczegolowy raport: data/wyniki_fifo.txt\n\n";

        // ---------------------------------------------------------------------
        // SYMULACJA 2: Strategia Priorytetowa
        // ---------------------------------------------------------------------
        std::cout << "--- Uruchamianie symulacji: Strategia PRIORYTETOWA ---" << std::endl;
        auto strategiaPrio = std::make_unique<KolejkaPriorytetowa>();
        SymulatorBoardingu symulatorPrio(std::move(strategiaPrio));

        symulatorPrio.wczytajISkonfiguruj(plikKonfig, plikLoty, plikZgloszenia);
        symulatorPrio.uruchomSymulacje();
        symulatorPrio.zakonczIZapiszRaport("data/wyniki_priorytet.txt", "PRIORYTETOWA");
        std::cout << "-> Zapisano szczegolowy raport: data/wyniki_priorytet.txt\n\n";

        // ---------------------------------------------------------------------
        // RAPORT PORÓWNAWCZY (data/wyniki.txt)
        // ---------------------------------------------------------------------
        MenedzerPlikow menedzer;
        menedzer.zapiszWynikiPorownawcze(
            plikWynikiZbiorcze,
            symulatorFifo.pobierzStatystyki(), symulatorFifo.pobierzAktualnyCzas(),
            symulatorPrio.pobierzStatystyki(), symulatorPrio.pobierzAktualnyCzas()
        );
        std::cout << "-> Zapisano raport porownawczy obu strategii: " << plikWynikiZbiorcze << "\n\n";

        std::cout << "Wszystkie symulacje oraz raport porownawczy zostaly wygenerowane!" << std::endl;
    } 
    catch (const std::exception& e) {
        std::cerr << "Blad podczas wykonywania symulacji: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}