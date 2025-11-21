#include "ListaReserva.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// Constructor
ListaReserva::ListaReserva() : head(nullptr), autoincID(1001) {}
ListaReserva::~ListaReserva() {
    if (!head) return;
    NodoReserva* temp = head->next;
    while (temp != head) {
        NodoReserva* to_delete = temp;
        temp = temp->next;
        delete to_delete;
    }
    delete head;
    head = nullptr;
}

// NUEVO: cuenta los asientos ocupados de una localidad
int ListaReserva::asientosOcupadosLocalidad(const string& localidad) {
    int total = 0;
    if (!head) return 0;
    NodoReserva* temp = head;
    do {
        if (temp->reserva->getLocalidad() == localidad)
            total += temp->reserva->getNumAsientos();
        temp = temp->next;
    } while (temp != head);
    return total;
}

// Agregar reserva valida cupo por localidad y por usuario
bool ListaReserva::agregarReserva(const string& nombres, const string& cedula,
                                  const string& telefono, const string& correo,
                                  const string& localidad, int& asientos) {
    // Validacion cupo total por localidad
    int maximo = 0;
    if (localidad == "palco") maximo = MAX_PALCO;
    else if (localidad == "tribuna") maximo = MAX_TRIBUNA;
    else if (localidad == "general") maximo = MAX_GENERAL;
    int ocupados = asientosOcupadosLocalidad(localidad);
    if (ocupados + asientos > maximo) {
        cout << "No hay suficientes asientos disponibles en " << localidad << ". Cupo completo o insuficiente." << endl;
        return false;
    }

    // Validar maximo 5 asientos por usuario
    int totalAsientos = 0;
    NodoReserva* temp = head;
    do {
        if (temp && temp->reserva->getCedula() == cedula)
            totalAsientos += temp->reserva->getNumAsientos();
        if (temp) temp = temp->next;
    } while (temp && temp != head);
    if (totalAsientos + asientos > 5) {
        cout << "Este usuario ya ha reservado el maximo permitido (5 asientos)." << endl;
        return false;
    }

    // Registrar la reserva
    Reserva* nueva = new Reserva(autoincID++, nombres, cedula, telefono, correo, localidad, asientos);
    NodoReserva* nodo = new NodoReserva(nueva);
    if (!head) {
        head = nodo;
        head->next = head;
    } else {
        NodoReserva* tail = head;
        while (tail->next != head)
            tail = tail->next;
        tail->next = nodo;
        nodo->next = head;
    }
    cout << "Reserva almacenada con ID: " << nueva->getIdReserva() << endl;
    return true;
}

void ListaReserva::mostrarReservas() {
    if (!head) {
        cout << "No hay reservas." << endl;
        return;
    }
    NodoReserva* temp = head;
    do {
        temp->reserva->mostrarDetalle();
        temp = temp->next;
    } while (temp != head);
}

bool ListaReserva::buscarPorID(int id) {
    if (!head) return false;
    NodoReserva* temp = head;
    do {
        if (temp->reserva->getIdReserva() == id) return true;
        temp = temp->next;
    } while (temp != head);
    return false;
}

NodoReserva* ListaReserva::obtenerPorID(int id) {
    if (!head) return nullptr;
    NodoReserva* temp = head;
    do {
        if (temp->reserva->getIdReserva() == id) return temp;
        temp = temp->next;
    } while (temp != head);
    return nullptr;
}

bool ListaReserva::eliminarPorID(int id) {
    if (!head) return false;
    NodoReserva* prev = head, *curr = head;
    do {
        if (curr->reserva->getIdReserva() == id) {
            if (curr == head && curr->next == head) {
                delete curr;
                head = nullptr;
                return true;
            }
            if (curr == head) {
                NodoReserva* tail = head;
                while (tail->next != head) tail = tail->next;
                head = head->next;
                tail->next = head;
                delete curr;
                return true;
            }
            prev->next = curr->next;
            delete curr;
            return true;
        }
        prev = curr;
        curr = curr->next;
    } while (curr != head);
    return false;
}

int ListaReserva::contarAsientosPorID(int id) {
    int total = 0;
    if (!head) return 0;
    NodoReserva* temp = head;
    do {
        if (temp->reserva->getIdReserva() == id)
            total += temp->reserva->getNumAsientos();
        temp = temp->next;
    } while (temp != head);
    return total;
}

int ListaReserva::contarAsientosPorCedula(const string& cedula) {
    int total = 0;
    if (!head) return 0;
    NodoReserva* temp = head;
    do {
        if (temp->reserva->getCedula() == cedula)
            total += temp->reserva->getNumAsientos();
        temp = temp->next;
    } while (temp != head);
    return total;
}

void ListaReserva::guardarEnArchivo(const string& filename) {
    ofstream out(filename);
    if (!head) return;
    NodoReserva* temp = head;
    do {
        out << temp->reserva->getIdReserva() << ','
            << temp->reserva->getNombres() << ','
            << temp->reserva->getCedula() << ','
            << temp->reserva->getTelefono() << ','
            << temp->reserva->getCorreo() << ','
            << temp->reserva->getLocalidad() << ','
            << temp->reserva->getNumAsientos() << '\n';
        temp = temp->next;
    } while (temp != head);
    out.close();
}

void ListaReserva::cargarDesdeArchivo(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) return;
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string id, nombre, ced, tel, mail, loc, numAs;
        getline(ss, id, ',');
        getline(ss, nombre, ',');
        getline(ss, ced, ',');
        getline(ss, tel, ',');
        getline(ss, mail, ',');
        getline(ss, loc, ',');
        getline(ss, numAs, '\n');
        int idn = stoi(id);
        int nAs = stoi(numAs);
        if (idn >= autoincID) autoincID = idn + 1;
        Reserva* nueva = new Reserva(idn, nombre, ced, tel, mail, loc, nAs);
        NodoReserva* nodo = new NodoReserva(nueva);
        if (!head) {
            head = nodo;
            head->next = head;
        } else {
            NodoReserva* tail = head;
            while (tail->next != head)
                tail = tail->next;
            tail->next = nodo;
            nodo->next = head;
        }
    }
    in.close();
}
