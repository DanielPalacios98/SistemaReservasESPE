#pragma once
#include <wx/wx.h>
#include "../lib/include/ListaReserva.h"
#include "../lib/include/Config.h"
#include "../lib/include/IReservaRepository.h"
#include "../lib/include/MongoReservaRepository.h"
#include "../lib/include/JsonReservaRepository.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit();
    int OnExit();
    
    // Global accessors/managers
    IReservaRepository* getRepository() { return repo; }
    ListaReserva& getLista() { return lista; }

private:
    IReservaRepository* repo = nullptr;
    ListaReserva lista;
};

wxDECLARE_APP(MyApp);
