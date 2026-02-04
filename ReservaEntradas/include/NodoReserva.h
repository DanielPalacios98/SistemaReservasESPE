#pragma once
#include "Reserva.h"

class NodoReserva {
public:
    Reserva* reserva;
    NodoReserva* next;
    NodoReserva(Reserva* reserva);
    ~NodoReserva();
};
