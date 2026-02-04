#include "MainFrame.h"
#include "App.h"
#include <iostream>

// =========================================================
// ALGORITMO 1: BUBBLE SORT (Burbuja)
// Complejidad: O(n^2)
// Estrategia: Intercambia adyacentes repetidamente
// =========================================================
void bubbleSortManual(Reserva** arr, int n, bool (*criterioMayor)(Reserva*, Reserva*)) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Punteros puros: *(arr + j)
            Reserva* a = *(arr + j);
            Reserva* b = *(arr + j + 1);

            if (criterioMayor(a, b)) {
                // Swap manual
                *(arr + j) = b;
                *(arr + j + 1) = a;
            }
        }
    }
}

// =========================================================
// ALGORITMO 2: SELECTION SORT (Seleccion)
// Complejidad: O(n^2) -> Menos intercambios que Burbuja
// Estrategia: Busca el menor elemento y lo pone al inicio
// =========================================================
void selectionSortManual(Reserva** arr, int n, bool (*criterioMayor)(Reserva*, Reserva*)) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        Reserva* minVal = *(arr + i);

        for (int j = i + 1; j < n; j++) {
            Reserva* actual = *(arr + j);
            
            // Si minVal > actual, entoces nuevo minimo es actual
            // Invertimos el criterioMayor para buscar el menor
            if (criterioMayor(minVal, actual)) { 
                minVal = actual;
                minIdx = j;
            }
        }

        // Swap si encontramos uno menor
        if (minIdx != i) {
            Reserva* temp = *(arr + i);
            *(arr + i) = *(arr + minIdx);
            *(arr + minIdx) = temp;
        }
    }
}

// =========================================================
// ALGORITMO 3: INSERTION SORT (Insercion)
// Complejidad: O(n^2) -> Muy rapido si esta casi ordenado
// Estrategia: Toma un elemento y lo inserta en su lugar correcto hacia atras
// =========================================================
void insertionSortManual(Reserva** arr, int n, bool (*criterioMayor)(Reserva*, Reserva*)) {
    for (int i = 1; i < n; i++) {
        Reserva* key = *(arr + i);
        int j = i - 1;

        // Movemos los elementos mayores a la derecha
        // Mientras (j >= 0) Y (arr[j] > key)
        while (j >= 0 && criterioMayor(*(arr + j), key)) {
            *(arr + j + 1) = *(arr + j);
            j = j - 1;
        }
        *(arr + j + 1) = key;
    }
}

// ---------------------------------------------------------
// FUNCIONES AUXILIARES DE CRITERIO
// ---------------------------------------------------------
bool compararPorID(Reserva* a, Reserva* b) {
    return a->getIdReserva() > b->getIdReserva();
}

bool compararPorCedula(Reserva* a, Reserva* b) {
    // Comparacion de strings (lexicografica)
    return a->getCedula() > b->getCedula();
}

bool compararPorNombre(Reserva* a, Reserva* b) {
    return a->getNombres() > b->getNombres();
}


void MainFrame::UpdateListFromArray(Reserva** arr, int n) {
    listView->DeleteAllItems();
    if (!arr) return;
    
    for (int i = 0; i < n; ++i) {
        Reserva* r = *(arr + i); 
        long index = listView->InsertItem(listView->GetItemCount(), wxString::Format("%d", r->getIdReserva()));
        listView->SetItem(index, 1, r->getNombres());
        listView->SetItem(index, 2, r->getCedula());
        listView->SetItem(index, 3, r->getTelefono());
        listView->SetItem(index, 4, r->getLocalidad());
        listView->SetItem(index, 5, wxString::Format("%d", r->getNumAsientos()));
    }
}

// ---------------------------------------------------------
// EVENTOS DE ORDENAMIENTO
// ---------------------------------------------------------

void MainFrame::OnSortId(wxCommandEvent& event) {
    // ORDENAMIENTO 1: BUBBLE SORT
    MyApp* app = (MyApp*)wxApp::GetInstance();
    int n = 0;
    Reserva** arr = app->getLista().getReservasArray(n);
    if (!arr) return;

    bubbleSortManual(arr, n, compararPorID);
    UpdateListFromArray(arr, n);
    
    currentSortMode = SORT_ID;
    SetStatusText("Ordenado por ID: Bubble Sort O(n^2)");
    wxMessageBox("Algoritmo usado: Bubble Sort (Burbuja)\nComplejidad: O(n^2)\nCriterio: ID", "Ordenamiento 1");
    delete[] arr;
}

void MainFrame::OnSortCedula(wxCommandEvent& event) {
    // ORDENAMIENTO 2: SELECTION SORT
    MyApp* app = (MyApp*)wxApp::GetInstance();
    int n = 0;
    Reserva** arr = app->getLista().getReservasArray(n);
    if (!arr) return;

    selectionSortManual(arr, n, compararPorCedula);
    UpdateListFromArray(arr, n);

    currentSortMode = SORT_CEDULA;
    SetStatusText("Ordenado por Cedula: Selection Sort O(n^2)");
    wxMessageBox("Algoritmo usado: Selection Sort (Seleccion)\nComplejidad: O(n^2)\nCriterio: Cedula\nNota: Minimiza el numero de intercambios.", "Ordenamiento 2");
    delete[] arr;
}

void MainFrame::OnSortName(wxCommandEvent& event) {
    // ORDENAMIENTO 3: INSERTION SORT
    MyApp* app = (MyApp*)wxApp::GetInstance();
    int n = 0;
    Reserva** arr = app->getLista().getReservasArray(n);
    if (!arr) return;

    insertionSortManual(arr, n, compararPorNombre);
    UpdateListFromArray(arr, n);

    currentSortMode = SORT_NAME;
    SetStatusText("Ordenado por Nombre: Insertion Sort O(n^2)");
    wxMessageBox("Algoritmo usado: Insertion Sort (Insercion)\nComplejidad: O(n^2)\nCriterio: Nombre\nNota: Muy eficiente para listas pequenias o casi ordenadas.", "Ordenamiento 3");
    delete[] arr;
}


