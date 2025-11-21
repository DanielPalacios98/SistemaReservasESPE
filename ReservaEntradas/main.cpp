#include "ListaReserva.h"
#include <iostream>
#include <limits>
using namespace std;

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int pedirOpcionMenu(int min, int max) {
    int op;
    while (true) {
        cout << "Ingrese opcion: ";
        if (cin >> op && op >= min && op <= max) {
            limpiarBuffer();
            return op;
        } else {
            limpiarBuffer();
            cout << "Opcion invalida. Vuelva a intentarlo." << endl;
        }
    }
}

int pedirIDValido() {
    int idelim;
    while (true) {
        cout << "Ingrese ID de reserva a eliminar: ";
        if (cin >> idelim && idelim > 0) {
            limpiarBuffer();
            return idelim;
        } else {
            limpiarBuffer();
            cout << "ID invalido. Debe ser un numero entero positivo." << endl;
        }
    }
}

int main() {
    ListaReserva reservas;
    reservas.cargarDesdeArchivo("reservas.txt");

    int op = -1;
    while (op != 0) {
        cout << "\n=== Menu de Reservas ===" << endl;
        cout << "1. Registrar nueva reserva" << endl;
        cout << "2. Mostrar reservas" << endl;
        cout << "3. Eliminar reserva por ID" << endl;
        cout << "4. Mostrar reservas ordenadas por nombre" << endl;
        cout << "5. Mostrar reservas ordenadas por cedula" << endl;
        cout << "0. Salir" << endl;
        op = pedirOpcionMenu(0, 5);

        if (op == 1) {
            string nombres, cedula, telefono, correo, localidad;
            int asientos = 1;

            do {
                cout << "Nombres (solo letras y espacios): ";
                getline(cin, nombres);
                if (!Reserva::validarNombres(nombres))
                    cout << " Error: Ingrese solo letras y espacios (sin numeros ni caracteres especiales)." << endl;
            } while (!Reserva::validarNombres(nombres));

            do {
                cout << "Cedula (10 digitos validos para Ecuador): ";
                getline(cin, cedula);
                if (!Reserva::validarCedula(cedula))
                    cout << " Error: Cedula ecuatoriana invalida. Deben ser 10 digitos correctos." << endl;
            } while (!Reserva::validarCedula(cedula));
            
            do {
                cout << "Telefono (10 digitos, inicia con 09): ";
                getline(cin, telefono);
                if (!Reserva::validarTelefono(telefono))
                    cout << " Error: Telefono invalido. Debe tener 10 digitos y empezar en 09." << endl;
            } while (!Reserva::validarTelefono(telefono));

            do {
                cout << "Correo: ";
                getline(cin, correo);
                if (!Reserva::validarCorreo(correo))
                    cout << " Error: Formato de correo invalido. Use ejemplo@dominio.com" << endl;
            } while (!Reserva::validarCorreo(correo));

            do {
                cout << "Localidad (palco/tribuna/general): ";
                getline(cin, localidad);
                if (!Reserva::validarLocalidad(localidad))
                    cout << " Error: Debe ser 'palco', 'tribuna' o 'general'." << endl;
            } while (!Reserva::validarLocalidad(localidad));

            while (true) {
                cout << "Cantidad de asientos (1-5): ";
                if (cin >> asientos && asientos >= 1 && asientos <= 5) {
                    limpiarBuffer();
                    break;
                } else {
                    limpiarBuffer();
                    cout << " Error: Debe ingresar un numero entre 1 y 5." << endl;
                }
            }

            reservas.agregarReserva(nombres, cedula, telefono, correo, localidad, asientos);
            reservas.guardarEnArchivo("reservas.txt");
        }

        if (op == 2) {
            reservas.mostrarReservas();
        }

        if (op == 3) {
            int idelim = pedirIDValido();
            if (reservas.eliminarPorID(idelim)) {
                cout << "Reserva ID " << idelim << " eliminada." << endl;
                reservas.guardarEnArchivo("reservas.txt");
            } else {
                cout << "No se encontro esa reserva." << endl;
            }
        }

        if (op == 4) {
            reservas.mostrarReservasOrdenadas(true); // Ordenar por nombre
        }

        if (op == 5) {
            reservas.mostrarReservasOrdenadas(false); // Ordenar por cedula
        }
    }
    cout << "Guardando y saliendo..." << endl;
    reservas.guardarEnArchivo("reservas.txt");
    return 0;
}
