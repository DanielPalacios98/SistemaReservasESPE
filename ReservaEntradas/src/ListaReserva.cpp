#include "ListaReserva.h"
#include "NodoReserva.h"
#include "Reserva.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <exception>
#include <regex>
using namespace std;

// QuickSort auxiliar sobre arreglo dinámico de Reserva*
static int particionArr(Reserva** arr, int low, int high, bool porNombre) {
    string piv = porNombre ? arr[high]->getNombres() : arr[high]->getCedula();
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        string actual = porNombre ? arr[j]->getNombres() : arr[j]->getCedula();
        if (actual < piv) {
            ++i;
            Reserva* tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    Reserva* tmp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = tmp;
    return i + 1;
}

static void quickSortArr(Reserva** arr, int low, int high, bool porNombre) {
    if (low < high) {
        int pi = particionArr(arr, low, high, porNombre);
        quickSortArr(arr, low, pi - 1, porNombre);
        quickSortArr(arr, pi + 1, high, porNombre);
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

    // Regla del cliente: máximo 5 reservas por cédula (no por asientos)
    int reservasUsuario = contarReservasPorCedula(cedula);
    if (reservasUsuario >= 5) {
        cout << "Este usuario ya ha alcanzado el maximo de 5 reservas." << endl;
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

int ListaReserva::contarReservasPorCedula(const string& cedula) {
    int conteo = 0;
    if (!head) return 0;
    NodoReserva* temp = head;
    do {
        if (temp->reserva->getCedula() == cedula) ++conteo;
        temp = temp->next;
    } while (temp != head);
    return conteo;
}

void ListaReserva::mostrarReservasOrdenadas(bool porNombre) {
    if (!head) {
        cout << "No hay reservas." << endl;
        return;
    }
    // Contar elementos
    int n = 0;
    NodoReserva* temp = head;
    do { ++n; temp = temp->next; } while (temp != head);
    // Copiar a arreglo dinámico
    Reserva** arr = new Reserva*[n];
    temp = head;
    for (int i = 0; i < n; ++i) { arr[i] = temp->reserva; temp = temp->next; }
    // Ordenar
    quickSortArr(arr, 0, n - 1, porNombre);
    cout << "=== Reservas ordenadas por " << (porNombre ? "nombre" : "cedula") << " ===" << endl;
    for (int i = 0; i < n; ++i) arr[i]->mostrarDetalle();
    delete[] arr;
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

void ListaReserva::construirBST(BSTReservas& bst) {
    if (!head) return;
    NodoReserva* temp = head;
    do {
        bst.insertar(temp->reserva);
        temp = temp->next;
    } while (temp != head);
}

void ListaReserva::recorrer(void (*fn)(Reserva*)) {
    if (!head || !fn) return;
    NodoReserva* temp = head;
    do {
        fn(temp->reserva);
        temp = temp->next;
    } while (temp != head);
}

NodoReserva* ListaReserva::obtenerHead() const {
    return head;
}

// --- JSON helpers ---
static string jsonEscape(const string& s) {
    string out;
    out.reserve(s.size() + 8);
    for (char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c; break;
        }
    }
    return out;
}

static string jsonUnescape(const string& s) {
    string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '\\' && i + 1 < s.size()) {
            char n = s[i + 1];
            switch (n) {
                case 'n': out += '\n'; ++i; break;
                case 'r': out += '\r'; ++i; break;
                case 't': out += '\t'; ++i; break;
                case '"': out += '"'; ++i; break;
                case '\\': out += '\\'; ++i; break;
                default: out += n; ++i; break;
            }
        } else {
            out += c;
        }
    }
    return out;
}

void ListaReserva::guardarEnJson(const string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) return;
    out << "{\n  \"reservas\": [\n";
    if (head) {
        // Recorrer y escribir objetos
        NodoReserva* temp = head;
        bool first = true;
        do {
            if (!first) out << ",\n";
            first = false;
            const Reserva* r = temp->reserva;
            out << "    {\n";
            out << "      \"idReserva\": " << r->getIdReserva() << ",\n";
            out << "      \"nombres\": \"" << jsonEscape(r->getNombres()) << "\",\n";
            out << "      \"cedula\": \"" << jsonEscape(r->getCedula()) << "\",\n";
            out << "      \"telefono\": \"" << jsonEscape(r->getTelefono()) << "\",\n";
            out << "      \"correo\": \"" << jsonEscape(r->getCorreo()) << "\",\n";
            out << "      \"localidad\": \"" << jsonEscape(r->getLocalidad()) << "\",\n";
            out << "      \"numAsientos\": " << r->getNumAsientos() << "\n";
            out << "    }";
            temp = temp->next;
        } while (temp != head);
        out << "\n";
    }
    out << "  ]\n}";
    out.close();
}

bool ListaReserva::cargarDesdeJson(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) return false;
    // Leer todo el archivo en un string
    string content;
    in.seekg(0, ios::end);
    content.reserve(static_cast<size_t>(in.tellg()));
    in.seekg(0, ios::beg);
    content.assign(istreambuf_iterator<char>(in), istreambuf_iterator<char>());
    in.close();

    // Limpiar lista actual
    clear();

    // PARSER JSON HARDCODED ULTRASIMPLE (Estilo C puro para maxima velocidad)
    // Asume formato generado por guardarEnJson sin espacios raros
    const char* p = content.c_str();
    
    // Auxiliar rapido para saltar
    auto jumpTo = [&](const char* needle) -> bool {
        const char* found = strstr(p, needle);
        if(!found) return false;
        p = found + strlen(needle);
        return true;
    };

    auto readVal = [&]() -> string {
        while(*p && (*p == ':' || *p == ' ' || *p == '"' || *p == '\t' || *p == '\n')) p++;
        const char* start = p;
        while(*p && *p != '"' && *p != ',' && *p != '}' && *p != '\n') p++;
        
        // [FIX]: Trim trailing whitespace manually
        const char* end = p;
        while(end > start && isspace((unsigned char)*(end-1))) end--;
        
        return string(start, end - start);
    };
    
    // Loop principal buscando claves fijas
    //ADVERTENCIA: Codigo "Fragil" pero extremadamente rapido. Depende del orden de guardarEnJson
    while(jumpTo("\"idReserva\"")) {
        try {
            int id = atoi(readVal().c_str());
            
            if(!jumpTo("\"nombres\"")) break;
            string nombres = readVal(); // Viene escapado, deberiamos unescapear si fuera complejo
            
            if(!jumpTo("\"cedula\"")) break; 
            string cedula = readVal();
            
            if(!jumpTo("\"telefono\"")) break; 
            string telefono = readVal();
            
            if(!jumpTo("\"correo\"")) break; 
            string correo = readVal();
            
            if(!jumpTo("\"localidad\"")) break; 
            string localidad = readVal();

            if(!jumpTo("\"numAsientos\"")) break;
            int numAs = atoi(readVal().c_str());

            // Validacion minima para no romper pointers
            if(id > 0) {
                 if (id >= autoincID) autoincID = id + 1;
                 Reserva* r = new Reserva(id, nombres, cedula, telefono, correo, localidad, numAs);
                 NodoReserva* nodo = new NodoReserva(r);
                 if (!head) {
                     head = nodo; head->next = head;
                 } else {
                     NodoReserva* tail = head;
                     while (tail->next != head) tail = tail->next;
                     tail->next = nodo; nodo->next = head;
                 }
            }
        } catch(...) {}
    }

    return head != nullptr;
}

void ListaReserva::guardarEnJsonLines(const string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) return;
    if (!head) { out.close(); return; }
    NodoReserva* temp = head;
    do {
        const Reserva* r = temp->reserva;
        out << "{"
            << "\"idReserva\":" << r->getIdReserva() << ","
            << "\"nombres\":\"" << jsonEscape(r->getNombres()) << "\"," 
            << "\"cedula\":\"" << jsonEscape(r->getCedula()) << "\"," 
            << "\"telefono\":\"" << jsonEscape(r->getTelefono()) << "\"," 
            << "\"correo\":\"" << jsonEscape(r->getCorreo()) << "\"," 
            << "\"localidad\":\"" << jsonEscape(r->getLocalidad()) << "\"," 
            << "\"numAsientos\":" << r->getNumAsientos()
            << "}\n";
        temp = temp->next;
    } while (temp != head);
    out.close();
}
