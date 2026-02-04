#pragma once
#include "IReservaRepository.h"
#include <string>
using namespace std;

class MongoReservaRepository : public IReservaRepository {
private:
    string uri;
    string db;
    string collection;
public:
    MongoReservaRepository(const string& mongoUri,
                           const string& mongoDb,
                           const string& mongoCollection)
        : uri(mongoUri), db(mongoDb), collection(mongoCollection) {}

    bool cargar(ListaReserva& lista) override;
    bool guardar(const ListaReserva& lista) override;

    // Implementación atómica
    int generarId() override;
    bool crear(const Reserva& r) override;
    bool eliminar(int id) override;
    int contarAsientos(const string& cedula) override;
    
    // Bloqueo de cédula con TTL
    bool validarYBloquearCedula(const string& cedula) override;
    bool desbloquearCedula(const string& cedula) override;
};
