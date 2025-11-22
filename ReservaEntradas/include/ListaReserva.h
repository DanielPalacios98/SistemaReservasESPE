#pragma once
#include "NodoReserva.h"
#include <string>
#include <vector>
using namespace std;

class ListaReserva {
private:
    NodoReserva* head;
    int autoincID;

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

    int asientosOcupadosLocalidad(const string& localidad);

    // NUEVAS FUNCIONES PARA ORDENAR Y MOSTRAR
    vector<Reserva*> obtenerReservasComoVector();
    void mostrarReservasOrdenadas(bool porNombre);

    void guardarEnArchivo(const string& filename);
    void cargarDesdeArchivo(const string& filename);// crear un DLL invoca una libreria dinamica con punteros , hacer un menu ordenamiento 1 sort , shell, quishrot , radix , ordenamiento xyz elija la opcion / crear persistencia de datos para todas las nuevas listas /
};
