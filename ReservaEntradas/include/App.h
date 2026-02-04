#pragma once
#include <wx/wx.h>
#include "ListaReserva.h"
#include "BSTReservas.h"
#include "HashTableUsuarios.h"
#include "Config.h"
#include "IReservaRepository.h"
#include "MongoReservaRepository.h"
#include "JsonReservaRepository.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit();
    int OnExit();
    
    // Global accessors/managers
    IReservaRepository* getRepository() { return repo; }
    ListaReserva& getLista() { return lista; }
    HashTableUsuarios* getUsuarios() { return usuarios; }
    BSTReservas& getBST() { return bst; }

private:
    IReservaRepository* repo = nullptr;
    ListaReserva lista;
    HashTableUsuarios* usuarios = nullptr;
    BSTReservas bst;
};

wxDECLARE_APP(MyApp);
