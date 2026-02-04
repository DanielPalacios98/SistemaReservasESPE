#include "MainFrame.h"
#include "App.h"
#include "ReservaDialog.h"
#include "Reserva.h"
#include "Log.h"
#include <wx/busyinfo.h>
// Eliminado <algorithm>, usamos sort manual en MainFrameExtensions.cpp

// Forward declaration de funcion manual
void bubbleSortManual(Reserva** arr, int n, bool (*debeIntercambiar)(Reserva*, Reserva*));

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_Hello,   MainFrame::OnHello)
    EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_BUTTON(ID_Refresh, MainFrame::OnRefresh)
    EVT_BUTTON(ID_Add, MainFrame::OnAdd)
    EVT_BUTTON(ID_Delete, MainFrame::OnDelete)
    EVT_TIMER(ID_Timer, MainFrame::OnTimer)
    EVT_BUTTON(ID_SortName, MainFrame::OnSortName)
    EVT_BUTTON(ID_SortCedula, MainFrame::OnSortCedula)
    EVT_BUTTON(ID_SortId, MainFrame::OnSortId)
    EVT_BUTTON(ID_ShellSort, MainFrame::OnShellSort)
    EVT_BUTTON(ID_SearchBST, MainFrame::OnSearchBST)
    EVT_BUTTON(ID_SearchLin, MainFrame::OnSearchLinear)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    // Menú
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hola...\tCtrl-H", "Saludo de prueba");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, "Salir");

    wxMenu *menuTools = new wxMenu;
    menuTools->Append(ID_SortId, "Ordenar por &ID (Default)");
    menuTools->Append(ID_SortName, "Ordenar por &Nombre");
    menuTools->Append(ID_SortCedula, "Ordenar por &Cedula");
    menuTools->AppendSeparator();
    menuTools->Append(ID_ShellSort, "Shell Sort (&Nombres)");
    menuTools->Append(ID_SearchBST, "&Buscar por ID (BST)");
    menuTools->Append(ID_SearchLin, "&Buscar por ID (Lineal/Exhaustivo)");

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&Archivo");
    menuBar->Append(menuTools, "&Herramientas");
    menuBar->Append(menuHelp, "&Ayuda");
    SetMenuBar(menuBar);

    // Layout principal
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Toolbar de botones
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btnAdd = new wxButton(this, ID_Add, "Nueva Data");
    wxButton* btnDel = new wxButton(this, ID_Delete, "Borrar");
    wxButton* btnRef = new wxButton(this, ID_Refresh, "Refrescar");
    
    // Botones adicionales solicitados
    wxButton* btnSearch = new wxButton(this, ID_SearchBST, "Buscar (BST)");
    wxButton* btnSearchLin = new wxButton(this, ID_SearchLin, "Buscar (Lin)");
    wxButton* btnSortId = new wxButton(this, ID_SortId, "Ord. ID");
    wxButton* btnSortN = new wxButton(this, ID_SortName, "Ord. Nombre");
    wxButton* btnSortC = new wxButton(this, ID_SortCedula, "Ord. Cedula");
    wxButton* btnShell = new wxButton(this, ID_ShellSort, "ShellSort");

    btnSizer->Add(btnAdd, 0, wxALL, 2);
    btnSizer->Add(btnDel, 0, wxALL, 2);
    btnSizer->Add(btnRef, 0, wxALL, 2);
    btnSizer->AddSpacer(5);
    btnSizer->Add(btnSearch, 0, wxALL, 2);
    btnSizer->Add(btnSearchLin, 0, wxALL, 2);
    btnSizer->Add(btnSortId, 0, wxALL, 2);
    btnSizer->Add(btnSortN, 0, wxALL, 2);
    btnSizer->Add(btnSortC, 0, wxALL, 2);
    btnSizer->Add(btnShell, 0, wxALL, 2);
    
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

    // [OPTIMIZACION]: Desactivado Timer automatico por causar bloqueos de UI (Event Loop Lag)
    // El usuario debe usar el boton "Refrescar" manualmente.
    // refreshTimer = new wxTimer(this, ID_Timer);
    // refreshTimer->Start(15000); 
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
    LoadData(false);
    
    // [FIX CONCURRENCIA ROBUSTO]
    // Loop de auditoria: repetimos el scanner hasta que no encontremos ningun exceso.
    // Esto asegura que si Maria tiene 3 reservas extra, se borren las 3 una por una.
    MyApp* app = (MyApp*)wxApp::GetInstance();
    IReservaRepository* repo = app->getRepository();

    bool huboCorrecciones = false;
    bool seguirBuscando = true;
    int iteraciones = 0;

    while(seguirBuscando && iteraciones < 10) { // Limitamos iteraciones por seguridad
        seguirBuscando = false;
        iteraciones++;

        int n = 0;
        Reserva** arr = app->getLista().getReservasArray(n);
        if (!arr) break;

        // Estructura para sumarizar por usuario
        // Como N es bajo, usamos dos punteros sin mapa.
        for (int i = 0; i < n; ++i) {
             string ced = arr[i]->getCedula();
             // Trim basico por si acaso
             size_t first = ced.find_first_not_of(" \t");
             if (first == string::npos) continue; // String vacio o solo espacios
             size_t last = ced.find_last_not_of(" \t");
             ced = ced.substr(first, (last - first + 1));

             int totalAsientos = 0;
             int maxId = -1;

             // Calcular total de ESTA cedula
             for (int j = 0; j < n; ++j) {
                 string c2 = arr[j]->getCedula();
                 // Trim 
                 size_t f2 = c2.find_first_not_of(" \t");
                 if (f2 != string::npos) {
                     size_t l2 = c2.find_last_not_of(" \t");
                     c2 = c2.substr(f2, (l2 - f2 + 1));
                 }
                 
                 if (c2 == ced) {
                     totalAsientos += arr[j]->getNumAsientos();
                     if (arr[j]->getIdReserva() > maxId) {
                         maxId = arr[j]->getIdReserva();
                     }
                 }
             }

             if (totalAsientos > 5 && maxId != -1) {
                 // Encontramos violacion de cupo!
                 Log::error("AUDITORIA: Exceso detectado (" + std::to_string(totalAsientos) + ") para: " + ced + ". Eliminando ID: " + std::to_string(maxId));
                 
                 app->getLista().eliminarPorID(maxId);
                 repo->guardar(app->getLista()); // Persistir inmediatamente
                 
                 huboCorrecciones = true;
                 seguirBuscando = true; // Reiniciar escaneo desde cero con la nueva lista
                 break; // Romper for loop interno
             }
        }
        
        delete[] arr; // Liberar temporal
    }

    // Si corregimos algo, avisamos y RECARGAMOS LA UI FINAL
    if (huboCorrecciones) {
        wxMessageBox("El sistema ha corregido automaticamente inconsistencias de cupo detectadas.", "Auditoria", wxOK | wxICON_INFORMATION);
        LoadData(false); 
    }
}

