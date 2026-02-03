#include "Controller.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include "../include/JsonReservaRepository.h"
#include "../include/MongoReservaRepository.h"
#include "../include/Config.h"

using namespace std;

Controller::Controller(ConsoleView* v)
    : tablaUsuarios(), reservas(), arbol(), view(v), repo(nullptr) {
    // Selección por configuración
    Config cfg = Config::load("config.json");
    if (cfg.backend == "mongo" && !cfg.mongoUri.empty() && !cfg.mongoDb.empty() && !cfg.mongoCollection.empty()) {
        repo = new MongoReservaRepository(cfg.mongoUri, cfg.mongoDb, cfg.mongoCollection);
        if (!repo->cargar(reservas)) {
            delete repo;
            repo = new JsonReservaRepository();
            repo->cargar(reservas);
        }
    } else {
        repo = new JsonReservaRepository();
        repo->cargar(reservas);
    }
    reservas.construirBST(arbol);
}

Controller::Controller(ConsoleView* v, IReservaRepository* repository)
    : tablaUsuarios(), reservas(), arbol(), view(v), repo(repository) {
    if (!repo) repo = new JsonReservaRepository();
    if (!repo->cargar(reservas)) {
        // Fallback a JSON en caso de error de carga
        delete repo;
        repo = new JsonReservaRepository();
        repo->cargar(reservas);
    }
    reservas.construirBST(arbol);
}

