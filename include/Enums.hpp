#pragma once
#include <string>

enum class TypPasazera {
    STANDARDOWY,
    W_CIAZY,
    NIEPELNOSPRAWNY,
    VIP
};

std::string typPasazeraNaTekst(TypPasazera typ);
TypPasazera tekstNaTypPasazera(const std::string& str);
int pobierzPriorytetTypu(TypPasazera typ);