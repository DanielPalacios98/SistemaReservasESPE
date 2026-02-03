#include "include/MongoReservaRepository.h"
#include "include/ListaReserva.h"
#include "include/Reserva.h"
#include <iostream>
#include <fstream>

using namespace std;

#ifdef USE_MONGO
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/types.hpp>

// Instancia global única de mongocxx
static mongocxx::instance mongo_instance{};

// Escape JSON mínimo, consistente con ListaReserva
static string jsonEscapeLocal(const string& s) {
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
#endif

bool MongoReservaRepository::cargar(ListaReserva& lista) {
#ifndef USE_MONGO
    cout << "[Aviso] Backend Mongo no habilitado en esta build. Usar JSON como fallback." << endl;
    return false; // Forzamos fallback en Controller
#else
    try {
        cout << "Conectando con MongoDB..." << endl;
        mongocxx::client client{mongocxx::uri{uri}};
        // Ping para confirmar la conexión
        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;
        auto pingResult = client[db].run_command(document{} << "ping" << 1 << finalize);
        (void)pingResult; // No usamos el resultado, sólo confirmación
        cout << "[Mongo] Conexión exitosa al cluster y base '" << db << "'." << endl;

        auto coll = client[db][collection];

        // Exportamos a JSON local con el mismo formato esperado por ListaReserva::cargarDesdeJson
        ofstream out("reservas.json");
        if (!out.is_open()) {
            cerr << "[Mongo] No se pudo abrir reservas.json para escritura." << endl;
            return false;
        }

        out << "{\n  \"reservas\": [\n";
        bool first = true;
        size_t exportadas = 0;
        for (auto&& doc : coll.find({})) {
            auto v = doc; // en v4, el cursor proporciona un view; si fuese value, puede usarse doc.view()

            // Extracción segura con defaults
            int id = 0;
            int numAs = 1;
            std::string nombres, cedula, telefono, correo, localidad;

            auto elId = v["idReserva"]; if (elId && elId.type() == bsoncxx::type::k_int32) id = elId.get_int32().value;
            auto elNA = v["numAsientos"]; if (elNA && elNA.type() == bsoncxx::type::k_int32) numAs = elNA.get_int32().value;

            auto elNom = v["nombres"];   if (elNom && elNom.type() == bsoncxx::type::k_string) nombres   = std::string(elNom.get_string().value);
            auto elCed = v["cedula"];    if (elCed && elCed.type() == bsoncxx::type::k_string) cedula    = std::string(elCed.get_string().value);
            auto elTel = v["telefono"];  if (elTel && elTel.type() == bsoncxx::type::k_string) telefono  = std::string(elTel.get_string().value);
            auto elCor = v["correo"];    if (elCor && elCor.type() == bsoncxx::type::k_string) correo    = std::string(elCor.get_string().value);
            auto elLoc = v["localidad"]; if (elLoc && elLoc.type() == bsoncxx::type::k_string) localidad = std::string(elLoc.get_string().value);

            if (!first) out << ",\n";
            first = false;
            out << "    {\n";
            out << "      \"idReserva\": " << id << ",\n";
            out << "      \"nombres\": \"" << jsonEscapeLocal(nombres) << "\",\n";
            out << "      \"cedula\": \"" << jsonEscapeLocal(cedula) << "\",\n";
            out << "      \"telefono\": \"" << jsonEscapeLocal(telefono) << "\",\n";
            out << "      \"correo\": \"" << jsonEscapeLocal(correo) << "\",\n";
            out << "      \"localidad\": \"" << jsonEscapeLocal(localidad) << "\",\n";
            out << "      \"numAsientos\": " << numAs << "\n";
            out << "    }";
            ++exportadas;
        }
        out << "\n  ]\n}";
        out.close();
        cout << "[Mongo] Exportadas " << exportadas << " reservas desde '" << db << "'.'" << collection << "'" << endl;

        // Cargar en memoria desde el JSON recién exportado
        lista.clear();
        lista.cargarDesdeJson("reservas.json");
        return true; // Retornamos true indicando que la conexión y sync fueron exitosas (aunque esté vacía)
    } catch (const std::exception& e) {
        cerr << "[Mongo] Error al cargar / conectar: " << e.what() << endl;
        return false;
    }
#endif
}

bool MongoReservaRepository::guardar(const ListaReserva& lista) {
#ifndef USE_MONGO
    cout << "[Aviso] Backend Mongo no habilitado en esta build. Usar JSON como fallback." << endl;
    return false; // Forzamos fallback en Controller
#else
    try {
        mongocxx::client client{mongocxx::uri{uri}};
        auto coll = client[db][collection];

        // Estrategia simple: reemplazar coleccion completa
        coll.delete_many({});

        // Recorrer la lista y subir cada documento
        NodoReserva* head = lista.obtenerHead();
        if (!head) return true; // Nada que guardar

        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;

        NodoReserva* tmp = head;
        size_t insertadas = 0;
        do {
            const Reserva* r = tmp->reserva;
            document doc;
            doc << "idReserva" << r->getIdReserva()
                << "nombres" << r->getNombres()
                << "cedula" << r->getCedula()
                << "telefono" << r->getTelefono()
                << "correo" << r->getCorreo()
                << "localidad" << r->getLocalidad()
                << "numAsientos" << r->getNumAsientos();
            coll.insert_one(doc << finalize);
            ++insertadas;
            tmp = tmp->next;
        } while (tmp != head);
        cout << "[Mongo] Insertadas " << insertadas << " reservas en '" << db << "'.'" << collection << "'" << endl;
        return true;
    } catch (const std::exception& e) {
        cerr << "[Mongo] Error al guardar: " << e.what() << endl;
        return false;
    }
#endif
}
