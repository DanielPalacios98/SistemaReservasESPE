#pragma once
#include <string>
using namespace std;

class Usuario {
private:
    string cedula;
    string nombres;
    string telefono;
    string correo;

public:
    Usuario() = default;

    Usuario(const string& cedula,
            const string& nombres,
            const string& telefono,
            const string& correo)
        : cedula(cedula), nombres(nombres),
          telefono(telefono), correo(correo) {}

    string getCedula() const  { return cedula; }
    string getNombres() const { return nombres; }
    string getTelefono() const{ return telefono; }
    string getCorreo() const  { return correo; }
};
