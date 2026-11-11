#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iomanip>

struct Lot {
    int id;
    std::string numerLotu;
    int idBramki;
    std::string kierunek;
    int czasOtwarcia;
    int czasZamkniecia;
};

struct Pasazer {
    int id;
    int idLotu;
    int czasNadejscia;
    int czasObslugi;
    int priorytet;
    std::string typ;
};

void generujDaneOdlotow(int liczbaPasazerow = 8000, int liczbaBramek = 13) {
    std::mt19937 gen(1337); // Stałe ziarno dla powtarzalności wyników

    std::vector<std::string> miasta = {
        "Warszawa", "Londyn", "Frankfurt", "Paryz", "Rzym", 
        "Madryt", "Barcelona", "Nowy_Jork", "Monachium", "Wieden",
        "Dubaj", "Abu_Zabi", "Doha", "Stambul", "Tokio"
    };
    std::vector<std::string> linie = {"LO", "FR", "LH", "KL", "AF", "BA", "EY", "EK"};

    // Parametry czasowe bramek
    const int DLUGOSC_OTWARCIA_BRAMKI = 2100; // 35 minut otwarcia bramki (w sekundach)
    const int PRZERWA_MIEDZY_LOTAMI   = 300;  // 5 minut przerwy technicznej
    const int LOTOW_NA_BRAMKE         = 20;   // 20 lotów na bramkę (łącznie 260 lotów)

    // 1. GENEROWANIE HARMONOGRAMU LOTÓW (loty.txt)
    std::vector<Lot> loty;
    int idLotuCounter = 1;
    
    for (int bramka = 1; bramka <= liczbaBramek; ++bramka) {
        int t = (bramka - 1) * 120; // Lekkie przesunięcie startu pierwszej fali między bramkami
        
        for (int i = 0; i < LOTOW_NA_BRAMKE; ++i) {
            std::string nr = linie[gen() % linie.size()] + std::to_string(1000 + gen() % 8999);
            std::string kierunek = miasta[gen() % miasta.size()];
            
            int czasOtwarcia = t;
            int czasZamkniecia = t + DLUGOSC_OTWARCIA_BRAMKI; 

            loty.push_back({idLotuCounter++, nr, bramka, kierunek, czasOtwarcia, czasZamkniecia});
            t = czasZamkniecia + PRZERWA_MIEDZY_LOTAMI;
        }
    }

    // Zapis do pliku loty.txt
    std::ofstream plikLoty("loty.txt");
    plikLoty << "# ==============================================================================\n";
    plikLoty << "# STRUKTURA PLIKU LOTÓW (loty.txt) - " << loty.size() << " LOTÓW (" << liczbaBramek << " BRAMEK)\n";
    plikLoty << "# ID_Lotu  Numer_Lotu  ID_Bramki  Kierunek        Czas_Otwarcia  Czas_Zamkniecia\n";
    plikLoty << "# ==============================================================================\n";

    for (const auto& l : loty) {
        plikLoty << std::left 
                 << std::setw(10) << l.id 
                 << std::setw(12) << l.numerLotu 
                 << std::setw(11) << l.idBramki 
                 << std::setw(16) << l.kierunek 
                 << std::setw(15) << l.czasOtwarcia 
                 << l.czasZamkniecia << "\n";
    }
    plikLoty.close();
    std::cout << "[OK] Wygenerowano loty.txt (" << loty.size() << " lotow na " << liczbaBramek << " bramkach)\n";

    // 2. GENEROWANIE PASAŻERÓW (zgloszenia.txt) - 8 000 REKORDÓW
    std::vector<Pasazer> pasazerowie;
    std::discrete_distribution<> distTyp({3, 5, 12, 80}); // 3% Niepełnosprawny, 5% Ciąża, 12% VIP, 80% Standard

    for (int i = 1; i <= liczbaPasazerow; ++i) {
        const Lot& przypisanyLot = loty[gen() % loty.size()];

        // ROZKLAD REALISTYCZNY (ABU ZABI / HUB STYLE):
        // Średnia przybycia (mu): 300 s (5 min) przed otwarciem bramki.
        // Odchylenie standardowe (sigma): 600 s (10 min).
        std::normal_distribution<double> distArrival(przypisanyLot.czasOtwarcia - 300, 600.0);

        int czasNadejscia = static_cast<int>(distArrival(gen));
        czasNadejscia = std::clamp(czasNadejscia, 0, przypisanyLot.czasZamkniecia - 15);

        int type_idx = distTyp(gen);
        std::string typ;
        int prio, czas_obslugi;

        if (type_idx == 0)      { typ = "NIEPELNOSPRAWNY"; prio = 1; czas_obslugi = 10 + (gen() % 10); }
        else if (type_idx == 1) { typ = "W_CIAZY";         prio = 2; czas_obslugi = 5 + (gen() % 8); }
        else if (type_idx == 2) { typ = "VIP";             prio = 2; czas_obslugi = 3 + (gen() % 6); }
        else                    { typ = "STANDARDOWY";     prio = 3; czas_obslugi = 3 + (gen() % 5); }

        pasazerowie.push_back({
            i, przypisanyLot.id, czasNadejscia, czas_obslugi, prio, typ
        });
    }

    // Chronologiczne sortowanie pasażerów według Czas_Nadejscia
    std::sort(pasazerowie.begin(), pasazerowie.end(), [](const Pasazer& a, const Pasazer& b) {
        return a.czasNadejscia < b.czasNadejscia;
    });

    // Reindeksacja ID po posortowaniu (od 1 do 8000)
    for (size_t i = 0; i < pasazerowie.size(); ++i) {
        pasazerowie[i].id = static_cast<int>(i + 1);
    }

    // Zapis do pliku zgloszenia.txt
    std::ofstream plikZgloszenia("zgloszenia.txt");
    plikZgloszenia << "# ==============================================================================\n";
    plikZgloszenia << "# STRUKTURA PLIKU ZGŁOSZEŃ (zgloszenia.txt) - " << liczbaPasazerow << " REKORDÓW\n";
    plikZgloszenia << "# ID  ID_Lotu  Czas_Nadejscia  Czas_Obslugi  Priorytet  Typ_Pasazera\n";
    plikZgloszenia << "# ==============================================================================\n";

    for (const auto& p : pasazerowie) {
        plikZgloszenia << p.id << "\t" 
                       << p.idLotu << "\t" 
                       << p.czasNadejscia << "\t" 
                       << p.czasObslugi << "\t" 
                       << p.priorytet << "\t" 
                       << p.typ << "\n";
    }

    plikZgloszenia.close();
    std::cout << "[OK] Wygenerowano zgloszenia.txt (" << liczbaPasazerow << " pasazerow)\n";
}

int main() {
    generujDaneOdlotow(8000, 13); // 8 000 pasażerów, 13 bramek (260 lotów)
    return 0;
}

