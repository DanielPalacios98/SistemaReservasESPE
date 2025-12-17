#include "BSTReservas.h"

void BSTReservas::insertarRec(NodoBST*& nodo, Reserva* r) {
    if (!nodo) {
        nodo = new NodoBST(r);
        return;
    }
    if (r->getIdReserva() < nodo->id) {
        insertarRec(nodo->izq, r);
    } else if (r->getIdReserva() > nodo->id) {
        insertarRec(nodo->der, r);
    }
}

Reserva* BSTReservas::buscarRec(NodoBST* nodo, int id) const {
    if (!nodo) return nullptr;
    if (id == nodo->id) return nodo->reserva;
    if (id < nodo->id) return buscarRec(nodo->izq, id);
    return buscarRec(nodo->der, id);
}

void BSTReservas::destruirRec(NodoBST* nodo) {
    if (!nodo) return;
    destruirRec(nodo->izq);
    destruirRec(nodo->der);
    delete nodo;
}

void BSTReservas::insertar(Reserva* r) {
    if (!r) return;
    insertarRec(raiz, r);
}

Reserva* BSTReservas::buscar(int id) const {
    return buscarRec(raiz, id);
}
