#ifndef RESERVA_H
#define RESERVA_H

#include <string>
using namespace std;

class Reserva {
private:
    string nombres;
    string cedula;
    string telefono;
    string correo;
    string localidad;
    int cantidadAsientos;
public:
    Reserva();   // Constructor
    ~Reserva();  // Destructor

    // Getters y setters necesarios para persistencia y operaciones
    // CAMBIO: Agregados para acceso y modificación segura
    void setNombres(const string& n);        // CAMBIO
    string getNombres() const;               // CAMBIO
    void setCedula(const string& c);         // CAMBIO
    string getCedula() const;                // CAMBIO
    void setTelefono(const string& t);       // CAMBIO
    string getTelefono() const;              // CAMBIO
    void setCorreo(const string& e);         // CAMBIO
    string getCorreo() const;                // CAMBIO
    void setLocalidad(const string& l);      // CAMBIO
    string getLocalidad() const;             // CAMBIO
    void setCantidadAsientos(int cant);      // CAMBIO
    int getCantidadAsientos() const;         // CAMBIO

    // Validadores y visualización de reserva
    bool validarNombres();
    bool validarCedula();
    bool validarTelefono();
    bool validarCorreo();
    bool validarLocalidad();
    bool validarCantidadAsientos();
    bool validarCampos();

    void mostrarDetalle(); // Mostrar datos de la reserva
};

#endif // RESERVA_H
