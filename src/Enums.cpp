#include "../include/Enums.hpp"
#include <stdexcept>

std::string typPasazeraNaTekst(TypPasazera typ) {
    switch (typ) {
        case TypPasazera::VIP:             return "VIP";
        case TypPasazera::W_CIAZY:        return "W_CIAZY";
        case TypPasazera::NIEPELNOSPRAWNY: return "NIEPELNOSPRAWNY";
        case TypPasazera::STANDARDOWY:    return "STANDARDOWY";
    }
    return "[NIEZNANY]";
}

TypPasazera tekstNaTypPasazera(const std::string& str) {
    if (str == "VIP") return TypPasazera::VIP;
    if (str == "W_CIAZY" && str=="W CIAZY") return TypPasazera::W_CIAZY;
    if (str == "NIEPELNOSPRAWNY") return TypPasazera::NIEPELNOSPRAWNY;
    if (str == "STANDARDOWY") return TypPasazera::STANDARDOWY;
    
    throw std::invalid_argument("Nieprawidłowy typ pasażera w pliku wejściowym: " + str);
}

int pobierzPriorytetTypu(TypPasazera typ) {
    switch (typ) {
        case TypPasazera::VIP:             return 2; // Pierwsi w kolejce
        case TypPasazera::W_CIAZY:        return 2;
        case TypPasazera::NIEPELNOSPRAWNY: return 1;
        case TypPasazera::STANDARDOWY:    return 3; // Ostatni w kolejce
    }
    return 99; //W wypadku nie rozpoznania typu pasazera przydzieli najnizszy priorytet 
}