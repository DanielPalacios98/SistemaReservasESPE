#include "Reserva.h"
#include <cctype>
#include <algorithm>
#include <iostream>

using namespace std;

Reserva::Reserva() {}
Reserva::~Reserva() {}


void Reserva::mostrarDetalle() {
    cout << "-----------------------------" << endl;
    cout << "Nombres: " << nombres << endl;
    cout << "Cédula: " << cedula << endl;
    cout << "Teléfono: " << telefono << endl;
    cout << "Correo: " << correo << endl;
    cout << "Localidad: " << localidad << endl;
    cout << "Cantidad de asientos: " << cantidadAsientos << endl;
    cout << "-----------------------------" << endl;
}

bool Reserva::validarNombres() {
    if (nombres.empty())
        return false;
    for (char c : nombres) {
        if (!isalpha(c) && !isspace(c))
            return false;
    }
    return true;
}

bool Reserva::validarCedula() {
    if (cedula.length() != 10)
        return false;
    for (char c : cedula) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

bool Reserva::validarTelefono() {
    if (telefono.length() != 10)
        return false;
    if (telefono.substr(0, 2) != "09")
        return false;
    for (char c : telefono) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

bool Reserva::validarCorreo() {
    size_t at_pos = correo.find('@');
    size_t dot_pos = correo.find('.', at_pos); // busca '.' después de '@'
    return (!correo.empty() && at_pos != string::npos && dot_pos != string::npos && dot_pos > at_pos);
}

bool Reserva::validarLocalidad() {
    if (localidad.empty())
        return false;
    string loc = localidad;
    transform(loc.begin(), loc.end(), loc.begin(), [](unsigned char c) { return tolower(c); });
    return (loc == "palco" || loc == "tribuna" || loc == "general");
}

bool Reserva::validarCantidadAsientos() {
    return cantidadAsientos == 1;
}

bool Reserva::validarCampos() {
    return validarNombres()
        && validarCedula()
        && validarTelefono()
        && validarCorreo()
        && validarLocalidad()
        && validarCantidadAsientos();
}

// CAMBIO: Setters y getters necesarios para persistencia y operaciones externas
void Reserva::setNombres(const string& n) { nombres = n; }
string Reserva::getNombres() const { return nombres; }
void Reserva::setCedula(const string& c) { cedula = c; }
string Reserva::getCedula() const { return cedula; }
void Reserva::setTelefono(const string& t) { telefono = t; }
string Reserva::getTelefono() const { return telefono; }
void Reserva::setCorreo(const string& e) { correo = e; }
string Reserva::getCorreo() const { return correo; }
void Reserva::setLocalidad(const string& l) { localidad = l; }
string Reserva::getLocalidad() const { return localidad; }
void Reserva::setCantidadAsientos(int cant) { cantidadAsientos = cant; }
int Reserva::getCantidadAsientos() const { return cantidadAsientos; }

