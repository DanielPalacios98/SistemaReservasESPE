#include "ListaReserva.h"
#include "NodoReserva.h"
#include "Reserva.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <exception>
using namespace std;

// QuickSort auxiliar
static int particion(vector<Reserva*>& arr, int low, int high, bool porNombre) {
    string piv = porNombre ? arr[high]->getNombres() : arr[high]->getCedula();
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        string actual = porNombre ? arr[j]->getNombres() : arr[j]->getCedula();
        if (actual < piv) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

static void quickSort(vector<Reserva*>& arr, int low, int high, bool porNombre) {
    if (low < high) {
        int pi = particion(arr, low, high, porNombre);
        quickSort(arr, low, pi - 1, porNombre);
        quickSort(arr, pi + 1, high, porNombre);
    }
}

ListaReserva::ListaReserva()
    : head(nullptr), autoincID(1001) {}

ListaReserva::~ListaReserva() {
    clear();
}

void ListaReserva::clear() {
    if (!head) return;
    NodoReserva* temp = head->next;
    while (temp != head) {
        NodoReserva* to_delete = temp;
        temp = temp->next;
        delete to_delete->reserva;
        delete to_delete;
    }
    delete head->reserva;
    delete head;
    head = nullptr;
    autoincID = 1001;
}

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

Reserva* ListaReserva::agregarReserva(const string& nombres, const string& cedula,
                                      const string& telefono, const string& correo,
                                      const string& localidad, int asientos) {
    int maximo = 0;
    if (localidad == "palco") maximo = MAX_PALCO;
    else if (localidad == "tribuna") maximo = MAX_TRIBUNA;
    else if (localidad == "general") maximo = MAX_GENERAL;

    int ocupados = asientosOcupadosLocalidad(localidad);
    if (ocupados + asientos > maximo) {
        cout << "No hay suficientes asientos disponibles en " << localidad
             << ". Cupo completo o insuficiente." << endl;
        return nullptr;
    }

    int totalAsientos = 0;
    NodoReserva* temp = head;
    if (temp) {
        do {
            if (temp->reserva->getCedula() == cedula)
                totalAsientos += temp->reserva->getNumAsientos();
            temp = temp->next;
        } while (temp && temp != head);
    }

    if (totalAsientos + asientos > 5) {
        cout << "Este usuario ya ha reservado el maximo permitido (5 asientos)." << endl;
        return nullptr;
    }

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
    return nueva;
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
    NodoReserva* prev = head;
    NodoReserva* curr = head;
    do {
        if (curr->reserva->getIdReserva() == id) {
            if (curr == head && curr->next == head) {
                delete curr->reserva;
                delete curr;
                head = nullptr;
                return true;
            }
            if (curr == head) {
                NodoReserva* tail = head;
                while (tail->next != head) tail = tail->next;
                head = head->next;
                tail->next = head;
                delete curr->reserva;
                delete curr;
                return true;
            }
            prev->next = curr->next;
            delete curr->reserva;
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

vector<Reserva*> ListaReserva::obtenerReservasComoVector() {
    vector<Reserva*> vec;
    if (!head) return vec;
    NodoReserva* temp = head;
    do {
        vec.push_back(temp->reserva);
        temp = temp->next;
    } while (temp != head);
    return vec;
}

vector<Reserva*> ListaReserva::obtenerReservasOrdenadasPorNombre() {
    vector<Reserva*> vec = obtenerReservasComoVector();
    sort(vec.begin(), vec.end(), [](Reserva* a, Reserva* b) {
        return a->getNombres() < b->getNombres();
    });
    return vec;
}

void ListaReserva::mostrarReservasOrdenadas(bool porNombre) {
    vector<Reserva*> arr = obtenerReservasComoVector();
    if (arr.empty()) {
        cout << "No hay reservas." << endl;
        return;
    }
    quickSort(arr, 0, static_cast<int>(arr.size()) - 1, porNombre);
    cout << "=== Reservas ordenadas por " << (porNombre ? "nombre" : "cedula") << " ===" << endl;
    for (auto r : arr)
        r->mostrarDetalle();
}

void ListaReserva::guardarEnArchivo(const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) return;
    if (!head) {
        out.close();
        return;
    }
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

        auto trim = [](string& s) {
            while (!s.empty() && isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin());
            while (!s.empty() && isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
        };
        trim(id); trim(nombre); trim(ced); trim(tel); trim(mail); trim(loc); trim(numAs);

        if (id.empty() || numAs.empty() || nombre.empty() || ced.empty()) {
            cout << "Aviso: linea de archivo ignorada por formato invalido: '" << line << "'\n";
            continue;
        }

        bool idDigits = all_of(id.begin(), id.end(), [](char c){ return isdigit(static_cast<unsigned char>(c)); });
        bool numAsDigits = all_of(numAs.begin(), numAs.end(), [](char c){ return isdigit(static_cast<unsigned char>(c)); });
        if (!idDigits || !numAsDigits) {
            cout << "Aviso: linea de archivo ignorada (id o numAs no numerico): '" << line << "'\n";
            continue;
        }

        int idn = 0;
        int nAs = 0;
        try {
            idn = stoi(id);
            nAs = stoi(numAs);
        } catch (const exception& e) {
            cout << "Aviso: stoi fallo para linea, se ignora: '" << line << "' (" << e.what() << ")\n";
            continue;
        }

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

void ListaReserva::recargarDesdeArchivo(const string& filename) {
    clear();
    cargarDesdeArchivo(filename);
}
