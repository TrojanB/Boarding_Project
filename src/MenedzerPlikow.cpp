#include "../include/MenedzerPlikow.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

std::vector<Stanowisko> MenedzerPlikow::odczytajKonfiguracje(const std::string& sciezka) {
    std::vector<Stanowisko> wynik;
    std::ifstream plik(sciezka);
    if (!plik.is_open()) {
        std::cerr << "Blad otwarcia pliku konfiguracji: " << sciezka << std::endl;
        return wynik;
    }

    std::string linia;
    while (std::getline(plik, linia)) {
        if (linia.empty() || linia[0] == '#') continue;
        std::stringstream ss(linia);
        int id;
        double czas;
        char separator;
        if (linia.find(';') != std::string::npos) {
            ss >> id >> separator >> czas;
        } else {
            ss >> id >> czas;
        }
        wynik.emplace_back(id, czas);
    }
    return wynik;
}

std::vector<Lot> MenedzerPlikow::odczytajLoty(const std::string& sciezka) {
    std::vector<Lot> wynik;
    std::ifstream plik(sciezka);
    if (!plik.is_open()) {
        std::cerr << "Blad otwarcia pliku lotow: " << sciezka << std::endl;
        return wynik;
    }

    std::string linia;
    while (std::getline(plik, linia)) {
        if (linia.empty() || linia[0] == '#') continue;

        std::stringstream ss(linia);
        std::vector<std::string> kolumny;
        std::string token;

        // Obsługa średników oraz białych znaków (spacji/tabulacji)
        if (linia.find(';') != std::string::npos) {
            while (std::getline(ss, token, ';')) {
                if (!token.empty()) kolumny.push_back(token);
            }
        } else {
            while (ss >> token) {
                kolumny.push_back(token);
            }
        }

        // 6 kolumn: ID_Lotu [0], Numer_Lotu [1], ID_Bramki [2], Kierunek [3], Czas_Otwarcia [4], Czas_Zamkniecia [5]
        if (kolumny.size() >= 6) {
            int id = std::stoi(kolumny[0]);
            int czasZamkniecia = std::stoi(kolumny[5]);
            wynik.emplace_back(id, czasZamkniecia);
        } else if (kolumny.size() >= 2) {
            int id = std::stoi(kolumny[0]);
            int czasZamkniecia = std::stoi(kolumny.back());
            wynik.emplace_back(id, czasZamkniecia);
        }
    }
    return wynik;
}

static TypPasazera konwertujNaTyp(const std::string& str) {
    if (str == "NIEPELNOSPRAWNY") return TypPasazera::NIEPELNOSPRAWNY;
    if (str == "W_CIAZY")         return TypPasazera::W_CIAZY;
    if (str == "VIP")             return TypPasazera::VIP;
    return TypPasazera::STANDARDOWY;
}

std::vector<Pasazer> MenedzerPlikow::odczytajZgloszenia(const std::string& sciezka) {
    std::vector<Pasazer> wynik;
    std::ifstream plik(sciezka);
    if (!plik.is_open()) {
        std::cerr << "Blad otwarcia pliku zgloszen: " << sciezka << std::endl;
        return wynik;
    }

    std::string linia;
    while (std::getline(plik, linia)) {
        if (linia.empty() || linia[0] == '#') continue;
        std::stringstream ss(linia);
        int id, idLotu, czasNadejscia, czasObslugi, priorytet;
        std::string typStr;

        if (linia.find(';') != std::string::npos) {
            char sep;
            ss >> id >> sep >> idLotu >> sep >> czasNadejscia >> sep >> czasObslugi >> sep >> priorytet >> sep >> typStr;
        } else {
            ss >> id >> idLotu >> czasNadejscia >> czasObslugi >> priorytet >> typStr;
        }

        TypPasazera typ = konwertujNaTyp(typStr);
        wynik.emplace_back(id, idLotu, czasNadejscia, czasObslugi, priorytet, typ);
    }
    return wynik;
}

