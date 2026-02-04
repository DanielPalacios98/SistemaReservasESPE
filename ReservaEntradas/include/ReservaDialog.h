#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>

class ReservaDialog : public wxDialog {
public:
    ReservaDialog(wxWindow* parent);

    // Getters para los datos validados
    wxString getNombres() const;
    wxString getCedula() const;
    wxString getTelefono() const;
    wxString getCorreo() const;
    wxString getLocalidad() const;
    int getAsientos() const;

private:
    void OnSave(wxCommandEvent& event);
    
    // Validadores individuales
    void OnValNombre(wxCommandEvent& event);
    void OnValCedula(wxCommandEvent& event);
    void OnValTelefono(wxCommandEvent& event);
    void OnValCorreo(wxCommandEvent& event);

    wxTextCtrl* txtNombres;
    wxButton* btnValNombres;

    wxTextCtrl* txtCedula;
    wxButton* btnValCedula;

    wxTextCtrl* txtTelefono;
    wxButton* btnValTelefono;

    wxTextCtrl* txtCorreo;
    wxButton* btnValCorreo;

    wxComboBox* cmbLocalidad;
    wxSpinCtrl* spinAsientos;
    wxButton* btnSave;

    // Control de cupo
    int cupoDisponibleUsuario = 5;

    wxDECLARE_EVENT_TABLE();
};
