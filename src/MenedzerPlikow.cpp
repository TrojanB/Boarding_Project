#include "../include/MenedzerPlikow.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iomanip>

//# ==============================================================================
//# STRUKTURA PLIKU LOTÓW (loty.txt)
//# ID_Lotu  Numer_Lotu  ID_Bramki  Kierunek Czas_Otwarcia  Czas_Zamkniecia
//# ==============================================================================

//# ==============================================================================
//# STRUKTURA PLIKU ZGLOSZEN (zglozsenia.txt)
//# ID  ID_Lotu  Czas_Nadejscia  Czas_Obslugi  Priorytet  Typ_Pasazera
//# ==============================================================================


//# ==============================================================================
//# KONFIGURACJA STANOWISK / BRAMEK (konfiguracja.txt)
//# ID_Stanowiska  Sredni_Czas_Obslugi
//# ==============================================================================

std::vector<Lot> MenedzerPlikow::odczytajLoty(const std::string& sciezka) {
    std::vector<Lot> listaLotow;
    std::ifstream plik(sciezka);

    if (!plik.is_open()) {
        throw std::runtime_error("Nie mozna otworzyc pliku pod sciezka: " + sciezka);
    }

    std::string linia;
    int numerLinii = 0;

    while (std::getline(plik, linia)) {
        numerLinii++;

        // 1. Zamiana twardych spacji (kopiowanych z sieci) na zwykle spacje
        for (size_t i = 0; i < linia.length(); ++i) {
            unsigned char c = static_cast<unsigned char>(linia[i]);
            if (c == 160 || c == 194) {
                linia[i] = ' ';
            }
        }

        // 2. Usuniecie spacji/tabulacji z poczatku wiersza
        size_t start = linia.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            continue; // Pusta linia
        }
        std::string czystaLinia = linia.substr(start);

        // 3. Ignorowanie komentarzy (#)
        if (czystaLinia[0] == '#') {
            continue;
        }

        // 4. Parsowanie kolumn
        std::stringstream ss(czystaLinia);
        int id, idBramki, czasOtwarcia, czasZamkniecia;
        std::string numerLotu, kierunek;

        if (ss >> id >> numerLotu >> idBramki >> kierunek >> czasOtwarcia >> czasZamkniecia) {
            listaLotow.emplace_back(id, numerLotu, idBramki, kierunek, czasOtwarcia, czasZamkniecia);
        } else {
            std::cerr << "[Ostrzezenie] Pominieto linie nr " << numerLinii 
                      << " (bledny format): " << linia << "\n";
        }
    }

    plik.close();
    return listaLotow;
}

std::vector<Pasazer> MenedzerPlikow::odczytajZgloszenia(const std::string& sciezka) {
    std::vector<Pasazer> listaPasazerow;
    std::ifstream plik(sciezka);

    if (!plik.is_open()) {
        throw std::runtime_error("Nie mozna otworzyc pliku zgłoszen pod sciezka: " + sciezka);
    }

    std::string linia;
    int numerLinii = 0;

    while (std::getline(plik, linia)) {
        numerLinii++;

        // Usuwanie UTF-8 BOM w pierwszej linii
        if (numerLinii == 1 && linia.size() >= 3) {
            if (static_cast<unsigned char>(linia[0]) == 0xEF &&
                static_cast<unsigned char>(linia[1]) == 0xBB &&
                static_cast<unsigned char>(linia[2]) == 0xBF) {
                linia = linia.substr(3);
            }
        }

        // Zamiana niewidocznych znaków na spacje
        for (size_t i = 0; i < linia.length(); ++i) {
            unsigned char c = static_cast<unsigned char>(linia[i]);
            if (c < 32 || c > 126) {
                linia[i] = ' ';
            }
        }

        size_t start = linia.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue; // Pusta linia
        
        std::string czystaLinia = linia.substr(start);
        if (czystaLinia[0] == '#') continue; // Komentarz

        std::stringstream ss(czystaLinia);
        int id, idLotu, czasPrzybycia, czasObslugi, priorytet;
        std::string typStr;

        // Parsowanie 6 kolumn: ID, ID_Lotu, Czas_Nadejscia, Czas_Obslugi, Priorytet, Typ_Pasazera
        if (ss >> id >> idLotu >> czasPrzybycia >> czasObslugi >> priorytet >> typStr) {
            TypPasazera typ = tekstNaTypPasazera(typStr);
            listaPasazerow.emplace_back(id, idLotu, czasPrzybycia, czasObslugi, priorytet, typ);
        }
    }

    plik.close();
    return listaPasazerow;
}

