#include "ListaReserva.h"
#include <iostream>
#include <algorithm>
#include <fstream>    // CAMBIO: Necesario para archivos
#include <sstream>    // CAMBIO: Necesario para separar datos
using namespace std;

// Inicializa contadores y la cabeza de la lista
ListaReserva::ListaReserva() {
    cabeza = nullptr;
    totalPalco = 0;
    totalTribuna = 0;
    totalGeneral = 0;
}

// Libera memoria y reinicia contadores
ListaReserva::~ListaReserva() {
    vaciar();
}

void ListaReserva::agregarReserva(Reserva* nuevaReserva) {
    if (!nuevaReserva->validarCampos()) {
        cout << "Los datos de la reserva no son validos." << endl;
        return;
    }
    // Evita duplicados por cédula
    if (buscarPorCedula(nuevaReserva->getCedula()) != nullptr) {
        cout << "Ya existe una reserva con esa cedula. No puede reservar más de un asiento." << endl;
        return;
    }
    // Control de cupos por localidad
    string loc = nuevaReserva->getLocalidad();
    std::transform(loc.begin(), loc.end(), loc.begin(), ::tolower);
    if (loc == "palco" && totalPalco >= 10) {
        cout << "Cupo de palco agotado." << endl;
        return;
    } else if (loc == "tribuna" && totalTribuna >= 20) {
        cout << "Cupo de tribuna agotado." << endl;
        return;
    } else if (loc == "general" && totalGeneral >= 60) {
        cout << "Cupo de general agotado." << endl;
        return;
    }
    // Inserta al inicio
    NodoReserva* nuevoNodo = new NodoReserva(nuevaReserva);
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
    // Actualiza el contador correspondiente
    if (loc == "palco") ++totalPalco;
    else if (loc == "tribuna") ++totalTribuna;
    else if (loc == "general") ++totalGeneral;
    cout << "Reserva agregada con éxito." << endl;
}

Reserva* ListaReserva::buscarPorCedula(const string& cedula) {
    NodoReserva* aux = cabeza;
    while (aux != nullptr) {
        if (aux->reserva->getCedula() == cedula) {
            return aux->reserva;
        }
        aux = aux->siguiente;
    }
    return nullptr;
}

void ListaReserva::mostrarReservas() {
    NodoReserva* aux = cabeza;
    while (aux != nullptr) {
        aux->reserva->mostrarDetalle();
        aux = aux->siguiente;
    }
}

void ListaReserva::vaciar() {
    NodoReserva* aux;
    while (cabeza != nullptr) {
        aux = cabeza;
        cabeza = cabeza->siguiente;
        delete aux->reserva;
        delete aux;
    }
    // CAMBIO: reinicio de contadores tras vaciar lista
    totalPalco = 0;
    totalTribuna = 0;
    totalGeneral = 0;
}

// CAMBIO: Método para guardar reservas en archivo txt
void ListaReserva::guardarEnArchivo(const string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    NodoReserva* aux = cabeza;
    while (aux != nullptr) {
        archivo << aux->reserva->getNombres() << ","
                << aux->reserva->getCedula() << ","
                << aux->reserva->getTelefono() << ","
                << aux->reserva->getCorreo() << ","
                << aux->reserva->getLocalidad() << ","
                << aux->reserva->getCantidadAsientos() << std::endl;
        aux = aux->siguiente;
    }
    archivo.close();
}

// CAMBIO: Método para cargar reservas desde archivo txt y actualizar contadores de cupo
void ListaReserva::cargarDesdeArchivo(const string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    std::string linea;
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string nombres, cedula, telefono, correo, localidad, asientosStr;
        std::getline(ss, nombres, ',');
        std::getline(ss, cedula, ',');
        std::getline(ss, telefono, ',');
        std::getline(ss, correo, ',');
        std::getline(ss, localidad, ',');
        std::getline(ss, asientosStr, ',');
        int asientos = std::stoi(asientosStr);
        Reserva* r = new Reserva();
        r->setNombres(nombres);
        r->setCedula(cedula);
        r->setTelefono(telefono);
        r->setCorreo(correo);
        r->setLocalidad(localidad);
        r->setCantidadAsientos(asientos);
        // Validación y control de duplicados
        if (r->validarCampos() && buscarPorCedula(cedula) == nullptr) {
            NodoReserva* nuevoNodo = new NodoReserva(r);
            nuevoNodo->siguiente = cabeza;
            cabeza = nuevoNodo;
            // CAMBIO: suma al contador adecuado
            string loc = localidad;
            std::transform(loc.begin(), loc.end(), loc.begin(), ::tolower);
            if (loc == "palco") ++totalPalco;
            else if (loc == "tribuna") ++totalTribuna;
            else if (loc == "general") ++totalGeneral;
        } else {
            delete r;
        }
    }
    archivo.close();
}

