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
};
