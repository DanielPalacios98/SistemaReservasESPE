#pragma once
#include "NodoReserva.h"
#include "BSTReservas.h"
#include <string>

// Cupos máximos por localidad
const int MAX_PALCO   = 10;
const int MAX_TRIBUNA = 20;
const int MAX_GENERAL = 30;

class ListaReserva {
private:
    NodoReserva* head;   // Lista circular
    int autoincID;       // ID autoincremental para nuevas reservas

    // Suma de asientos ocupados en una localidad (palco/tribuna/general)
    int asientosOcupadosLocalidad(const std::string& localidad);

public:
    ListaReserva();
    ~ListaReserva();

    // Agrega una reserva si hay cupo y no supera 5 asientos por cédula
    Reserva* agregarReserva(const std::string& nombres,
                            const std::string& cedula,
                            const std::string& telefono,
                            const std::string& correo,
                            const std::string& localidad,
                            int asientos);

    void mostrarReservas();

    bool buscarPorID(int id);
    NodoReserva* obtenerPorID(int id);
    bool eliminarPorID(int id);

    int contarAsientosPorID(int id);
    int contarAsientosPorCedula(const std::string& cedula);

    // Muestra reservas ordenadas por nombre (true) o por cedula (false)
    void mostrarReservasOrdenadas(bool porNombre);

    // Construye el BST insertando todas las reservas actuales
    void construirBST(BSTReservas& bst);

    // Recorre todas las reservas aplicando una función visitante
    void recorrer(void (*fn)(Reserva*));

    // Acceso controlado al inicio de la lista para recorridos externos
    NodoReserva* obtenerHead() const;

    // Persistencia actual en TXT (CSV simple)
    void guardarEnArchivo(const std::string& filename);
    void cargarDesdeArchivo(const std::string& filename);
    void clear();
    void recargarDesdeArchivo(const std::string& filename);
};
