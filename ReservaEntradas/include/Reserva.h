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

    // Se pueden declarar más atributos si modificamos el UML

public:

// Constructor y destructor
    Reserva();
    ~Reserva();

    bool validarNombres();
    bool validarCedula();
    bool validarTelefono();
    bool validarCorreo();
    bool validarLocalidad();
    bool validarCantidadAsientos();
    bool validarCampos();

    // para visualizar
    void mostrarDetalle();
};

#endif // RESERVA_H
