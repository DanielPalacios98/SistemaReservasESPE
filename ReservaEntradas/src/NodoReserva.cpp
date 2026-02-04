#include "NodoReserva.h"
NodoReserva::NodoReserva(Reserva* r) : reserva(r), next(nullptr) {}
NodoReserva::~NodoReserva() { } // No borrar reserva aquí para evitar double delete
