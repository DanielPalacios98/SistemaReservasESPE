#pragma once
#include "ConsoleView.h"
#include "../include/HashTableUsuarios.h"
#include "../include/ListaReserva.h"
#include "../include/BSTReservas.h"

class Controller {
private:
    HashTableUsuarios tablaUsuarios;
    ListaReserva reservas;
    BSTReservas arbol;
    ConsoleView* view;
public:
    explicit Controller(ConsoleView* v);
    void run();
};
