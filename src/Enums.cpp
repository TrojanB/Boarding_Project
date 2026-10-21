#include "../include/Enums.hpp"
#include <stdexcept>
#include <algorithm>

std::string typPasazeraNaTekst(TypPasazera typ) {
    switch (typ) {
        case TypPasazera::VIP:             return "VIP";
        case TypPasazera::W_CIAZY:        return "W_CIAZY";
        case TypPasazera::NIEPELNOSPRAWNY: return "NIEPELNOSPRAWNY";
        case TypPasazera::STANDARDOWY:    return "STANDARDOWY";
    }
    return "NIEZNANY";
}

TypPasazera tekstNaTypPasazera(const std::string& strInput) {
    std::string str = strInput;

    // Obcinanie białych i niewidocznych znaków z początku i końca ciągu
    size_t first = str.find_first_not_of(" \t\r\n\v\f");
    if (first != std::string::npos) {
        size_t last = str.find_last_not_of(" \t\r\n\v\f");
        str = str.substr(first, (last - first + 1));
    } else {
        str = "";
    }

    if (str == "VIP") return TypPasazera::VIP;
    if (str == "W_CIAZY" || str == "WCIAZY" || str == "W_CIĄŻY") return TypPasazera::W_CIAZY;
    if (str == "NIEPELNOSPRAWNY" || str == "NIEPEŁNOSPRAWNY") return TypPasazera::NIEPELNOSPRAWNY;
    if (str == "STANDARDOWY") return TypPasazera::STANDARDOWY;

    throw std::invalid_argument("Nieprawidlowy typ pasazera w pliku wejsciowym: [" + str + "]");
}

int pobierzPriorytetTypu(TypPasazera typ) {
    switch (typ) {
        case TypPasazera::NIEPELNOSPRAWNY: return 1;
        case TypPasazera::W_CIAZY:        return 2;
        case TypPasazera::VIP:             return 2;
        case TypPasazera::STANDARDOWY:    return 3;
    }
    return 99;
}