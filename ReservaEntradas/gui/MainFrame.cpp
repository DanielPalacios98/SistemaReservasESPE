#include "MainFrame.h"
#include "App.h"
#include "ReservaDialog.h"
#include "../lib/include/Reserva.h"
#include "../lib/include/Log.h"
#include <wx/busyinfo.h>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_Hello,   MainFrame::OnHello)
    EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_BUTTON(ID_Refresh, MainFrame::OnRefresh)
    EVT_BUTTON(ID_Add, MainFrame::OnAdd)
    EVT_BUTTON(ID_Delete, MainFrame::OnDelete)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    // Menú
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hola...\tCtrl-H", "Saludo de prueba");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&Archivo");
    menuBar->Append(menuHelp, "&Ayuda");
    SetMenuBar(menuBar);

    // Layout principal
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Toolbar de botones
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btnAdd = new wxButton(this, ID_Add, "Nueva Reserva");
    wxButton* btnDel = new wxButton(this, ID_Delete, "Eliminar Seleccion");
    wxButton* btnRef = new wxButton(this, ID_Refresh, "Recargar");
    
    btnSizer->Add(btnAdd, 0, wxALL, 5);
    btnSizer->Add(btnDel, 0, wxALL, 5);
    btnSizer->Add(btnRef, 0, wxALL, 5);
    
    mainSizer->Add(btnSizer, 0, wxEXPAND | wxALL, 5);

    // Lista
    listView = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    listView->AppendColumn("ID", wxLIST_FORMAT_LEFT, 60);
    listView->AppendColumn("Nombres", wxLIST_FORMAT_LEFT, 200);
    listView->AppendColumn("Cedula", wxLIST_FORMAT_LEFT, 100);
    listView->AppendColumn("Telefono", wxLIST_FORMAT_LEFT, 100);
    listView->AppendColumn("Localidad", wxLIST_FORMAT_LEFT, 80);
    listView->AppendColumn("Asientos", wxLIST_FORMAT_RIGHT, 60);
    
    mainSizer->Add(listView, 1, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);
    CreateStatusBar();
    SetStatusText("Bienvenido al Sistema de Reservas ESPE");

    RefreshList();

    // Auto-sincronización removida: sincroniza solo al presionar Recargar
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Sistema de Reservas con wxWidgets y MongoDB\n\nDesarrollado para ESPE",
                 "Acerca de", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hola mundo desde wxWidgets!");
}

void MainFrame::OnRefresh(wxCommandEvent& event) {
    // Recargar desde el repositorio (BD o local) y refrescar vista
    MyApp* app = (MyApp*)wxApp::GetInstance();
    IReservaRepository* repo = app->getRepository();
    SetStatusText("Sincronizando con repositorio...");
    wxBusyCursor busy;
    repo->cargar(app->getLista());
    RefreshList();
    SetStatusText("Lista actualizada desde repositorio");
}

void MainFrame::OnAdd(wxCommandEvent& event) {
    ReservaDialog dlg(this);
    if (dlg.ShowModal() == wxID_OK) {
        Log::info("Dialog OK: creando reserva");
        // Recuperar datos validados
        std::string nombres = dlg.getNombres().ToStdString();
        std::string cedula = dlg.getCedula().ToStdString();
        std::string telefono = dlg.getTelefono().ToStdString();
        std::string correo = dlg.getCorreo().ToStdString();
        std::string localidad = dlg.getLocalidad().ToStdString();
        int asientos = dlg.getAsientos();

        // Lógica de negocio: insertar en lista y guardar
        MyApp* app = (MyApp*)wxApp::GetInstance();
        IReservaRepository* repo = app->getRepository();

        // Verificamos si el repositorio soporta operaciones atómicas (Nube)
        // Usamos contarAsientos como proxy: si devuelve -1 es local, si >= 0 es cloud.
        bool modoNube = (repo->contarAsientos(cedula) != -1);

        if (modoNube) {
            SetStatusText("Guardando en la nube...");
            wxBusyCursor busy;
            // 1. Generar ID único atómico
            int newId = repo->generarId();
            if (newId <= 0) {
                 wxMessageBox("Error: No se pudo generar un ID único en la base de datos.", "Error Crítico");
                 return;
            }

            // 2. Crear objeto temporal
            Reserva r(newId, nombres, cedula, telefono, correo, localidad, asientos);
            
            // 3. Insertar directo en la nube
            if (repo->crear(r)) {
                Log::info(std::string("Reserva creada id=") + std::to_string(newId));
                wxMessageBox(wxString::Format("Reserva #%d guardada exitosamente en la nube.", newId), "Éxito");
                
                // 4. Refrescar lista completa (descarga lo nuevo de todos los usuarios)
                repo->cargar(app->getLista());
                RefreshList();
                SetStatusText("Sincronizado.");
            } else {
                Log::error("Fallo al crear reserva en la nube");
                wxMessageBox("Error al guardar en la base de datos remota.", "Error");
            }
        } 
        else {
            // MODO LOCAL / LEGACY
            Reserva* nueva = app->getLista().agregarReserva(nombres, cedula, telefono, correo, localidad, asientos);
            if (nueva) {
                Log::info("Reserva local creada");
                // Guardar en repositorio (sobrescribe archivo JSON)
                if (repo->guardar(app->getLista())) {
                    wxMessageBox("Reserva guardada localmente.", "Info");
                } else {
                    Log::error("Fallo al guardar reserva local");
                    wxMessageBox("Reserva creada en memoria pero fallo el guardado en disco.", "Alerta");
                }
                RefreshList();
            } else {
                Log::error("No se pudo crear reserva (validacion negocio)");
                wxMessageBox("No se pudo crear la reserva (Cupo lleno o límite local).", "Error");
            }
        }
    }
}

// AutoRefresh eliminado: sincronización manual únicamente

void MainFrame::OnDelete(wxCommandEvent& event) {
    long item = listView->GetFirstSelected();
    if (item == -1) {
        wxMessageBox("Seleccione una reserva para eliminar.", "Aviso");
        return;
    }
    
    wxString textId = listView->GetItemText(item, 0); // Columna 0 es ID
    long idVal;
    if (textId.ToLong(&idVal)) {
        MyApp* app = (MyApp*)wxApp::GetInstance();
        if (app->getLista().eliminarPorID((int)idVal)) {
            // Guardar cambios en repo
            app->getRepository()->guardar(app->getLista());
            RefreshList();
            wxMessageBox("Reserva eliminada.", "Exito");
        } else {
            wxMessageBox("Error al eliminar.", "Error");
        }
    }
}

void MainFrame::RefreshList() {
    listView->DeleteAllItems();
    MyApp* app = (MyApp*)wxApp::GetInstance();
    ListaReserva& lista = app->getLista();
    
    NodoReserva* head = lista.obtenerHead();
    if (!head) return;

    NodoReserva* temp = head;
    do {
        Reserva* r = temp->reserva;
        // Insertar item
        long index = listView->GetItemCount();
        listView->InsertItem(index, wxString::Format("%d", r->getIdReserva()));
        listView->SetItem(index, 1, wxString::FromUTF8(r->getNombres().c_str()));
        listView->SetItem(index, 2, r->getCedula());
        listView->SetItem(index, 3, r->getTelefono());
        listView->SetItem(index, 4, r->getLocalidad());
        listView->SetItem(index, 5, wxString::Format("%d", r->getNumAsientos()));
        
        temp = temp->next;
    } while (temp != head);
}
