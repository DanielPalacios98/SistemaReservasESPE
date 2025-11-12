#ifndef LISTARESERVA_H
#define LISTARESERVA_H

#include "NodoReserva.h"
#include <string>
using namespace std;

class ListaReserva {
private:
    NodoReserva* cabeza;
    int totalPalco;
    int totalTribuna;
    int totalGeneral;

      // Puntero al primer nodo de la lista
public:
    ListaReserva();       // Constructor
    ~ListaReserva();      // Destructor

    // Operaciones principales basadas en tu UML:
    void agregarReserva(Reserva* nuevaReserva);                     // Inserta una reserva en la lista
    void mostrarReservas();                                         // Muestra todas las reservas
    Reserva* buscarPorCedula(const string& cedula);                 // Busca reserva por cédula
    bool eliminarReserva(const string& cedula);                     // Elimina una reserva por cédula
    bool actualizarReserva(const string& cedula, Reserva* reserva); // Actualiza una reserva existente
    int contarReserva();                                            // Cuenta el total de reservas
    void vaciar();                                                  // Elimina todas las reservas
    bool verificarFecha(const string& fechaActual);                 // Verifica estado según fecha
    // CAMBIO: Se agregan estos métodos para persistencia
    void guardarEnArchivo(const string& nombreArchivo) const;
    void cargarDesdeArchivo(const string& nombreArchivo);

};

#endif // LISTARESERVA_H
