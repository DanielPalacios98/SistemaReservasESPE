#include "include/MongoReservaRepository.h"
#include "include/ListaReserva.h"
#include "include/Reserva.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

#ifdef USE_MONGO
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/options/update.hpp>
#include <mongocxx/exception/exception.hpp>
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
    std::cout << "Aviso: Backend Mongo no habilitado en esta build. Usar JSON como fallback." << std::endl;
    return false; // Forzamos fallback en Controller
#else
    try {
        std::cout << "Mongo: conectando con MongoDB..." << std::endl;
        mongocxx::client client{mongocxx::uri{uri}};
        // Ping para confirmar la conexión
        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;
        auto dbh = client.database(db);
        auto pingResult = dbh.run_command(document{} << "ping" << 1 << finalize);
        (void)pingResult; // No usamos el resultado, sólo confirmación
        std::cout << "Mongo: conexion exitosa al cluster y base '" << db << "'." << std::endl;

        auto coll = dbh.collection(collection);

        // Exportamos a JSON local con el mismo formato esperado por ListaReserva::cargarDesdeJson
        // Archivo temporal único por proceso
        std::string tmpFile = std::string("reservas_sync_") + std::to_string(std::time(nullptr)) + "_" + std::to_string(std::rand()) + ".json";
        ofstream out(tmpFile);
        if (!out.is_open()) {
            cerr << "Mongo: no se pudo abrir archivo temporal para escritura: " << tmpFile << endl;
            return false;
        }

        out << "{\n  \"reservas\": " << static_cast<char>(91) << "\n";
        bool first = true;
        size_t exportadas = 0;
        for (auto&& doc : coll.find({})) {
            auto v = doc; // en v4, el cursor proporciona un view; si fuese value, puede usarse doc.view()

            // Extracción segura con defaults
            int id = 0;
            int numAs = 1;
            std::string nombres, cedula, telefono, correo, localidad;

            auto itId = v.find("idReserva");
            if (itId != v.end() && itId->type() == bsoncxx::type::k_int32) id = itId->get_int32().value;
            auto itNA = v.find("numAsientos");
            if (itNA != v.end() && itNA->type() == bsoncxx::type::k_int32) numAs = itNA->get_int32().value;

            auto itNom = v.find("nombres");
            if (itNom != v.end() && itNom->type() == bsoncxx::type::k_string) nombres = std::string(itNom->get_string().value);
            auto itCed = v.find("cedula");
            if (itCed != v.end() && itCed->type() == bsoncxx::type::k_string) cedula = std::string(itCed->get_string().value);
            auto itTel = v.find("telefono");
            if (itTel != v.end() && itTel->type() == bsoncxx::type::k_string) telefono = std::string(itTel->get_string().value);
            auto itCor = v.find("correo");
            if (itCor != v.end() && itCor->type() == bsoncxx::type::k_string) correo = std::string(itCor->get_string().value);
            auto itLoc = v.find("localidad");
            if (itLoc != v.end() && itLoc->type() == bsoncxx::type::k_string) localidad = std::string(itLoc->get_string().value);

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
        out << "\n  " << static_cast<char>(93) << "\n}";
        out.close();
        std::cout << "Mongo: exportadas " << exportadas << " reservas desde " << db << "/" << collection << std::endl;

        // Cargar en memoria desde el JSON recién exportado
        lista.clear();
        lista.cargarDesdeJson(tmpFile);
        // Intentamos borrar el temporal (ignorar errores)
        try { std::remove(tmpFile.c_str()); } catch(...) {}
        return true; // Retornamos true indicando que la conexión y sync fueron exitosas (aunque esté vacía)
    } catch (const std::exception& e) {
        std::cerr << "Mongo: error al cargar o conectar: " << e.what() << std::endl;
        return false;
    }
#endif
}

bool MongoReservaRepository::guardar(const ListaReserva& lista) {
#ifndef USE_MONGO
    std::cout << "Aviso: Backend Mongo no habilitado en esta build. Usar JSON como fallback." << std::endl;
    return false; // Forzamos fallback en Controller
#else
    try {
        mongocxx::client client{mongocxx::uri{uri}};
        auto coll = client.database(db).collection(collection);

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
        std::cout << "Mongo: insertadas " << insertadas << " reservas en '" << db << "'.'" << collection << "'" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Mongo: error al guardar: " << e.what() << std::endl;
        // Log to file for debug
        ofstream log("mongo_error_guardar.txt");
        log << e.what() << endl;
        log.close();
        return false;
    }
#endif
}