void Controller::run() {
    int op = -1;
    while (op != 0) {
        view->mostrarMensaje("\n=== Menu de Reservas v2 ===");
        view->mostrarMensaje("1. Registrar nueva reserva");
        view->mostrarMensaje("2. Mostrar reservas");
        view->mostrarMensaje("3. Eliminar reserva por ID");
        view->mostrarMensaje("4. Mostrar reservas ordenadas por nombre");
        view->mostrarMensaje("5. Mostrar reservas ordenadas por cedula");
        view->mostrarMensaje("6. Ordenar caracteres del nombre (Shell Sort)");
        view->mostrarMensaje("7. Buscar reserva por ID (BST)");
        view->mostrarMensaje("8. Buscar reserva por nombre (Busqueda Binaria)");
        view->mostrarMensaje("9. Buscar reserva por telefono (Busqueda Lineal)");
        view->mostrarMensaje("0. Salir");
        op = view->pedirOpcionMenu(0, 9);

        if (op == 1) {
            string nombres, cedula, telefono, correo, localidad;
            int asientos = 1;

            do {
                cedula = view->pedirLinea("Cedula (10 digitos validos para Ecuador): ");
                if (!Reserva::validarCedula(cedula))
                    view->mostrarMensaje(" Error: Cedula ecuatoriana invalida. Deben ser 10 digitos correctos.");
            } while (!Reserva::validarCedula(cedula));

            const Usuario* uExistente = nullptr;
            if (tablaUsuarios.existe(cedula)) {
                uExistente = tablaUsuarios.obtener(cedula);
                if (uExistente) {
                    nombres  = uExistente->getNombres();
                    telefono = uExistente->getTelefono();
                    correo   = uExistente->getCorreo();
                    view->mostrarMensaje("Usuario encontrado en la tabla:");
                    view->mostrarMensaje(string(" Nombre  : ") + nombres);
                    view->mostrarMensaje(string(" Telefono: ") + telefono);
                    view->mostrarMensaje(string(" Correo  : ") + correo);
                } else {
                    view->mostrarMensaje("Aviso: cedula existe en hash pero no se pudo recuperar datos.");
                }
            } else {
                view->mostrarMensaje("Nuevo usuario. Registrando en la tabla de usuarios...");

                do {
                    nombres = view->pedirLinea("Nombres: ");
                    if (!Reserva::validarNombres(nombres))
                        view->mostrarMensaje(" Error: Escriba nombre y apellido, cada palabra con Mayuscula inicial y resto minusculas, sin numeros ni caracteres especiales.");
                } while (!Reserva::validarNombres(nombres));

                do {
                    telefono = view->pedirLinea("Telefono (10 digitos, inicia con 09): ");
                    if (!Reserva::validarTelefono(telefono))
                        view->mostrarMensaje(" Error: Telefono invalido. Debe tener 10 digitos y empezar en 09.");
                } while (!Reserva::validarTelefono(telefono));

                do {
                    correo = view->pedirLinea("Correo: ");
                    if (!Reserva::validarCorreo(correo))
                        view->mostrarMensaje(" Error: Formato de correo invalido. Use ejemplo@dominio.com");
                } while (!Reserva::validarCorreo(correo));

                Usuario u(cedula, nombres, telefono, correo);
                if (!tablaUsuarios.insertar(u)) {
                    view->mostrarMensaje(" Aviso: No se pudo registrar el usuario (cedula duplicada en hash)." );
                }
            }

            do {
                localidad = view->pedirLinea("Localidad (palco/tribuna/general): ");
                if (!Reserva::validarLocalidad(localidad))
                    view->mostrarMensaje(" Error: Debe ser 'palco', 'tribuna' o 'general'.");
            } while (!Reserva::validarLocalidad(localidad));

            while (true) {
                view->mostrarMensaje("Cantidad de asientos (1-5): ");
                int temp;
                if (cin >> temp && temp >= 1 && temp <= 5) {
                    view->limpiarBuffer();
                    asientos = temp;
                    break;
                } else {
                    view->limpiarBuffer();
                    view->mostrarMensaje(" Error: Debe ingresar un numero entre 1 y 5.");
                }
            }

            Reserva* nueva = reservas.agregarReserva(nombres, cedula, telefono, correo, localidad, asientos);
            if (nueva != nullptr) {
                arbol.insertar(nueva);
                if (!repo->guardar(reservas)) {
                    // Fallback a JSON si falla
                    JsonReservaRepository jsonRepo;
                    jsonRepo.guardar(reservas);
                }
            }
        }

        if (op == 2) {
            if (!repo->cargar(reservas)) {
                JsonReservaRepository jsonRepo;
                jsonRepo.cargar(reservas);
            }
            reservas.mostrarReservas();
        }

        if (op == 3) {
            int idelim = view->pedirIDValido();
            if (reservas.eliminarPorID(idelim)) {
                view->mostrarMensaje(string("Reserva ID ") + to_string(idelim) + " eliminada.");
                repo->guardar(reservas);

                // Reconstruir BST de forma segura (sin copiar árboles)
                arbol.clear();
                reservas.construirBST(arbol);
            } else {
                view->mostrarMensaje("No se encontro esa reserva.");
            }
        }

        if (op == 4) {
            if (!repo->cargar(reservas)) {
                JsonReservaRepository jsonRepo;
                jsonRepo.cargar(reservas);
            }
            reservas.mostrarReservasOrdenadas(true);
        }

        if (op == 5) {
            if (!repo->cargar(reservas)) {
                JsonReservaRepository jsonRepo;
                jsonRepo.cargar(reservas);
            }
            reservas.mostrarReservasOrdenadas(false);
        }

        if (op == 6) {
            if (!repo->cargar(reservas)) {
                JsonReservaRepository jsonRepo;
                jsonRepo.cargar(reservas);
            }
            NodoReserva* head = reservas.obtenerHead();
            if (!head) {
                view->mostrarMensaje("No hay reservas para procesar desde el archivo.");
            } else {
                view->mostrarMensaje("Ordenando primer nombre de cada reserva (caracteres) usando Shell Sort:");
                NodoReserva* tmp = head;
                do {
                    string nombresTmp = tmp->reserva->getNombres();
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
                    tmp = tmp->next;
                } while (tmp != head);
            }
        }

        if (op == 7) {
            int idBuscar;
            cout << "Ingrese ID a buscar: ";
            cin >> idBuscar;
            view->limpiarBuffer();
            Reserva* r = arbol.buscar(idBuscar);
            if (r) {
                view->mostrarMensaje("Reserva encontrada:");
                r->mostrarDetalle();
            } else {
                view->mostrarMensaje("No se encontro reserva con ese ID.");
            }
        }

        if (op == 8) {
            repo->cargar(reservas);
            NodoReserva* head = reservas.obtenerHead();
            if (!head) {
                view->mostrarMensaje("No hay reservas para buscar.");
            } else {
                string nombreBuscar = view->pedirLinea("Ingrese el nombre completo a buscar: ");
                int encontrados = 0;
                NodoReserva* tmp = head;
                do {
                    if (tmp->reserva->getNombres() == nombreBuscar) {
                        if (encontrados == 0)
                            view->mostrarMensaje(string("Reservas encontradas con el nombre '") + nombreBuscar + "':");
                        tmp->reserva->mostrarDetalle();
                        view->mostrarMensaje("---");
                        ++encontrados;
                    }
                    tmp = tmp->next;
                } while (tmp != head);
                if (encontrados == 0) view->mostrarMensaje("No se encontro reserva con ese nombre.");
            }
        }

        if (op == 9) {
            if (!reservas.cargarDesdeJson("reservas.json")) {
                reservas.recargarDesdeArchivo("reservas.txt");
            }
            NodoReserva* head = reservas.obtenerHead();
            if (!head) {
                view->mostrarMensaje("No hay reservas para buscar.");
            } else {
                string telefonoBuscar = view->pedirLinea("Ingrese telefono a buscar: ");
                // Trim
                telefonoBuscar.erase(telefonoBuscar.begin(),
                    find_if(telefonoBuscar.begin(), telefonoBuscar.end(),
                            [](unsigned char ch){ return !isspace(ch); }));
                telefonoBuscar.erase(
                    find_if(telefonoBuscar.rbegin(), telefonoBuscar.rend(),
                            [](unsigned char ch){ return !isspace(ch); }).base(),
                    telefonoBuscar.end());

                view->mostrarMensaje(string("Buscando telefono: ") + telefonoBuscar);
                int encontrados = 0;
                NodoReserva* tmp = head;
                do {
                    if (tmp->reserva->getTelefono() == telefonoBuscar) {
                        ++encontrados;
                        if (encontrados == 1)
                            view->mostrarMensaje(string("Reservas encontradas con el telefono '") + telefonoBuscar + "':");
                        tmp->reserva->mostrarDetalle();
                        view->mostrarMensaje("---");
                    }
                    tmp = tmp->next;
                } while (tmp != head);
                view->mostrarMensaje(string("Encontradas: ") + to_string(encontrados));
                if (encontrados == 0) view->mostrarMensaje("No se encontro reserva con ese telefono.");
            }
        }
    }

    view->mostrarMensaje("Guardando y saliendo...");
    if (!repo->guardar(reservas)) {
        JsonReservaRepository jsonRepo;
        jsonRepo.guardar(reservas);
    }
}