std::vector<Stanowisko> MenedzerPlikow::odczytajKonfiguracje(const std::string& sciezka) {
    std::vector<Stanowisko> listaStanowisk;
    std::ifstream plik(sciezka);

    if (!plik.is_open()) {
        throw std::runtime_error("Nie mozna otworzyc pliku konfiguracji: " + sciezka);
    }

    std::string linia;
    int numerLinii = 0;

    while (std::getline(plik, linia)) {
        numerLinii++;
        size_t start = linia.find_first_not_of(" \t\r\n");
        if (start == std::string::npos || linia[start] == '#' || linia[start] == '/') continue;

        std::stringstream ss(linia.substr(start));
        int id;
        double sredniCzas;

        // Wczytujemy tylko 2 kolumny: ID oraz Sredni_Czas_Obslugi
        if (ss >> id >> sredniCzas) {
            listaStanowisk.emplace_back(id, sredniCzas);
        }
    }

    plik.close();
    return listaStanowisk;
}

void MenedzerPlikow::zapiszWyniki(const std::string& sciezka, const Statystyki& stat, int calkowityCzas, const std::string& nazwaStrategii) {
    std::ofstream plik(sciezka);
    if (!plik.is_open()) throw std::runtime_error("Nie mozna utworzyc: " + sciezka);

    int sumaWszystkich = stat.pobierzIloscObsluzonych() + stat.pobierzIloscSpoznionych();
    double procObsluzonych = sumaWszystkich > 0 ? (100.0 * stat.pobierzIloscObsluzonych() / sumaWszystkich) : 0.0;
    double procSpoznionych = sumaWszystkich > 0 ? (100.0 * stat.pobierzIloscSpoznionych() / sumaWszystkich) : 0.0;

    // Nagłówek raportu
    plik << "# ==============================================================================\n";
    plik << "# PLIK WYJŚCIOWY - WYNIKI SYMULACJI BOARDINGU (" << sciezka << ")\n";
    plik << "# ==============================================================================\n\n";

    plik << "================================================================================\n";
    plik << "1. PODSUMOWANIE STATYSTYCZNE METRYK\n";
    plik << "================================================================================\n";
    plik << "Zastosowana strategia obslugi   : " << nazwaStrategii << "\n";
    plik << "Calkowita liczba pasazerow      : " << sumaWszystkich << "\n";
    plik << "Pasazerowie odprawieni na czas  : " << stat.pobierzIloscObsluzonych() 
         << " (" << std::fixed << std::setprecision(1) << procObsluzonych << "%)\n";
    plik << "Pasazerowie spoznieni (No-Show) : " << stat.pobierzIloscSpoznionych() 
         << " (" << std::fixed << std::setprecision(1) << procSpoznionych << "%)\n\n";

    // Średni czas oczekiwania
    plik << "SREDNI CZAS OCZEKIWANIA WEDLUG GRUP:\n";
    plik << "- NIEPELNOSPRAWNY : " << stat.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::NIEPELNOSPRAWNY) << " sek.\n";
    plik << "- W_CIAZY         : " << stat.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::W_CIAZY) << " sek.\n";
    plik << "- VIP             : " << stat.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::VIP) << " sek.\n";
    plik << "- STANDARDOWY     : " << stat.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::STANDARDOWY) << " sek.\n\n";

    plik << "OGOLNE METRYKI KOLEJKI:\n";
    plik << "Sredni czas oczekiwania w kolejce : " << stat.obliczSredniCzasCzekania() << " sek.\n";
    plik << "Maksymalny czas oczekiwania       : " << stat.pobierzNajdluzszyCzasCzekania() << " sek.\n\n";

    plik << "WYKORZYSTANIE POSZCZEGOLNYCH STANOWISK:\n";
    for (const auto& [idStanowiska, czasPracy] : stat.pobierzCzasPracyStanowisk()) {
        double obciazenie = stat.obliczWykorzystanieStanowiska(idStanowiska, calkowityCzas);
        plik << "- Stanowisko " << idStanowiska << " : " 
             << std::fixed << std::setprecision(1) << obciazenie << "% czasu w uzyciu\n";
    }
    plik << "\n";

    // Tabela
    plik << "================================================================================\n";
    plik << "2. SZCZEGOLOWY DZIENNIK ODPRAWIONYCH PASAZEROW\n";
    plik << "================================================================================\n";
    plik << std::left << std::setw(5) << "ID" << std::setw(18) << "Typ_Pasazera" 
         << std::setw(11) << "Przybycie" << std::setw(8) << "Start" 
         << std::setw(10) << "Czekanie" << "Stan\n";
    plik << "--------------------------------------------------------------------------------\n";

    for (const auto& log : stat.pobierzHistorie()) {
        std::string status = log.czyObsluzony ? "OBSLUZONY" : "SPOZNIONY (Gate Closure)";
        std::string startStr = (log.czasRozpoczeciaObslugi == -1) ? "-" : std::to_string(log.czasRozpoczeciaObslugi) + "s";
        
        plik << std::left 
             << std::setw(5)  << log.idPasazera
             << std::setw(18) << typPasazeraNaTekst(log.typ)
             << std::setw(11) << (std::to_string(log.czasPrzybycia) + "s")
             << std::setw(8)  << startStr
             << std::setw(10) << (std::to_string(log.czasCzekania) + "s")
             << status << "\n";
    }
    plik.close();
}