#pragma once
#include <string>
using namespace std;

class Reserva {
private:
    int idReserva;
    string nombres;
    string cedula;
    string telefono;
    string correo;
    string localidad;
    int numAsientos;
public:
    Reserva(int idReserva, const string& nombres, const string& cedula, 
            const string& telefono, const string& correo,
            const string& localidad, int numAsientos);

    int getIdReserva() const;
    string getNombres() const;
    string getCedula() const;
    string getTelefono() const;
    string getCorreo() const;
    string getLocalidad() const;
    int getNumAsientos() const;
    void setNumAsientos(int);

    // Validadores
    static bool validarNombres(const string&);
    static bool validarCedula(const string&);
    static bool validarTelefono(const string&);
    static bool validarCorreo(const string&);
    static bool validarLocalidad(const string&);

    void mostrarDetalle() const;
};
