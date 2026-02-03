#pragma once
#include "IReservaRepository.h"
#include <string>
using namespace std;

class JsonReservaRepository : public IReservaRepository {
private:
    string jsonFile;
    string jsonlFile;
    string txtFile;
public:
    JsonReservaRepository(const string& json = "reservas.json",
                          const string& jsonl = "reservas.jsonl",
                          const string& txt = "reservas.txt")
        : jsonFile(json), jsonlFile(jsonl), txtFile(txt) {}

    bool cargar(ListaReserva& lista) override;
    bool guardar(const ListaReserva& lista) override;
};
