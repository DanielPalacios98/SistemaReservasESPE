#include "include/JsonReservaRepository.h"
#include "include/ListaReserva.h"

bool JsonReservaRepository::cargar(ListaReserva& lista) {
    if (!lista.cargarDesdeJson(jsonFile)) {
        // fallback a TXT por compatibilidad
        lista.recargarDesdeArchivo(txtFile);
    }
    return true;
}

bool JsonReservaRepository::guardar(const ListaReserva& lista) {
    // Necesitamos métodos no const; hacemos una copia superficial del puntero a lista para reutilizar API
    ListaReserva& ref = const_cast<ListaReserva&>(lista);
    ref.guardarEnJson(jsonFile);
    ref.guardarEnJsonLines(jsonlFile);
    ref.guardarEnArchivo(txtFile);
    return true;
}
