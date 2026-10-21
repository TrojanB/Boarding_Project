#include "../include/SymulatorBoardingu.hpp"
#include "../include/MenedzerPlikow.hpp"

SymulatorBoardingu::SymulatorBoardingu(std::unique_ptr<StrategiaObslugi> wybranaStrategia)
    : aktualnyCzas(0), strategia(std::move(wybranaStrategia)) {}

void SymulatorBoardingu::wczytajISkonfiguruj(const std::string& plikKonfig, const std::string& plikLoty, const std::string& plikZgloszenia) {
    MenedzerPlikow menedzer;
    stanowiska = menedzer.odczytajKonfiguracje(plikKonfig);
    
    std::vector<Lot> lotyVector = menedzer.odczytajLoty(plikLoty);
    for (const auto& lot : lotyVector) {
        lotyMap.emplace(lot.pobierzId(), lot);
    }
    nadchodzacyPasazerowie = menedzer.odczytajZgloszenia(plikZgloszenia);
}

void SymulatorBoardingu::uruchomSymulacje() {
    // Pętla zdarzeń dyskretnych - główny upływ czasu
    while (!nadchodzacyPasazerowie.empty() || !strategia->czyKolejkaPusta() || czyJakiesStanowiskoZajete()) {
        wykonajKrok();
        aktualnyCzas++; // Upływ czasu o 1 jednostkę (sekundę)
    }
}

void SymulatorBoardingu::wykonajKrok() {
    // 1. Zmiana stanu: Nadejście zgłoszeń (pasażerów) w danej sekundzie
    auto it = nadchodzacyPasazerowie.begin();
    while (it != nadchodzacyPasazerowie.end()) {
        if (it->pobierzCzasPrzybycia() == aktualnyCzas) {
            strategia->dodajPasazera(*it);
            it = nadchodzacyPasazerowie.erase(it);
        } else {
            ++it;
        }
    }

    // 2. Zmiana stanów wykonawców (stanowisk) i obsługa
    for (auto& stanowisko : stanowiska) {
        stanowisko.aktualizujCzas(); // Zmniejsza czas obsługi, zwalnia stanowisko jeśli koniec

        if (stanowisko.czyZajete()) {
            statystyki.zanotujPraceStanowiska(stanowisko.pobierzId());
        }

        // Przydzielenie zgłoszenia do wolnego wykonawcy
        if (!stanowisko.czyZajete() && !strategia->czyKolejkaPusta()) {
            Pasazer nastepny = strategia->pobierzNastepnegoPasazera();
            auto itLot = lotyMap.find(nastepny.pobierzIdLotu());
            
            if (itLot != lotyMap.end()) {
                if (aktualnyCzas > itLot->second.pobierzCzasZamkniecia()) {
                    statystyki.zanotujSpoznienie(nastepny); // Zmiana stanu: Spóźniony
                } else {
                    stanowisko.przydzielPasazera(nastepny); // Zmiana stanu: W trakcie obsługi
                    statystyki.zanotujObsluge(nastepny, aktualnyCzas);
                }
            }
        }
    }
}

bool SymulatorBoardingu::czyJakiesStanowiskoZajete() const {
    for (const auto& st : stanowiska) {
        if (st.czyZajete()) return true;
    }
    return false;
}

void SymulatorBoardingu::zakonczIZapiszRaport(const std::string& plikWyjsciowy, const std::string& nazwaStrategii) {
    MenedzerPlikow menedzer;
    menedzer.zapiszWyniki(plikWyjsciowy, statystyki, aktualnyCzas, nazwaStrategii);
}