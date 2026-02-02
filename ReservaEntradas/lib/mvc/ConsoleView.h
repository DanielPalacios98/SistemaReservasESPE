#pragma once
#include <string>

class Reserva;

class ConsoleView {
public:
    void limpiarBuffer();
    int pedirOpcionMenu(int min, int max);
    int pedirIDValido();
    std::string pedirLinea(const std::string& prompt);
    void mostrarMensaje(const std::string& msg);
    void mostrarReservaDetalle(Reserva* r);
};
