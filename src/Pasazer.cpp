#include "../include/Pasazer.hpp"

Pasazer::Pasazer(int id, int idLotu, int czasPrzybycia, int czasObslugi, int priorytet, TypPasazera typ)
    : id(id), idLotu(idLotu), czasPrzybycia(czasPrzybycia),
      czasObslugi(czasObslugi), priorytet(priorytet), typ(typ) {}