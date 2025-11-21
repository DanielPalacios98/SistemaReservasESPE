#pragma once
#include "NodoReserva.h"
#include <string>
using namespace std;

// Manejo de lista circular con control de cupos y CRUD
class ListaReserva {
private:
    NodoReserva* head;
    int autoincID;

    // Cupos maximos por localidad (constantes)
    static const int MAX_PALCO = 10;
    static const int MAX_TRIBUNA = 20;
    static const int MAX_GENERAL = 60;

public:
    ListaReserva();
    ~ListaReserva();

    bool agregarReserva(const string&, const string&, const string&,
                        const string&, const string&, int&);
    bool buscarPorID(int id);
    NodoReserva* obtenerPorID(int id);
    void mostrarReservas();
    bool eliminarPorID(int id);
    int contarAsientosPorID(int id);
    int contarAsientosPorCedula(const string& cedula);

    // NUEVO: contar asientos ocupados por localidad
    int asientosOcupadosLocalidad(const string& localidad);

    void guardarEnArchivo(const string& filename);
    void cargarDesdeArchivo(const string& filename);
};
