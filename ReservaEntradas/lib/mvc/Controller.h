#pragma once
#include "ConsoleView.h"
#include "../include/HashTableUsuarios.h"
#include "../include/ListaReserva.h"
#include "../include/BSTReservas.h"
#include "../include/IReservaRepository.h"

class Controller {
private:
    HashTableUsuarios tablaUsuarios;
    ListaReserva reservas;
    BSTReservas arbol;
    ConsoleView* view;
    IReservaRepository* repo; // propiedad simple; vida del repo administrada en Controller
public:
    explicit Controller(ConsoleView* v);
    explicit Controller(ConsoleView* v, IReservaRepository* repository);
    void run();
};
