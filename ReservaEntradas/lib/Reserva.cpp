#include "Reserva.h"
#include <iostream>
#include <cctype>
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
    if (nom.empty()) return false;

    size_t i = 0;
    int palabras = 0;
    while (i < nom.size()) {
        if (nom.at(i) == ' ') return false; // no espacios al inicio

        const char first = nom.at(i);
        if (first < 'A' || first > 'Z') return false;
        ++i;

        size_t lenPalabra = 1;
        while (i < nom.size() && nom.at(i) != ' ') {
            const char c = nom.at(i);
            if (c < 'a' || c > 'z') return false;
            ++i;
            ++lenPalabra;
        }
        if (lenPalabra < 2) return false;
        ++palabras;

        if (i == nom.size()) break;
        if (nom.at(i) != ' ') return false;
        ++i;
        if (i >= nom.size()) return false; // no espacio al final
        if (nom.at(i) == ' ') return false; // no doble espacio
    }

    return palabras >= 2;
}

bool Reserva::validarCedula(const string& cedula) {
    if (cedula.size() != 10) return false;
    for (size_t i = 0; i < cedula.size(); ++i) {
        if (isdigit(static_cast<unsigned char>(cedula.at(i))) == 0) return false;
    }

    int provincia = (static_cast<int>(cedula.at(0)) - static_cast<int>('0')) * 10 +
                    (static_cast<int>(cedula.at(1)) - static_cast<int>('0'));
    if (provincia < 1 || provincia > 24)
        return false;
    int suma = 0;
    for (int i = 0; i < 9; ++i) {
        int dig = static_cast<int>(cedula.at(static_cast<size_t>(i))) - static_cast<int>('0');
        if (i % 2 == 0) {
            dig = dig * 2;
            if (dig > 9) dig = dig - 9;
        }
        suma += dig;
    }
    int d10 = static_cast<int>(cedula.at(9)) - static_cast<int>('0');
    int mod = suma % 10;
    if (mod == 0) return d10 == 0;
    return (10 - mod) == d10;
}

bool Reserva::validarTelefono(const string& tel) {
    if (tel.size() != 10) return false;
    if (tel.at(0) != '0' || tel.at(1) != '9') return false;
    for (size_t i = 2; i < tel.size(); ++i) {
        if (isdigit(static_cast<unsigned char>(tel.at(i))) == 0) return false;
    }
    return true;
}

bool Reserva::validarCorreo(const string& correo) {
    if (correo.empty()) return false;
    const size_t atPos = correo.find('@');
    if (atPos == string::npos) return false;
    if (atPos == 0) return false;
    if (atPos + 1 >= correo.size()) return false;
    if (correo.find('@', atPos + 1) != string::npos) return false;

    const size_t lastDot = correo.rfind('.');
    if (lastDot == string::npos) return false;
    if (lastDot <= atPos + 1) return false;
    if (lastDot + 1 >= correo.size()) return false;

    for (size_t i = 0; i < atPos; ++i) {
        const char c = correo.at(i);
        const bool ok = (isalnum(static_cast<unsigned char>(c)) != 0) || c == '_' || c == '.' || c == '-';
        if (!ok) return false;
    }

    for (size_t i = atPos + 1; i < correo.size(); ++i) {
        const char c = correo.at(i);
        const bool ok = (isalnum(static_cast<unsigned char>(c)) != 0) || c == '.' || c == '-';
        if (!ok) return false;
    }

    const size_t tldLen = correo.size() - (lastDot + 1);
    if (tldLen < 2) return false;
    for (size_t i = lastDot + 1; i < correo.size(); ++i) {
        if (isalpha(static_cast<unsigned char>(correo.at(i))) == 0) return false;
    }

    return true;
}

bool Reserva::validarLocalidad(const string& loc) {
    return (loc == "palco" || loc == "tribuna" || loc == "general");
}

void Reserva::mostrarDetalle() const {
    cout << "ID: " << idReserva << " | Nombre: " << nombres << " | Cedula: " << cedula
         << " | Telefono: " << telefono << " | Correo: " << correo
         << " | Localidad: " << localidad << " | Asientos: " << numAsientos << endl;
}