void MenedzerPlikow::zapiszWyniki(const std::string& sciezka, const Statystyki& statystyki, int calkowityCzas, const std::string& nazwaStrategii) {
    std::ofstream plik(sciezka);
    if (!plik.is_open()) {
        std::cerr << "Blad otwarcia pliku do zapisu: " << sciezka << std::endl;
        return;
    }

    plik << "================================================================================\n";
    plik << "1. PODSUMOWANIE STATYSTYCZNE METRYK\n";
    plik << "================================================================================\n";
    plik << "Zastosowana strategia obslugi   : " << nazwaStrategii << "\n";

    int obsluzeni = statystyki.pobierzIloscObsluzonych();
    int spoznieni = statystyki.pobierzIloscSpoznionych();
    int calkowita = obsluzeni + spoznieni;
    double procObs = calkowita > 0 ? (100.0 * obsluzeni / calkowita) : 0.0;
    double procSpoz = calkowita > 0 ? (100.0 * spoznieni / calkowita) : 0.0;

    plik << "Calkowita liczba pasazerow      : " << calkowita << "\n";
    plik << "Pasazerowie odprawieni na czas  : " << obsluzeni << " (" << std::fixed << std::setprecision(1) << procObs << "%)\n";
    plik << "Pasazerowie spoznieni (No-Show) : " << spoznieni << " (" << procSpoz << "%)\n\n";

    plik << "SREDNI CZAS OCZEKIWANIA WEDLUG GRUP:\n";
    plik << "- NIEPELNOSPRAWNY : " << statystyki.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::NIEPELNOSPRAWNY) << " sek.\n\n";
    plik << "- W_CIAZY         : " << statystyki.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::W_CIAZY) << " sek.\n\n";
    plik << "- VIP             : " << statystyki.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::VIP) << " sek.\n\n";
    plik << "- STANDARDOWY     : " << statystyki.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::STANDARDOWY) << " sek.\n\n\n";

    plik << "OGOLNE METRYKI KOLEJKI:\n";
    plik << "Sredni czas oczekiwania w kolejce : " << statystyki.obliczSredniCzasCzekania() << " sek.\n";
    plik << "Maksymalny czas oczekiwania        : " << statystyki.pobierzNajdluzszyCzasCzekania() << " sek.\n\n";

    plik << "WYKORZYSTANIE POSZCZEGOLNYCH STANOWISK:\n";
    for (const auto& pair : statystyki.pobierzCzasPracyStanowisk()) {
        double proc = statystyki.obliczWykorzystanieStanowiska(pair.first, calkowityCzas);
        plik << "- Stanowisko " << pair.first << " : " << std::fixed << std::setprecision(1) << proc << "% czasu w uzyciu\n\n";
    }
    plik << "================================================================================\n";
    plik.close();
}

