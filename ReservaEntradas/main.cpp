#include "lib/include/ListaReserva.h"
#include "lib/include/HashTableUsuarios.h"
#include "lib/include/BSTReservas.h"
#include <iostream>
#include <limits>
#include <vector>
#include <cctype>
#include <algorithm>
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
        cout << "Ingrese ID de reserva: ";
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
    HashTableUsuarios tablaUsuarios;
    ListaReserva reservas;
    BSTReservas arbol;

    // Carga inicial desde archivo
    reservas.cargarDesdeArchivo("reservas.txt");

    // Construir BST por ID
    {
        vector<Reserva*> vec = reservas.obtenerReservasComoVector();
        for (auto r : vec) {
            arbol.insertar(r);
        }
    }

    int op = -1;
    while (op != 0) {
        cout << "\n=== Menu de Reservas v2 ===" << endl;
        cout << "1. Registrar nueva reserva" << endl;
        cout << "2. Mostrar reservas" << endl;
        cout << "3. Eliminar reserva por ID" << endl;
        cout << "4. Mostrar reservas ordenadas por nombre" << endl;
        cout << "5. Mostrar reservas ordenadas por cedula" << endl;
        cout << "6. Ordenar caracteres del nombre (Shell Sort)" << endl;
        cout << "7. Buscar reserva por ID (BST)" << endl;
        cout << "8. Buscar reserva por nombre (Busqueda Binaria)" << endl;
        cout << "9. Buscar reserva por telefono (Busqueda Lineal)" << endl;
        cout << "0. Salir" << endl;
        op = pedirOpcionMenu(0, 9);

        // 1) Registrar nueva reserva
        if (op == 1) {
            string nombres, cedula, telefono, correo, localidad;
            int asientos = 1;

            // Cedula
            do {
                cout << "Cedula (10 digitos validos para Ecuador): ";
                getline(cin, cedula);
                if (!Reserva::validarCedula(cedula))
                    cout << " Error: Cedula ecuatoriana invalida. Deben ser 10 digitos correctos." << endl;
            } while (!Reserva::validarCedula(cedula));

            // Usuario en tabla hash
            const Usuario* uExistente = nullptr;
            if (tablaUsuarios.existe(cedula)) {
                uExistente = tablaUsuarios.obtener(cedula);
                if (uExistente) {
                    nombres  = uExistente->getNombres();
                    telefono = uExistente->getTelefono();
                    correo   = uExistente->getCorreo();
                    cout << "Usuario encontrado en la tabla:" << endl;
                    cout << " Nombre  : " << nombres  << endl;
                    cout << " Telefono: " << telefono << endl;
                    cout << " Correo  : " << correo   << endl;
                } else {
                    cout << "Aviso: cedula existe en hash pero no se pudo recuperar datos." << endl;
                }
            } else {
                cout << "Nuevo usuario. Registrando en la tabla de usuarios..." << endl;

                // Nombres
                do {
                    cout << "Nombres: ";
                    getline(cin >> ws, nombres);
                    if (!Reserva::validarNombres(nombres))
                        cout << " Error: Escriba nombre y apellido, cada palabra con Mayuscula inicial y resto minusculas, sin numeros ni caracteres especiales." << endl;
                } while (!Reserva::validarNombres(nombres));

                // Telefono
                do {
                    cout << "Telefono (10 digitos, inicia con 09): ";
                    getline(cin, telefono);
                    if (!Reserva::validarTelefono(telefono))
                        cout << " Error: Telefono invalido. Debe tener 10 digitos y empezar en 09." << endl;
                } while (!Reserva::validarTelefono(telefono));

                // Correo
                do {
                    cout << "Correo: ";
                    getline(cin, correo);
                    if (!Reserva::validarCorreo(correo))
                        cout << " Error: Formato de correo invalido. Use ejemplo@dominio.com" << endl;
                } while (!Reserva::validarCorreo(correo));

                Usuario u(cedula, nombres, telefono, correo);
                if (!tablaUsuarios.insertar(u)) {
                    cout << " Aviso: No se pudo registrar el usuario (cedula duplicada en hash)." << endl;
                }
            }

            // Localidad
            do {
                cout << "Localidad (palco/tribuna/general): ";
                getline(cin, localidad);
                if (!Reserva::validarLocalidad(localidad))
                    cout << " Error: Debe ser 'palco', 'tribuna' o 'general'." << endl;
            } while (!Reserva::validarLocalidad(localidad));

            // Asientos
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

            // Registrar en lista y BST
            Reserva* nueva = reservas.agregarReserva(nombres, cedula, telefono, correo, localidad, asientos);
            if (nueva != nullptr) {
                arbol.insertar(nueva);
                reservas.guardarEnArchivo("reservas.txt");
            }
        }

        // 2) Mostrar reservas
        if (op == 2) {
            reservas.recargarDesdeArchivo("reservas.txt");
            reservas.mostrarReservas();
        }

        // 3) Eliminar por ID
        if (op == 3) {
            int idelim = pedirIDValido();
            if (reservas.eliminarPorID(idelim)) {
                cout << "Reserva ID " << idelim << " eliminada." << endl;
                reservas.guardarEnArchivo("reservas.txt");

                // Reconstruir BST
                BSTReservas nuevoArbol;
                vector<Reserva*> vec = reservas.obtenerReservasComoVector();
                for (auto r : vec) nuevoArbol.insertar(r);
                arbol = nuevoArbol;
            } else {
                cout << "No se encontro esa reserva." << endl;
            }
        }

        // 4) Mostrar ordenadas por nombre
        if (op == 4) {
            reservas.recargarDesdeArchivo("reservas.txt");
            reservas.mostrarReservasOrdenadas(true);
        }

        // 5) Mostrar ordenadas por cedula
        if (op == 5) {
            reservas.recargarDesdeArchivo("reservas.txt");
            reservas.mostrarReservasOrdenadas(false);
        }

        // 6) Shell sort caracteres del primer nombre
        if (op == 6) {
            reservas.recargarDesdeArchivo("reservas.txt");
            vector<Reserva*> vec = reservas.obtenerReservasComoVector();
            if (vec.empty()) {
                cout << "No hay reservas para procesar desde el archivo." << endl;
            } else {
                cout << "Ordenando primer nombre de cada reserva (caracteres) usando Shell Sort:\n";
                for (auto r : vec) {
                    string nombresTmp = r->getNombres();
                    size_t posEspacio = nombresTmp.find(' ');
                    string soloNombre = (posEspacio != string::npos) ? nombresTmp.substr(0, posEspacio) : nombresTmp;
                    int n = static_cast<int>(soloNombre.size());
                    const char* src = soloNombre.c_str();
                    char* caracteres = nullptr;
                    if (n > 0) {
                        caracteres = new char[n];

                        for (int i = 0; i < n; ++i) *(caracteres + i) = *(src + i);

                        for (int gap = n / 2; gap > 0; gap /= 2) {
                            for (int i = gap; i < n; ++i) {
                                char temp = *(caracteres + i);
                                int j = i;
                                while (j >= gap && tolower(*(caracteres + j - gap)) > tolower(temp)) {
                                    *(caracteres + j) = *(caracteres + j - gap);
                                    j -= gap;
                                }
                                *(caracteres + j) = temp;
                            }
                        }

                        *(caracteres + 0) = static_cast<char>(toupper(static_cast<unsigned char>(*(caracteres + 0))));
                        for (int i = 1; i < n; ++i) {
                            *(caracteres + i) = static_cast<char>(tolower(static_cast<unsigned char>(*(caracteres + i))));
                        }
                    }

                    cout << "Original: " << nombresTmp << " | Nombre ordenado: ";
                    if (caracteres) {
                        for (int i = 0; i < n; ++i) cout << *(caracteres + i);
                        delete[] caracteres;
                    }
                    cout << endl;
                }
            }
        }

        // 7) Buscar por ID
        if (op == 7) {
            vector<Reserva*> vec = reservas.obtenerReservasComoVector();
            int idBuscar;
            cout << "Ingrese ID a buscar: ";
            cin >> idBuscar;
            limpiarBuffer();
            bool encontrado = false;
            for (auto r : vec) {
                if (r->getIdReserva() == idBuscar) {
                    cout << "Reserva encontrada:" << endl;
                    r->mostrarDetalle();
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) {
                cout << "No se encontro reserva con ese ID." << endl;
            }
        }

        // 8) Buscar por nombre
        if (op == 8) {
            reservas.recargarDesdeArchivo("reservas.txt");
            vector<Reserva*> vec = reservas.obtenerReservasOrdenadasPorNombre();
            if (vec.empty()) {
                cout << "No hay reservas para buscar.\n";
            } else {
                string nombreBuscar;
                cout << "Ingrese el nombre completo a buscar: ";
                getline(cin, nombreBuscar);

                auto it = lower_bound(vec.begin(), vec.end(), nombreBuscar,
                    [](Reserva* r, const string& val) {
                        return r->getNombres() < val;
                    });

                if (it != vec.end() && (*it)->getNombres() == nombreBuscar) {
                    cout << "Reservas encontradas con el nombre '" << nombreBuscar << "':\n";
                    for (; it != vec.end() && (*it)->getNombres() == nombreBuscar; ++it) {
                        (*it)->mostrarDetalle();
                        cout << "---\n";
                    }
                } else {
                    cout << "No se encontro reserva con ese nombre.\n";
                }
            }
        }

        // 9) Buscar por telefono (lineal)
        if (op == 9) {
            reservas.recargarDesdeArchivo("reservas.txt");
            vector<Reserva*> vec = reservas.obtenerReservasComoVector();
            if (vec.empty()) {
                cout << "No hay reservas para buscar.\n";
            } else {
                string telefonoBuscar;
                cout << "Ingrese telefono a buscar: ";
                getline(cin, telefonoBuscar);    // aquí NO llamamos limpiarBuffer antes

                // Trim espacios
                telefonoBuscar.erase(telefonoBuscar.begin(),
                    find_if(telefonoBuscar.begin(), telefonoBuscar.end(),
                            [](unsigned char ch){ return !isspace(ch); }));
                telefonoBuscar.erase(
                    find_if(telefonoBuscar.rbegin(), telefonoBuscar.rend(),
                            [](unsigned char ch){ return !isspace(ch); }).base(),
                    telefonoBuscar.end());

                cout << "Buscando telefono: " << telefonoBuscar << endl;

                vector<Reserva*> encontradas;
                for (auto r : vec) {
                    if (r->getTelefono() == telefonoBuscar) {
                        encontradas.push_back(r);
                    }
                }

                cout << "Encontradas: " << encontradas.size() << endl;
                if (!encontradas.empty()) {
                    cout << "Reservas encontradas con el telefono '" << telefonoBuscar << "':\n";
                    for (auto r : encontradas) {
                        r->mostrarDetalle();
                        cout << "---\n";
                    }
                } else {
                    cout << "No se encontro reserva con ese telefono.\n";
                }
            }
        }
    } // cierra while(op != 0)

    cout << "Guardando y saliendo..." << endl;
    reservas.guardarEnArchivo("reservas.txt");
    return 0;
}
