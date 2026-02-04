#include "ListaReserva.h"
#include "NodoReserva.h"
#include "Reserva.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <exception>
using namespace std;

static Reserva* getArrElem(Reserva** base, int index) {
    return *(base + index);
}

static void setArrElem(Reserva** base, int index, Reserva* value) {
    *(base + index) = value;
}

static void trimInPlace(string& s) {
    while (!s.empty() && isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin());
    while (!s.empty() && isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
}

static bool isDigitChar(char c) {
    return isdigit(static_cast<unsigned char>(c)) != 0;
}

// QuickSort auxiliar sobre arreglo dinámico de Reserva*
static int particionArr(Reserva** arr, int low, int high, bool porNombre) {
    Reserva* pivR = getArrElem(arr, high);
    string piv = porNombre ? pivR->getNombres() : pivR->getCedula();
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        Reserva* actR = getArrElem(arr, j);
        string actual = porNombre ? actR->getNombres() : actR->getCedula();
        if (actual < piv) {
            ++i;
            Reserva* tmp = getArrElem(arr, i);
            setArrElem(arr, i, actR);
            setArrElem(arr, j, tmp);
        }
    }
    Reserva* tmp = getArrElem(arr, i + 1);
    setArrElem(arr, i + 1, pivR);
    setArrElem(arr, high, tmp);
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

    // Regla del cliente: máximo 5 asientos por cédula (suma de asientos)
    int asientosUsuario = contarAsientosPorCedula(cedula);
    if (asientosUsuario + asientos > 5) {
        cout << "Este usuario ya ha alcanzado el maximo de 5 asientos." << endl;
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
    void* mem = ::operator new(sizeof(Reserva*) * static_cast<size_t>(n));
    Reserva** arr = static_cast<Reserva**>(mem);
    temp = head;
    for (int i = 0; i < n; ++i) { *(arr + i) = temp->reserva; temp = temp->next; }
    // Ordenar
    quickSortArr(arr, 0, n - 1, porNombre);
    cout << "=== Reservas ordenadas por " << (porNombre ? "nombre" : "cedula") << " ===" << endl;
    for (int i = 0; i < n; ++i) getArrElem(arr, i)->mostrarDetalle();
    ::operator delete(mem);
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

        trimInPlace(id);
        trimInPlace(nombre);
        trimInPlace(ced);
        trimInPlace(tel);
        trimInPlace(mail);
        trimInPlace(loc);
        trimInPlace(numAs);

        if (id.empty() || numAs.empty() || nombre.empty() || ced.empty()) {
            cout << "Aviso: linea de archivo ignorada por formato invalido: '" << line << "'\n";
            continue;
        }

        bool idDigits = all_of(id.begin(), id.end(), isDigitChar);
        bool numAsDigits = all_of(numAs.begin(), numAs.end(), isDigitChar);
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
    const char* data = s.c_str();
    const size_t len = s.size();
    for (size_t i = 0; i < len; ++i) {
        char c = *(data + i);
        if (c == '\\' && i + 1 < len) {
            char n = *(data + i + 1);
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
    out << "{\n  \"reservas\": " << static_cast<char>(91) << "\n";
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
    out << "  " << static_cast<char>(93) << "\n}";
    out.close();
}

static bool isJsonSpace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static bool findKeyInRange(const string& content, size_t rangeStart, size_t rangeEnd, const string& key, size_t& outPos) {
    const string needle = string("\"") + key + "\"";
    size_t pos = content.find(needle, rangeStart);
    if (pos == string::npos) return false;
    if (pos >= rangeEnd) return false;
    outPos = pos;
    return true;
}

static bool parseJsonStringField(const string& content, size_t rangeStart, size_t rangeEnd, const string& key, string& outValue) {
    size_t keyPos = 0;
    if (!findKeyInRange(content, rangeStart, rangeEnd, key, keyPos)) return false;
    const string needle = string("\"") + key + "\"";
    size_t pos = content.find(':', keyPos + needle.size());
    if (pos == string::npos || pos >= rangeEnd) return false;
    ++pos;
    while (pos < rangeEnd && isJsonSpace(content.at(pos))) ++pos;
    if (pos >= rangeEnd || content.at(pos) != '"') return false;
    ++pos;

    string raw;
    while (pos < rangeEnd) {
        const char c = content.at(pos);
        if (c == '\\') {
            if (pos + 1 < rangeEnd) {
                raw.push_back('\\');
                raw.push_back(content.at(pos + 1));
                pos += 2;
                continue;
            }
            return false;
        }
        if (c == '"') break;
        raw.push_back(c);
        ++pos;
    }
    outValue = jsonUnescape(raw);
    return true;
}

static bool parseJsonIntField(const string& content, size_t rangeStart, size_t rangeEnd, const string& key, int& outValue) {
    size_t keyPos = 0;
    if (!findKeyInRange(content, rangeStart, rangeEnd, key, keyPos)) return false;
    const string needle = string("\"") + key + "\"";
    size_t pos = content.find(':', keyPos + needle.size());
    if (pos == string::npos || pos >= rangeEnd) return false;
    ++pos;
    while (pos < rangeEnd && isJsonSpace(content.at(pos))) ++pos;
    if (pos >= rangeEnd) return false;

    bool any = false;
    int value = 0;
    while (pos < rangeEnd) {
        const char c = content.at(pos);
        if (!isDigitChar(c)) break;
        any = true;
        value = (value * 10) + (static_cast<int>(c) - static_cast<int>('0'));
        ++pos;
    }
    if (!any) return false;
    outValue = value;
    return true;
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

    size_t pos = 0;
    while (true) {
        const size_t objStart = content.find('{', pos);
        if (objStart == string::npos) break;
        const size_t objEnd = content.find('}', objStart + 1);
        if (objEnd == string::npos) break;

        pos = objEnd + 1;
        const size_t rangeStart = objStart;
        const size_t rangeEnd = objEnd + 1;

        size_t dummy = 0;
        if (!findKeyInRange(content, rangeStart, rangeEnd, "idReserva", dummy)) continue;

        int id = 0;
        int numAs = 0;
        string nombres;
        string cedula;
        string telefono;
        string correo;
        string localidad;

        if (!parseJsonIntField(content, rangeStart, rangeEnd, "idReserva", id)) continue;
        if (!parseJsonStringField(content, rangeStart, rangeEnd, "nombres", nombres)) continue;
        if (!parseJsonStringField(content, rangeStart, rangeEnd, "cedula", cedula)) continue;
        if (!parseJsonStringField(content, rangeStart, rangeEnd, "telefono", telefono)) continue;
        if (!parseJsonStringField(content, rangeStart, rangeEnd, "correo", correo)) continue;
        if (!parseJsonStringField(content, rangeStart, rangeEnd, "localidad", localidad)) continue;
        if (!parseJsonIntField(content, rangeStart, rangeEnd, "numAsientos", numAs)) continue;

        // Validaciones básicas consientes con TXT
        if (!Reserva::validarCedula(cedula) || !Reserva::validarNombres(nombres) ||
            !Reserva::validarTelefono(telefono) || !Reserva::validarCorreo(correo) ||
            !Reserva::validarLocalidad(localidad) || numAs < 1 || numAs > 5) {
            cout << "Aviso: objeto JSON ignorado por validacion: id=" << id << "\n";
            continue;
        }

        if (id >= autoincID) autoincID = id + 1;

        Reserva* r = new Reserva(id, nombres, cedula, telefono, correo, localidad, numAs);
        NodoReserva* nodo = new NodoReserva(r);
        if (!head) {
            head = nodo;
            head->next = head;
        } else {
            NodoReserva* tail = head;
            while (tail->next != head) tail = tail->next;
            tail->next = nodo;
            nodo->next = head;
        }
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