void MainFrame::OnTimer(wxTimerEvent& event) {
    LoadData(true);
}

void MainFrame::LoadData(bool silent) {
    MyApp* app = (MyApp*)wxApp::GetInstance();
    IReservaRepository* repo = app->getRepository();
    
    if (!silent) {
        SetStatusText("Sincronizando con repositorio...");
        wxBusyCursor busy;
        repo->cargar(app->getLista());
        RefreshList();
        SetStatusText("Lista actualizada desde repositorio");
    } else {
        // En modo silencioso (timer), cargamos sin cursor de espera
        repo->cargar(app->getLista());
        
        // Guardar selección
        long item = listView->GetFirstSelected();
        long selectedId = -1;
        if (item != -1) {
             wxString textId = listView->GetItemText(item, 0); 
             textId.ToLong(&selectedId);
        }

        RefreshList();

        // Restaurar selección
        if (selectedId != -1) {
             long count = listView->GetItemCount();
             for(long i=0; i<count; i++) {
                 long idVal;
                 if (listView->GetItemText(i, 0).ToLong(&idVal) && idVal == selectedId) {
                     listView->SetItemState(i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
                     break;
                 }
             }
        }
    }
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

            // [FIX CONCURRENCIA]: Revalidar cupos "Just-In-Time" antes de escribir.
            // Esto previene que dos terminales lean "3", sumen "2", y ambas guarden llegando a "7".
            int checkActuales = repo->contarAsientos(cedula);
            if (checkActuales + asientos > 5) {
                // Cancelamos operación silenciosamente o con log, para mostrar MessageBox fuera del busy cursor scope si fuera necesario
                // Pero wxMessageBox funciona sobre busy cursor generalmente.
                wxMessageBox(wxString::Format("Error de Sincronizacion:\nEl usuario ya tiene %d boletos registrados.\nNo es posible agregar %d mas (Limite 5).", checkActuales, asientos), "Error de Concurrencia", wxOK | wxICON_ERROR);
                Log::error("Concurrencia detectada y prevenida para cedula " + cedula);
                return;
            }

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
                app->getLista().construirBST(app->getBST()); // Reconstruimos BST con data fresca
                RefreshList();
                SetStatusText("Sincronizado.");
            } else {
                Log::error("Fallo al crear reserva en la nube");
                wxMessageBox("Error al guardar en la base de datos remota.", "Error");
            }
        } 
        else {
            // MODO LOCAL / LEGACY

            // 1. Intentamos agregar la reserva
            // Nota: Ya no bloqueamos antes, dejaremos que ocurra la carrera y la corregimos DESPUES
            //       siguiendo el principio de "Sanitizacion Retroactiva".
            
            // Primero cargamos lo ultimo para tener el ID correcto y datos frescos
            repo->cargar(app->getLista());
            
            Reserva* nueva = app->getLista().agregarReserva(nombres, cedula, telefono, correo, localidad, asientos);
            if (nueva) {
                // Guardar inmediatamente
                if (repo->guardar(app->getLista())) {
                    
                    // [CORRECCION CONCURRENCIA - ESTRATEGIA REACTIVA]
                    // Inmediatamente despues de guardar, re-leemos todo el archivo para ver que paso realmente
                    // en el disco (por si otro proceso escribio al mismo tiempo).
                    repo->cargar(app->getLista());

                    // Analizar las reservas de ESTA cedula
                    int totalReal = app->getLista().contarAsientosPorCedula(cedula);
                    
                    if (totalReal > 5) {
                        // DETECTAMOS OLA DE RESERVAS CONCURRENTES
                        // Debemos eliminar la(s) ultima(s) reserva(s) de este usuario hasta cumplir el cupo.
                        // Como acabamos de cargar, tenemos las reservas de AMBOS procesos.
                        // Asumimos que la nuestra (o la del otro) es la que tiene el ID mas alto si se generaron en orden,
                        // o simplemente borramos la ultima de la lista hasta cumplir.
                        
                        Log::error("CONCURRENCIA DETECTADA: Usuario excedio cupo post-guardado (" + std::to_string(totalReal) + "). Revirtiendo...");

                        while (totalReal > 5) {
                            // Buscar la ultima reserva de este usuario
                            Reserva* ultima = nullptr;
                            int maxId = -1;
                            
                            // Recorrido manual usando el array copy para encontrar el maximo ID
                            int n = 0;
                            Reserva** arr = app->getLista().getReservasArray(n);
                            if (arr) {
                                for(int i=0; i<n; i++) {
                                    Reserva* r = *(arr+i);
                                    if (r->getCedula() == cedula) {
                                         if (r->getIdReserva() > maxId) {
                                             maxId = r->getIdReserva();
                                             ultima = r;
                                         }
                                    }
                                }
                                delete[] arr; // Importante liberar
                            }

                            if (ultima && maxId != -1) {
                                int asientosAborrar = ultima->getNumAsientos();
                                app->getLista().eliminarPorID(maxId);
                                totalReal -= asientosAborrar; // Estimacion rapida para loop
                                
                                // Guardar immediato para evitar bucles infinitos si falla borrado
                                repo->guardar(app->getLista());
                            } else {
                                break; // Seguridad anti loop infinito
                            }
                        }
                        
                        // Guardar la lista FINALMENTE CORREGIDA
                        repo->guardar(app->getLista());
                        
                        // Avisar al usuario
                        wxMessageBox("Advertencia de Concurrencia:\nSe detectó que otra terminal completó el cupo simultáneamente.\nSu reserva excedente ha sido cancelada automaticamete para respetar el limite de 5.", "Ajuste de Sistema", wxOK | wxICON_EXCLAMATION);
                    } else {
                        // Todo salio bien
                        wxMessageBox("Reserva guardada localmente.", "Info");
                    }

                } else {
                    Log::error("Fallo al guardar reserva local");
                    wxMessageBox("Reserva creada en memoria pero fallo el guardado en disco.", "Alerta");
                }
                
                // Actualizar UI
                RefreshList();
                
                // Actualizar BST / Hash con la lista final saneada
                app->getBST().clear();
                app->getLista().construirBST(app->getBST());
                
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
            // Reconstruccion del Arbol BST (Requerimiento explícito)
            app->getBST().clear();
            app->getLista().construirBST(app->getBST());
            
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
    MyApp* app = (MyApp*)wxApp::GetInstance();
    int n = 0;
    
    // Obtener arreglo dinamico fresco
    Reserva** arr = app->getLista().getReservasArray(n);
    if (!arr) {
         listView->DeleteAllItems();
         return;
    }

    // Aplicar orden activo
    if (currentSortMode == SORT_NAME) {
         // Sort Manual (Bubble)
         bubbleSortManual(arr, n, [](Reserva* a, Reserva* b) {
             return a->getNombres() > b->getNombres(); // > para Ascendente en Bubble
         });
    }
    else if (currentSortMode == SORT_CEDULA) {
         bubbleSortManual(arr, n, [](Reserva* a, Reserva* b) {
             return a->getCedula() > b->getCedula();
         });
    }
    else {
         // SORT_ID (Default)
         bubbleSortManual(arr, n, [](Reserva* a, Reserva* b) {
             return a->getIdReserva() > b->getIdReserva();
         });
    }

    UpdateListFromArray(arr, n);
    delete[] arr; // Siempre liberar
}
