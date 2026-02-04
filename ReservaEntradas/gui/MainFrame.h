#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/timer.h>
#include "../lib/include/ListaReserva.h"

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

    void RefreshList();

    wxListView* listView;
    // Auto-refresh deshabilitado
    wxTimer* refreshTimer = nullptr;

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_Hello = 1,
    ID_Refresh = 2,
    ID_Add = 3,
    ID_Delete = 4
};
