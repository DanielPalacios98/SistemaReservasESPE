#pragma once
#include <string>

class Usuario {
private:
    std::string cedula;
    std::string nombres;
    std::string telefono;
    std::string correo;

public:
    Usuario() = default;

    Usuario(const std::string& cedula,
            const std::string& nombres,
            const std::string& telefono,
            const std::string& correo)
        : cedula(cedula), nombres(nombres),
          telefono(telefono), correo(correo) {}

    std::string getCedula() const  { return cedula; }
    std::string getNombres() const { return nombres; }
    std::string getTelefono() const{ return telefono; }
    std::string getCorreo() const  { return correo; }
};
