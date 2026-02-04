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

    // Métodos dummy o simulados para JSON (que sigue siendo monousuario/local)
    int generarId() override { return 0; } // 0 indica que use lógica local del contador
    bool crear(const Reserva& r) override;
    bool eliminar(int id) override;
    int contarAsientos(const string& cedula) override;
};
