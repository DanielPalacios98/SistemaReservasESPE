#include "NodoReserva.h"

NodoReserva::NodoReserva(Reserva* r) {
    reserva = r;
    siguiente = nullptr;
}

NodoReserva::~NodoReserva() {
    // No elimines 'reserva' aquí si la gestionas en ListaReserva::vaciar
}
