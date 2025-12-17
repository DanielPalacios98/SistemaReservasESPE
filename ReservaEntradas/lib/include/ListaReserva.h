#pragma once
#include "NodoReserva.h"
#include <string>
#include <vector>
using namespace std;

// Cupos máximos por localidad
const int MAX_PALCO   = 10;
const int MAX_TRIBUNA = 20;
const int MAX_GENERAL = 30;

class ListaReserva {
private:
    NodoReserva* head;   // Lista circular
    int autoincID;       // ID autoincremental para nuevas reservas

    // Suma de asientos ocupados en una localidad (palco/tribuna/general)
    int asientosOcupadosLocalidad(const string& localidad);

public:
    ListaReserva();
    ~ListaReserva();

    // Agrega una reserva si hay cupo y no supera 5 asientos por cédula
    Reserva* agregarReserva(const string& nombres,
                            const string& cedula,
                            const string& telefono,
                            const string& correo,
                            const string& localidad,
                            int asientos);

    void mostrarReservas();

    bool buscarPorID(int id);
    NodoReserva* obtenerPorID(int id);
    bool eliminarPorID(int id);

    int contarAsientosPorID(int id);
    int contarAsientosPorCedula(const string& cedula);

    // Convierte la lista circular en un vector de punteros para ordenamiento
    vector<Reserva*> obtenerReservasComoVector();

    // Obtiene vector ordenado por nombre para búsqueda binaria
    vector<Reserva*> obtenerReservasOrdenadasPorNombre();

    // Muestra reservas ordenadas por nombre (true) o por cedula (false)
    void mostrarReservasOrdenadas(bool porNombre);

    // Persistencia actual en TXT (CSV simple)
    void guardarEnArchivo(const string& filename);
    void cargarDesdeArchivo(const string& filename);
    void clear();
    void recargarDesdeArchivo(const string& filename);
};
