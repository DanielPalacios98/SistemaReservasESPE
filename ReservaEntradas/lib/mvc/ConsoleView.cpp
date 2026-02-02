#include "ConsoleView.h"
#include "../include/Reserva.h"
#include <iostream>
#include <limits>
#include <string>
#include <cctype>

using namespace std;

void ConsoleView::limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int ConsoleView::pedirOpcionMenu(int min, int max) {
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

int ConsoleView::pedirIDValido() {
    int id;
    while (true) {
        cout << "Ingrese ID de reserva: ";
        if (cin >> id && id > 0) {
            limpiarBuffer();
            return id;
        } else {
            limpiarBuffer();
            cout << "ID invalido. Debe ser un numero entero positivo." << endl;
        }
    }
}

string ConsoleView::pedirLinea(const string& prompt) {
    cout << prompt;
    string line;
    getline(cin, line);
    return line;
}

void ConsoleView::mostrarMensaje(const string& msg) {
    cout << msg << endl;
}

// Dependencia ligera a Reserva; imprimimos detalle si no es null
void ConsoleView::mostrarReservaDetalle(Reserva* r) {
    if (!r) return;
    r->mostrarDetalle();
}
