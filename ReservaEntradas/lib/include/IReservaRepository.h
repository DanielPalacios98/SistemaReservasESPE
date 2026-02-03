#pragma once
#include "ListaReserva.h"
#include <string>
using namespace std;

class IReservaRepository {
public:
    virtual ~IReservaRepository() {}
    virtual bool cargar(ListaReserva& lista) = 0;
    virtual bool guardar(const ListaReserva& lista) = 0;
};
