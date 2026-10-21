#pragma once
#include <string>

enum class TypPasazera {
    STANDARDOWY,
    W_CIAZY,
    NIEPELNOSPRAWNY,
    VIP
};

std::string typPasazeraNaTekst(TypPasazera typ);
TypPasazera tekstNaTypPasazera(const std::string& strInput); // Przyjmuje const std::string&
int pobierzPriorytetTypu(TypPasazera typ);