void MenedzerPlikow::zapiszWynikiPorownawcze(const std::string& sciezka, 
                                             const Statystyki& statFifo, int czasFifo,
                                             const Statystyki& statPrio, int czasPrio) {
    std::ofstream plik(sciezka);
    if (!plik.is_open()) {
        std::cerr << "Blad otwarcia pliku do zapisu: " << sciezka << std::endl;
        return;
    }

    plik << "================================================================================\n";
    plik << "               ZESTAWIENIE POROWNAWCZE STRATEGII OBSLUGI BRAMEK                 \n";
    plik << "================================================================================\n\n";

    plik << std::left << std::setw(38) << "METRYKA" 
         << std::setw(22) << "STRATEGIA FIFO" 
         << std::setw(22) << "STRATEGIA PRIORYTETOWA" << "\n";
    plik << "--------------------------------------------------------------------------------\n";

    int ogolemFifo = statFifo.pobierzIloscObsluzonych() + statFifo.pobierzIloscSpoznionych();
    int ogolemPrio = statPrio.pobierzIloscObsluzonych() + statPrio.pobierzIloscSpoznionych();

    double procObsFifo = ogolemFifo > 0 ? (100.0 * statFifo.pobierzIloscObsluzonych() / ogolemFifo) : 0;
    double procObsPrio = ogolemPrio > 0 ? (100.0 * statPrio.pobierzIloscObsluzonych() / ogolemPrio) : 0;

    plik << std::setw(38) << "Calkowity czas symulacji [s]:" 
         << std::setw(22) << czasFifo 
         << std::setw(22) << czasPrio << "\n";

    plik << std::setw(38) << "Liczba pasazerow obsluzonych:" 
         << std::setw(22) << (std::to_string(statFifo.pobierzIloscObsluzonych()) + " (" + std::to_string(procObsFifo).substr(0,4) + "%)")
         << std::setw(22) << (std::to_string(statPrio.pobierzIloscObsluzonych()) + " (" + std::to_string(procObsPrio).substr(0,4) + "%)") << "\n";

    plik << std::setw(38) << "Liczba pasazerow spoznionych:" 
         << std::setw(22) << statFifo.pobierzIloscSpoznionych() 
         << std::setw(22) << statPrio.pobierzIloscSpoznionych() << "\n";

    plik << std::setw(38) << "Sredni czas oczekiwania (ogolny) [s]:" 
         << std::setw(22) << std::fixed << std::setprecision(1) << statFifo.obliczSredniCzasCzekania()
         << std::setw(22) << std::fixed << std::setprecision(1) << statPrio.obliczSredniCzasCzekania() << "\n";

    plik << std::setw(38) << "Maksymalny czas oczekiwania [s]:" 
         << std::setw(22) << statFifo.pobierzNajdluzszyCzasCzekania()
         << std::setw(22) << statPrio.pobierzNajdluzszyCzasCzekania() << "\n";

    plik << "\nLICZBA PASAZEROW WEDLUG GRUP:\n";
    plik << "--------------------------------------------------------------------------------\n";
    plik << std::setw(38) << "- NIEPELNOSPRAWNY:" 
         << std::setw(22) << statFifo.pobierzIloscDlaTypu(TypPasazera::NIEPELNOSPRAWNY)
         << std::setw(22) << statPrio.pobierzIloscDlaTypu(TypPasazera::NIEPELNOSPRAWNY) << "\n";

    plik << std::setw(38) << "- W_CIAZY:" 
         << std::setw(22) << statFifo.pobierzIloscDlaTypu(TypPasazera::W_CIAZY)
         << std::setw(22) << statPrio.pobierzIloscDlaTypu(TypPasazera::W_CIAZY) << "\n";

    plik << std::setw(38) << "- VIP:" 
         << std::setw(22) << statFifo.pobierzIloscDlaTypu(TypPasazera::VIP)
         << std::setw(22) << statPrio.pobierzIloscDlaTypu(TypPasazera::VIP) << "\n";

    plik << std::setw(38) << "- STANDARDOWY:" 
         << std::setw(22) << statFifo.pobierzIloscDlaTypu(TypPasazera::STANDARDOWY)
         << std::setw(22) << statPrio.pobierzIloscDlaTypu(TypPasazera::STANDARDOWY) << "\n";

    plik << "\nSREDNI CZAS OCZEKIWANIA WEDLUG GRUP [sekundy]:\n";
    plik << "--------------------------------------------------------------------------------\n";
    plik << std::setw(38) << "- NIEPELNOSPRAWNY:" 
         << std::setw(22) << statFifo.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::NIEPELNOSPRAWNY)
         << std::setw(22) << statPrio.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::NIEPELNOSPRAWNY) << "\n";

    plik << std::setw(38) << "- W_CIAZY:" 
         << std::setw(22) << statFifo.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::W_CIAZY)
         << std::setw(22) << statPrio.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::W_CIAZY) << "\n";

    plik << std::setw(38) << "- VIP:" 
         << std::setw(22) << statFifo.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::VIP)
         << std::setw(22) << statPrio.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::VIP) << "\n";

    plik << std::setw(38) << "- STANDARDOWY:" 
         << std::setw(22) << statFifo.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::STANDARDOWY)
         << std::setw(22) << statPrio.obliczSredniCzasCzekaniaDlaTypu(TypPasazera::STANDARDOWY) << "\n";

    plik << "================================================================================\n";
    plik.close();
}