// ---------------------------------------------------------
// EVENTOS DE BUSQUEDA
// ---------------------------------------------------------

void MainFrame::OnSearchLinear(wxCommandEvent& event) {
    // BUSQUEDA POR TELEFONO (LINEAL)
    // Se usa lineal porque el arbol no esta ordenado por telefono
    
    wxTextEntryDialog dlg(this, "Ingrese NUMERO DE TELEFONO a buscar:", "Busqueda Lineal (Telefono)");
    if (dlg.ShowModal() == wxID_OK) {
        string telefonoBuscado = dlg.GetValue().ToStdString();
        
        MyApp* app = (MyApp*)wxApp::GetInstance();
        int n = 0;
        Reserva** arr = app->getLista().getReservasArray(n);
        
        if (!arr) {
            wxMessageBox("La lista esta vacia.", "Info");
            return;
        }

        Reserva* encontrado = nullptr;
        int comparaciones = 0;
        
        // Algoritmo: BUSQUEDA LINEAL (Secuencial)
        // Recorre puntero por puntero hasta encontrar coincidencia
        for(int i = 0; i < n; i++) {
            comparaciones++;
            Reserva* actual = *(arr + i);
            
            if (actual->getTelefono() == telefonoBuscado) {
                encontrado = actual;
                break; 
            }
        }

        if (encontrado) {
            wxString info = wxString::Format(
                "--- BUSQUEDA LINEAL EXITOSA ---\n\n"
                "Criterio: Telefono (%s)\n"
                "Comparaciones: %d\n"
                "Complejidad: O(n)\n\n"
                "Resultado:\n"
                "ID: %d\nNombre: %s\nCedula: %s\n",
                telefonoBuscado, comparaciones,
                encontrado->getIdReserva(), encontrado->getNombres(), encontrado->getCedula());
            wxMessageBox(info, "Busqueda por Telefono", wxOK | wxICON_INFORMATION);
        } else {
            wxMessageBox(wxString::Format("No se encontro el telefono '%s' tras %d comparaciones.", telefonoBuscado, comparaciones), "Fallo Busqueda Lineal", wxOK | wxICON_ERROR);
        }
        
        delete[] arr; 
    }
}

void MainFrame::OnSearchBST(wxCommandEvent& event) {
    // BUSQUEDA POR ID (BINARIA / ARBOL)
    
    wxTextEntryDialog dlg(this, "Ingrese ID de Reserva a buscar:", "Busqueda Binaria (BST)");
    if (dlg.ShowModal() == wxID_OK) {
        long idVal;
        if (dlg.GetValue().ToLong(&idVal)) {
            MyApp* app = (MyApp*)wxApp::GetInstance();
            
            // La busqueda se delega al BST que tiene estructura arborea
            Reserva* r = app->getBST().buscar((int)idVal);
            
            if (r) {
                wxString info = wxString::Format(
                    "--- BUSQUEDA EN ARBOL BINARIO (BST) EXITOSA ---\n\n"
                    "Criterio: ID (%d)\n"
                    "Complejidad: O(log n) promedio\n"
                    "Nota: Divide el espacio de busqueda en dos en cada paso.\n\n"
                    "Resultado:\n"
                    "Nombre: %s\nCedula: %s\nLocalidad: %s",
                    r->getIdReserva(), r->getNombres(), r->getCedula(), r->getLocalidad());
                wxMessageBox(info, "Busqueda por ID (BST)", wxOK | wxICON_INFORMATION);
            } else {
                wxMessageBox("No se encontro ninguna reserva con ese ID en el BST.", "Fallo Busqueda BST", wxOK | wxICON_ERROR);
            }
        }
    }
}

void MainFrame::OnShellSort(wxCommandEvent& event) {
    // ESTE ES EL ORDENAMIENTO DE CARACTERES (SHELL SORT)
    MyApp* app = (MyApp*)wxApp::GetInstance();
    int n = 0;
    Reserva** arr = app->getLista().getReservasArray(n);
    
    if (!arr) {
        wxMessageBox("No hay datos.", "Info");
        return;
    }

    wxString reporte = "Reporte Shell Sort (Ordenamiento Interno de Caracteres):\n";
    reporte += "Algoritmo: Shell Sort\n";
    reporte += "Objetivo: Ordenar las letras del primer nombre de cada persona\n\n";
    
    for (int i = 0; i < n; i++) {
        Reserva* r = *(arr + i);
        string original = r->getNombres();
        string primerNombre = original.substr(0, original.find(' ')); 
        string procesado = ListaReserva::shellSortChars(primerNombre);
        
        reporte += wxString::Format("%s -> %s\n", primerNombre, procesado);
    }
    
    delete[] arr; 

    wxDialog* dlg = new wxDialog(this, wxID_ANY, "Shell Sort - Letras", wxDefaultPosition, wxSize(400, 500));
    wxTextCtrl* txt = new wxTextCtrl(dlg, wxID_ANY, reporte, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    
    wxBoxSizer* sz = new wxBoxSizer(wxVERTICAL);
    sz->Add(txt, 1, wxEXPAND | wxALL, 10);
    sz->Add(new wxButton(dlg, wxID_OK, "Cerrar"), 0, wxALIGN_CENTER | wxALL, 10);
    
    dlg->SetSizer(sz);
    dlg->ShowModal();
    dlg->Destroy();
}
