#pragma once
#include "ListaReserva.h"
#include "Reserva.h"
#include <string>
using namespace std;

class IReservaRepository {
public:
    virtual ~IReservaRepository() {}
    
    // Carga masiva (para llenar la lista al inicio/refrescar)
    virtual bool cargar(ListaReserva& lista) = 0;
    
    // Guardado masivo (deprecated para multi-usuario real, pero mantenido para JSON)
    virtual bool guardar(const ListaReserva& lista) = 0;

    // Métodos CRUD granulares (Multi-usuario)
    // Retorna el ID generado o -1 si falla. 0 si el backend no maneja IDs (JSON usa autoinc local)
    virtual int generarId() { return 0; } 
    
    virtual bool crear(const Reserva& r) = 0;
    virtual bool eliminar(int id) = 0;
    virtual int contarAsientos(const string& cedula) = 0;
    
    // Métodos de bloqueo transaccional para evitar race conditions
    // Intenta bloquear una cédula por 30 segundos. Retorna true si el bloqueo fue exitoso.
    // Si la cédula ya está bloqueada, retorna false.
    virtual bool validarYBloquearCedula(const string& cedula) { return true; }
    
    // Desbloquea una cédula después de confirmar o cancelar la reserva
    virtual bool desbloquearCedula(const string& cedula) { return true; }
};
