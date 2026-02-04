#include "App.h"
#include "MainFrame.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "../lib/include/Log.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    // Configuración de repositorio (copiado de Controller logic)
    Config cfg = Config::load("config.json");
    // Fallback search logic for config handled in Config::load fix previously

    // Log de runtime
    std::ofstream log("runtime.log", std::ios::out);
    log << "Backend: " << cfg.backend << "\n";
    log << "URI len: " << cfg.mongoUri.size() << "\n";
    log << "DB: " << cfg.mongoDb << " Collection: " << cfg.mongoCollection << "\n";
    Log::info(std::string("App init backend=") + cfg.backend);

    if (cfg.backend == "mongo" && !cfg.mongoUri.empty()) {
        repo = new MongoReservaRepository(cfg.mongoUri, cfg.mongoDb, cfg.mongoCollection);
        
        // Redirigir cerr para capturar el error de conexion
        std::stringstream errStream;
        std::streambuf* oldCerr = std::cerr.rdbuf(errStream.rdbuf());

        // Intentar carga
        bool cargado = repo->cargar(lista);
        
        // Restaurar cerr
        std::cerr.rdbuf(oldCerr);

        if (!cargado) {
            wxString err = errStream.str();
            log << "Mongo cargar: FAIL\n";
            log << "Error: " << err.ToStdString() << "\n";
            Log::error(std::string("Mongo cargar FAIL: ") + err.ToStdString());
            wxMessageBox("Fallo conexion a MongoDB.\n\n" + err + "\n\nEl sistema cargara los datos LOCALES (reservas.json).", "Modo Offline Activo", wxICON_WARNING);
            
            delete repo;
            repo = new JsonReservaRepository();
            repo->cargar(lista);
            
            // Marcar titulo de ventana como offline (esto requiere acceso al frame, lo haremos en MainFrame)
        } else {
            log << "Mongo cargar: OK\n";
            Log::info("Mongo cargar OK");
        }
    } else {
        repo = new JsonReservaRepository();
        repo->cargar(lista);
        log << "Backend JSON local activo\n";
        Log::info("Backend JSON local activo");
    }

    MainFrame* frame = new MainFrame("Sistema de Reservas ESPE (GUI)", wxPoint(50, 50), wxSize(1000, 700));
    
    // Si estamos en modo offline pese a pedir mongo
    if (cfg.backend == "mongo" && dynamic_cast<MongoReservaRepository*>(repo) == nullptr) {
        frame->SetTitle("Sistema de Reservas ESPE (MODO OFFLINE - Datos Locales)");
    }
    
    // Conteo de elementos cargados
    int count = 0;
    if (lista.obtenerHead()) {
        NodoReserva* t = lista.obtenerHead();
        NodoReserva* p = t;
        do { ++count; p = p->next; } while (p != t);
    }
    log << "Reservas cargadas: " << count << "\n";
    Log::info(std::string("Reservas cargadas=") + std::to_string(count));
    log.close();
    
    frame->Show(true);
    return true;
}

int MyApp::OnExit() {
    if (repo) {
        // Guardar al salir
        repo->guardar(lista);
        delete repo;
    }
    return 0;
}
