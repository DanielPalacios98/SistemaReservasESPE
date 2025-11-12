#include "ListaReserva.h"
#include "Reserva.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    ListaReserva lista;
    lista.cargarDesdeArchivo("reservas.txt"); // CAMBIO: Carga datos existentes al iniciar
    int opcion;
    do {
        cout << "\n----- Sistema de Reservas de Boletos -----" << endl;
        cout << "1. Agregar reserva" << endl;
        cout << "   Solo se permite reservar un asiento por usuario (por cedula)." << endl;
        cout << "   Cupos maximos: Palco: 10, Tribuna: 20, General: 60." << endl;
        cout << "2. Mostrar reservas" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore(); // Limpiar salto de línea

        if (opcion == 1) {
            Reserva* r = new Reserva();
            string s;
            cout << "\n-- Registro de Nueva Reserva --" << endl;
            cout << "Ingrese Nombres (solo letras y espacios): ";
            getline(cin, s); r->setNombres(s);
            cout << "Ingrese Cedula (exactamente 10 digitos, sin espacios ni letras): ";
            getline(cin, s); r->setCedula(s);
            cout << "Ingrese Telefono (exactamente 10 digitos, debe iniciar con 09): ";
            getline(cin, s); r->setTelefono(s);
            cout << "Ingrese Correo (debe tener '@' y al menos un punto '.'): ";
            getline(cin, s); r->setCorreo(s);
            cout << "Ingrese Localidad (palco, tribuna, general; todo en minusculas): ";
            getline(cin, s); r->setLocalidad(s);
            r->setCantidadAsientos(1); // Se permite sólo uno por reserva
            lista.agregarReserva(r); // Backend hará validaciones y mensajes
        } else if (opcion == 2) {
            cout << "\n-- Listado de Reservas --" << endl;
            lista.mostrarReservas();
        } else if (opcion != 0) {
            cout << "Opcion invalida. Intente nuevamente." << endl;
        }

    } while (opcion != 0);
    lista.guardarEnArchivo("reservas.txt"); // CAMBIO: Guarda datos al salir
    cout << "Gracias por usar el sistema." << endl;
    return 0;
}
