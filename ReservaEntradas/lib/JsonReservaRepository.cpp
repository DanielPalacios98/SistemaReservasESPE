#include "include/JsonReservaRepository.h"
#include "include/ListaReserva.h"

bool JsonReservaRepository::cargar(ListaReserva& lista) {
    if (!lista.cargarDesdeJson(jsonFile)) {
        // Si falla JSON, iniciamos vacio (o se podria intentar otra logica)
        // Ya no usamos TXT fallback.
        return false;
    }
    return true;
}

bool JsonReservaRepository::guardar(const ListaReserva& lista) {
    // Necesitamos métodos no const; hacemos una copia superficial del puntero a lista para reutilizar API
    ListaReserva& ref = const_cast<ListaReserva&>(lista);
    ref.guardarEnJson(jsonFile);
    // ref.guardarEnJsonLines(jsonlFile); // Deshabilitado legacy
    // ref.guardarEnArchivo(txtFile);     // Deshabilitado legacy
    return true;
}

// Implementación de compatibilidad para métodos granulares (Modo Local)
// En JSON, al no ser BD real, delegamos en la lista en memoria y guardamos todo.

bool JsonReservaRepository::crear(const Reserva& r) {
    // Nota: En modo local, la reserva ya se agregó a la lista "en memoria" antes de llamar a esto
    // (debido a como funciona la app legacy).
    // Pero para ser consistentes con la nueva arquitectura, deberíamos guardar.
    // Como no tenemos acceso a la "Lista" global aquí sin pasártela, asumiremos
    // que el controlador llama a guardar() después. 
    // PERO: Para cumplir la interfaz, si nos pasan una reserva suelta, en modo archivo
    // es ineficiente insertar una sola línea en medio del JSON.
    // Por simplicidad en modo local: Retornamos true y dejamos que la app llame a guardar() masivo.
    return true; 
}

bool JsonReservaRepository::eliminar(int id) {
    return true; 
}

int JsonReservaRepository::contarAsientos(const string& cedula) {
    // En modo local (sin BD), no podemos consultar "la nube".
    // Dependemos de lo que esté cargado en memoria en la ListaReserva.
    // Retornamos -1 para indicar que "use la lógica de memoria local".
    return -1;
}
