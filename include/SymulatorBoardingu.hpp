    #pragma once

    #include "StrategiaObslugi.hpp"
    #include "Stanowisko.hpp"
    #include "Lot.hpp"
    #include "Pasazer.hpp"
    #include "Statystyki.hpp"

    #include <memory>
    #include <vector>
    #include <unordered_map>
    #include <string>

    class SymulatorBoardingu {
    private:
        int aktualnyCzas;
        std::unique_ptr<StrategiaObslugi> strategia;
        std::vector<Stanowisko> stanowiska;
        std::unordered_map<int, Lot> lotyMap;
        std::vector<Pasazer> nadchodzacyPasazerowie;
        Statystyki statystyki;

        // Prywatne metody pomocnicze do pętli zdarzeń dyskretnych
        void wykonajKrok();
        bool czyJakiesStanowiskoZajete() const;

    public:
        explicit SymulatorBoardingu(std::unique_ptr<StrategiaObslugi> wybranaStrategia);

        // Wczytywanie konfiguracji i danych wejściowych
        void wczytajISkonfiguruj(const std::string& plikKonfig, const std::string& plikLoty, const std::string& plikZgloszenia);
        
        // Główna pętla czasu
        void uruchomSymulacje();
        
        // Zapis wyników do pliku
        void zakonczIZapiszRaport(const std::string& plikWyjsciowy, const std::string& nazwaStrategii);
    };