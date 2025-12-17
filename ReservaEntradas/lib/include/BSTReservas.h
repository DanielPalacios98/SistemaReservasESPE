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

    void insertarRec(NodoBST*& nodo, Reserva* r) {
        if (!nodo) {
            nodo = new NodoBST(r);
            return;
        }
        if (r->getIdReserva() < nodo->id) {
            insertarRec(nodo->izq, r);
        } else if (r->getIdReserva() > nodo->id) {
            insertarRec(nodo->der, r);
        } else {
            // ID duplicado no debería ocurrir porque autoincID es único
        }
    }

    Reserva* buscarRec(NodoBST* nodo, int id) const {
        if (!nodo) return nullptr;
        if (id == nodo->id) return nodo->reserva;
        if (id < nodo->id) return buscarRec(nodo->izq, id);
        return buscarRec(nodo->der, id);
    }

    void destruirRec(NodoBST* nodo) {
        if (!nodo) return;
        destruirRec(nodo->izq);
        destruirRec(nodo->der);
        delete nodo;
    }

public:
    BSTReservas() : raiz(nullptr) {}
    ~BSTReservas() { destruirRec(raiz); }

    void insertar(Reserva* r) {
        if (!r) return;
        insertarRec(raiz, r);
    }

    Reserva* buscar(int id) const {
        return buscarRec(raiz, id);
    }
};
