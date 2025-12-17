#pragma once
#include "Reserva.h"

struct NodoBST {
    int id;
    Reserva* reserva;
    NodoBST* izq;
    NodoBST* der;

    explicit NodoBST(Reserva* r)
        : id(r->getIdReserva()), reserva(r), izq(nullptr), der(nullptr) {}
};

class BSTReservas {
private:
    NodoBST* raiz;

    void insertarRec(NodoBST*& nodo, Reserva* r);
    Reserva* buscarRec(NodoBST* nodo, int id) const;
    void destruirRec(NodoBST* nodo);

public:
    BSTReservas() : raiz(nullptr) {}
    ~BSTReservas() { destruirRec(raiz); }

    void insertar(Reserva* r);
    Reserva* buscar(int id) const;
};
