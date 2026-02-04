#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/timer.h>
#include "ListaReserva.h"

enum SortType { SORT_ID, SORT_NAME, SORT_CEDULA };

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnAdd(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    
    // Nuevas funcionalidades
    void OnSortName(wxCommandEvent& event);
    void OnSortCedula(wxCommandEvent& event);
    void OnSortId(wxCommandEvent& event); // Boton reset orden
    void OnShellSort(wxCommandEvent& event);
    void OnSearchBST(wxCommandEvent& event);
    void OnSearchLinear(wxCommandEvent& event); // Busqueda exhaustiva

    void RefreshList();
    void UpdateListFromArray(Reserva** arr, int n); 
    void LoadData(bool silent);

    wxListView* listView;
    // Auto-refresh timer
    wxTimer* refreshTimer = nullptr;
    
    // Estado persistente
    SortType currentSortMode = SORT_ID;

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_Hello = 1,
    ID_Refresh = 2,
    ID_Add = 3,
    ID_Delete = 4,
    ID_Timer = 5,
    ID_SortName = 6,
    ID_SortCedula = 7,
    ID_ShellSort = 8,
    ID_SearchBST = 9,
    ID_SortId = 10,
    ID_SearchLin = 11
};