// --- Implementación de nuevos métodos granulares ---

int MongoReservaRepository::generarId() {
#ifdef USE_MONGO
    try {
        mongocxx::client client{mongocxx::uri{uri}};
        auto coll = client.database(db).collection("contadores");
        
        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;
        using bsoncxx::builder::stream::open_document;
        using bsoncxx::builder::stream::close_document;

        // findOneAndUpdate para incrementar atomicamente
        auto result = coll.find_one_and_update(
            document{} << "_id" << "reservaId" << finalize,
            document{} << "$inc" << open_document << "seq" << 1 << close_document << finalize,
            mongocxx::options::find_one_and_update{}.upsert(true).return_document(mongocxx::options::return_document::k_after)
        );

        if (result) {
            auto view = result->view();
            auto itSeq = view.find("seq");
            if (itSeq != view.end() && itSeq->type() == bsoncxx::type::k_int32) return itSeq->get_int32().value;
        }
    } catch (const std::exception& e) {
        std::cerr << "Mongo: error generarId: " << e.what() << std::endl;
    }
#endif
    return -1; // Fallo
}

bool MongoReservaRepository::crear(const Reserva& r) {
#ifdef USE_MONGO
    try {
        mongocxx::client client{mongocxx::uri{uri}};
        auto coll = client.database(db).collection(collection);
        auto totalsColl = client.database(db).collection("cedula_totales");

        if (r.getNumAsientos() > 5) return false;

        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;
        using bsoncxx::builder::stream::open_document;
        using bsoncxx::builder::stream::close_document;

        // CONFIAR EN EL LOCK: La cédula ya fue bloqueada en validarYBloquearCedula()
        // Solo actualizar total y insertar la reserva (sin revalidación de cupo)
        
        // 1) Incrementar contador de asientos por cédula
        auto updateResult = totalsColl.update_one(
            document{} << "_id" << r.getCedula() << finalize,
            document{} << "$inc" << open_document << "total" << r.getNumAsientos() << close_document << finalize
        );

        // Si la cédula no existe en cedula_totales (nunca se bloqueó), algo está mal
        // Insertar nuevo documento de totales como fallback
        if (updateResult && updateResult->matched_count() == 0) {
            try {
                document docTot;
                docTot << "_id" << r.getCedula()
                       << "cedula" << r.getCedula()
                       << "total" << r.getNumAsientos();
                totalsColl.insert_one(docTot << finalize);
            } catch (const mongocxx::exception& e) {
                // Si hay error de clave duplicada, alguien más lo insertó; ignorar
                if (e.code().value() != 11000) throw;
            }
        }

        // 2) Insertar la reserva (cédula ya está bloqueada y cupo asignado)
        document doc;
        doc << "idReserva" << r.getIdReserva()
            << "nombres" << r.getNombres()
            << "cedula" << r.getCedula()
            << "telefono" << r.getTelefono()
            << "correo" << r.getCorreo()
            << "localidad" << r.getLocalidad()
            << "numAsientos" << r.getNumAsientos();

        coll.insert_one(doc << finalize);
        std::cerr << "Mongo: reserva creada (cedula " << r.getCedula() << " desbloqueada por MainFrame)" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Mongo: error al crear reserva: " << e.what() << std::endl;
        return false;
    }
#else
    return false;
#endif
}

bool MongoReservaRepository::eliminar(int id) {
#ifdef USE_MONGO
    try {
        mongocxx::client client{mongocxx::uri{uri}};
        auto coll = client.database(db).collection(collection);
        
        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;

        auto result = coll.delete_one(document{} << "idReserva" << id << finalize);
        if (result && result->deleted_count() > 0) return true;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Mongo: error al eliminar: " << e.what() << std::endl;
        return false;
    }
#else
    return false;
#endif
}

