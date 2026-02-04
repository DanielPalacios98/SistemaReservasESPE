#include "Reserva.h"
#include <iostream>
#include <regex>
#include <algorithm>
using namespace std;

Reserva::Reserva(int idReserva, const string& nombres, const string& cedula, 
        const string& telefono, const string& correo,
        const string& localidad, int numAsientos)
    : idReserva(idReserva), nombres(nombres), cedula(cedula),
      telefono(telefono), correo(correo), localidad(localidad),
      numAsientos(numAsientos) {}

int Reserva::getIdReserva() const { return idReserva; }
string Reserva::getNombres() const { return nombres; }
string Reserva::getCedula() const { return cedula; }
string Reserva::getTelefono() const { return telefono; }
string Reserva::getCorreo() const { return correo; }
string Reserva::getLocalidad() const { return localidad; }
int Reserva::getNumAsientos() const { return numAsientos; }
void Reserva::setNumAsientos(int n) { numAsientos = n; }

bool Reserva::validarNombres(const string& nom) {
    // Exige al menos dos palabras, cada una con mayúscula inicial, solo letras.
    regex re("^([A-Z][a-z]+)( [A-Z][a-z]+)+$");
    return regex_match(nom, re);
}

bool Reserva::validarCedula(const string& cedula) {
    if (cedula.size() != 10 || !all_of(cedula.begin(), cedula.end(), ::isdigit))
        return false;
    int provincia = stoi(cedula.substr(0, 2));
    if (provincia < 1 || provincia > 24)
        return false;
    int suma = 0;
    for (int i = 0; i < 9; ++i) {
        int dig = cedula[i] - '0';
        if (i % 2 == 0) {
            dig = dig * 2;
            if (dig > 9) dig = dig - 9;
        }
        suma += dig;
    }
    int d10 = cedula[9] - '0';
    int mod = suma % 10;
    if (mod == 0) return d10 == 0;
    return (10 - mod) == d10;
}

bool Reserva::validarTelefono(const string& tel) {
    regex re("^09[0-9]{8}$");
    return regex_match(tel, re);
}

bool Reserva::validarCorreo(const string& correo) {
    regex re("^[\\w.-]+@[\\w.-]+\\.[a-zA-Z]{2,}$");
    return regex_match(correo, re);
}

bool Reserva::validarLocalidad(const string& loc) {
    return (loc == "palco" || loc == "tribuna" || loc == "general");
}

void Reserva::mostrarDetalle() const {
    cout << "ID: " << idReserva << " | Nombre: " << nombres << " | Cedula: " << cedula
         << " | Telefono: " << telefono << " | Correo: " << correo
         << " | Localidad: " << localidad << " | Asientos: " << numAsientos << endl;
}
