#pragma once
#include "Reserva.h"

// Nodo para lista circular
class NodoReserva {
public:
    Reserva* reserva;
    NodoReserva* next;

    NodoReserva(Reserva* reserva);
    ~NodoReserva();
};
