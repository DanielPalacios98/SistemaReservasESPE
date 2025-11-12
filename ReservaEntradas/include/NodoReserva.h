#ifndef NODORESERVA_H
#define NODORESERVA_H
#include "Reserva.h"

class NodoReserva {
public:
    Reserva* reserva;           // Puntero a la Reserva que almacena este nodo
    NodoReserva* siguiente;     // Puntero al siguiente nodo de la lista

    NodoReserva(Reserva* r);    // Constructor adecuado para listas
    ~NodoReserva();
};

#endif // NODORESERVA_H