int MongoReservaRepository::contarAsientos(const string& cedula) {
#ifdef USE_MONGO
    try {
        mongocxx::client client{mongocxx::uri{uri}};
        auto coll = client.database(db).collection(collection);

        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;
        
        // Pipeline de agregacion para sumar asientos de esa cedula
        mongocxx::pipeline pipe;
        pipe.match(document{} << "cedula" << cedula << finalize);
        pipe.group(document{} 
            << "_id" << bsoncxx::types::b_null{}
            << "total" << bsoncxx::builder::stream::open_document << "$sum" << "$numAsientos" << bsoncxx::builder::stream::close_document << finalize);

        auto cursor = coll.aggregate(pipe);
        for (auto&& doc : cursor) {
            auto itTotal = doc.find("total");
            if (itTotal != doc.end() && itTotal->type() == bsoncxx::type::k_int32) return itTotal->get_int32().value;
            if (itTotal != doc.end() && itTotal->type() == bsoncxx::type::k_int64) return static_cast<int>(itTotal->get_int64().value);
        }
        return 0; // Si no hay docs, es 0
    } catch (const std::exception& e) {
        std::cerr << "Mongo: error contarAsientos: " << e.what() << std::endl;
        return 0; // Fallback seguro
    }
#else
    return 0;
#endif
}
bool MongoReservaRepository::validarYBloquearCedula(const string& cedula) {
#ifdef USE_MONGO
    try {
        mongocxx::client client{mongocxx::uri{uri}};
        auto locksDb = client.database(db);
        auto locksColl = locksDb.collection("cedula_locks");

        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;

        // Crear índice TTL si no existe (30 segundos de expiración)
        try {
            mongocxx::options::index indexOptions;
            indexOptions.expire_after(std::chrono::seconds(30));
            locksColl.create_index(
                document{} << "lock_expires" << 1 << finalize,
                indexOptions
            );
        } catch (...) {
            // El índice ya existe, ignorar
        }

        // 1) Contar asientos actuales desde reservas
        auto reservasColl = locksDb.collection(collection);
        mongocxx::pipeline pipe;
        pipe.match(document{} << "cedula" << cedula << finalize);
        pipe.group(document{}
            << "_id" << bsoncxx::types::b_null{}
            << "total" << bsoncxx::builder::stream::open_document
                << "$sum" << "$numAsientos"
            << bsoncxx::builder::stream::close_document
            << finalize);

        int actuales = 0;
        auto cursor = reservasColl.aggregate(pipe);
        for (auto&& doc : cursor) {
            auto itTotal = doc.find("total");
            if (itTotal != doc.end() && itTotal->type() == bsoncxx::type::k_int32)
                actuales = itTotal->get_int32().value;
            else if (itTotal != doc.end() && itTotal->type() == bsoncxx::type::k_int64)
                actuales = static_cast<int>(itTotal->get_int64().value);
        }

        // 2) Si ya tiene 5 asientos, rechazar
        if (actuales >= 5) {
            std::cerr << "Mongo: cedula " << cedula << " ya tiene " << actuales << " asientos (maximo 5)" << std::endl;
            return false;
        }

        // 3) Intentar insertar lock de cédula (fail si ya existe)
        std::time_t now = std::time(nullptr);
        document lockDoc;
        lockDoc << "_id" << cedula
                << "cedula" << cedula
                << "locked_at" << static_cast<long long>(now)
                << "lock_expires" << bsoncxx::types::b_date(std::chrono::system_clock::now() + std::chrono::seconds(30));

        try {
            locksColl.insert_one(lockDoc << finalize);
            std::cerr << "Mongo: cedula " << cedula << " bloqueada (lock creado)" << std::endl;
            return true;
        } catch (const mongocxx::exception& e) {
            // Duplicate key error: la cédula ya está bloqueada
            if (e.code().value() == 11000) {
                std::cerr << "Mongo: cedula " << cedula << " ya está bloqueada por otro usuario" << std::endl;
                return false;
            }
            throw;
        }
    } catch (const std::exception& e) {
        std::cerr << "Mongo: error validarYBloquearCedula: " << e.what() << std::endl;
        return false;
    }
#else
    return true; // Fallback en modo local
#endif
}

bool MongoReservaRepository::desbloquearCedula(const string& cedula) {
#ifdef USE_MONGO
    try {
        mongocxx::client client{mongocxx::uri{uri}};
        auto locksColl = client.database(db).collection("cedula_locks");

        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;

        auto result = locksColl.delete_one(document{} << "_id" << cedula << finalize);
        if (result && result->deleted_count() > 0) {
            std::cerr << "Mongo: cedula " << cedula << " desbloqueada" << std::endl;
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Mongo: error desbloquearCedula: " << e.what() << std::endl;
        return false;
    }
#else
    return true;
#endif
}