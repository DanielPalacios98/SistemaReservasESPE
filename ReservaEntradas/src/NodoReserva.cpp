#include "NodoReserva.h"
NodoReserva::NodoReserva(Reserva* r) : reserva(r), next(nullptr) {}
NodoReserva::~NodoReserva() { delete reserva; }
