#include "ReservaDialog.h"
#include "App.h" // Para acceder a ListaReserva y validar cupos
#include "../lib/include/Reserva.h"

enum {
    ID_Save = 1001,
    ID_ValNom = 2001,
    ID_ValCed = 2002,
    ID_ValTel = 2003,
    ID_ValCor = 2004
};

wxBEGIN_EVENT_TABLE(ReservaDialog, wxDialog)
    EVT_BUTTON(ID_Save, ReservaDialog::OnSave)
    EVT_BUTTON(ID_ValNom, ReservaDialog::OnValNombre)
    EVT_BUTTON(ID_ValCed, ReservaDialog::OnValCedula)
    EVT_BUTTON(ID_ValTel, ReservaDialog::OnValTelefono)
    EVT_BUTTON(ID_ValCor, ReservaDialog::OnValCorreo)
wxEND_EVENT_TABLE()

ReservaDialog::ReservaDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Nueva Reserva", wxDefaultPosition, wxSize(550, 500))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    // 3 columnas: Label | Input | Bot�n Validar
    wxFlexGridSizer* formSizer = new wxFlexGridSizer(3, 10, 10);
    formSizer->AddGrowableCol(1, 1);

    // --- 1. Nombres ---
    formSizer->Add(new wxStaticText(this, wxID_ANY, "Nombres:"), 0, wxALIGN_CENTER_VERTICAL);
    txtNombres = new wxTextCtrl(this, wxID_ANY, "");
    txtNombres->SetHint("Ej: Juan Perez");
    formSizer->Add(txtNombres, 1, wxEXPAND);
    btnValNombres = new wxButton(this, ID_ValNom, "Validar");
    formSizer->Add(btnValNombres, 0);

    // --- 2. Cedula ---
    formSizer->Add(new wxStaticText(this, wxID_ANY, "Cedula:"), 0, wxALIGN_CENTER_VERTICAL);
    txtCedula = new wxTextCtrl(this, wxID_ANY, "");
    txtCedula->SetMaxLength(10);
    txtCedula->Enable(false); // Deshabilitado al inicio
    formSizer->Add(txtCedula, 1, wxEXPAND);
    btnValCedula = new wxButton(this, ID_ValCed, "Validar");
    btnValCedula->Enable(false);
    formSizer->Add(btnValCedula, 0);

    // --- 3. Telefono ---
    formSizer->Add(new wxStaticText(this, wxID_ANY, "Telefono:"), 0, wxALIGN_CENTER_VERTICAL);
    txtTelefono = new wxTextCtrl(this, wxID_ANY, "");
    txtTelefono->SetMaxLength(10);
    txtTelefono->Enable(false);
    formSizer->Add(txtTelefono, 1, wxEXPAND);
    btnValTelefono = new wxButton(this, ID_ValTel, "Validar");
    btnValTelefono->Enable(false);
    formSizer->Add(btnValTelefono, 0);

    // --- 4. Correo ---
    formSizer->Add(new wxStaticText(this, wxID_ANY, "Correo:"), 0, wxALIGN_CENTER_VERTICAL);
    txtCorreo = new wxTextCtrl(this, wxID_ANY, "");
    txtCorreo->Enable(false);
    formSizer->Add(txtCorreo, 1, wxEXPAND);
    btnValCorreo = new wxButton(this, ID_ValCor, "Validar");
    btnValCorreo->Enable(false);
    formSizer->Add(btnValCorreo, 0);

    // --- 5. Localidad ---
    formSizer->Add(new wxStaticText(this, wxID_ANY, "Localidad:"), 0, wxALIGN_CENTER_VERTICAL);
    wxArrayString choices;
    choices.Add("palco");
    choices.Add("tribuna");
    choices.Add("general");
    cmbLocalidad = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, choices, wxCB_READONLY);
    cmbLocalidad->SetSelection(2);
    cmbLocalidad->Enable(false);
    formSizer->Add(cmbLocalidad, 1, wxEXPAND);
    formSizer->Add(new wxStaticText(this, wxID_ANY, ""), 0); // Spacer

    // --- 6. Asientos ---
    formSizer->Add(new wxStaticText(this, wxID_ANY, "Asientos:"), 0, wxALIGN_CENTER_VERTICAL);
    spinAsientos = new wxSpinCtrl(this, wxID_ANY);
    spinAsientos->SetRange(1, 5);
    spinAsientos->SetValue(1);
    spinAsientos->Enable(false);
    formSizer->Add(spinAsientos, 1, wxEXPAND);
    formSizer->Add(new wxStaticText(this, wxID_ANY, ""), 0); // Spacer

    mainSizer->Add(formSizer, 1, wxEXPAND | wxALL, 20);

    // Botones Finales
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSave = new wxButton(this, ID_Save, "Guardar Reserva");
    btnSave->Enable(false); // Solo activo al final
    wxButton* btnCancel = new wxButton(this, wxID_CANCEL, "Cancelar");
    
    btnSizer->Add(btnSave, 0, wxALL, 5);
    btnSizer->Add(btnCancel, 0, wxALL, 5);

    mainSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxBOTTOM, 15);

    SetSizer(mainSizer);
    Center();
}

// --- Validadores Individuales ---

void ReservaDialog::OnValNombre(wxCommandEvent& event) {
    wxString val = txtNombres->GetValue().Trim(true).Trim(false);
    if (!Reserva::validarNombres(val.ToStdString())) {
        wxMessageBox("Error: Escriba nombre y apellido, cada palabra con Mayuscula inicial y resto minusculas, sin numeros ni caracteres especiales.", "Validacion");
        txtNombres->SetFocus();
    } else {
        // Exito paso 1
        txtNombres->Disable();
        btnValNombres->Disable();
        
        txtCedula->Enable(true);
        btnValCedula->Enable(true);
        txtCedula->SetFocus();
    }
}

void ReservaDialog::OnValCedula(wxCommandEvent& event) {
    wxString val = txtCedula->GetValue().Trim(true).Trim(false);
    if (!Reserva::validarCedula(val.ToStdString())) {
        wxMessageBox("Error: Cedula ecuatoriana invalida. Deben ser 10 digitos correctos.", "Validacion");
        txtCedula->SetFocus();
        return;
    }

    // Validacion de negocio: Maximo 5 boletos por usuario (Suma total de asientos)
    MyApp* app = (MyApp*)wxApp::GetInstance();
    
    // Intentamos consultar directo al repositorio (BD) primero
    int actuales = app->getRepository()->contarAsientos(val.ToStdString());
    
    // Si devuelve -1 (es JSON local o error), usamos la lista cargada en memoria
    if (actuales == -1) {
        actuales = app->getLista().contarAsientosPorCedula(val.ToStdString()); 
    }
    
    if (actuales >= 5) {
        wxMessageBox(wxString::Format("Error: Este usuario ya tiene %d boletos reservados (verificado en BD). El maximo es 5.", actuales), "Limite Alcanzado");
        // No permitimos avanzar
        return; 
    }

    // Calculamos cuantos puede comprar ahora
    cupoDisponibleUsuario = 5 - actuales;
    wxMessageBox(wxString::Format("Validacion correcta. El usuario tiene %d boletos previos.\nPuede comprar hasta %d mas.", actuales, cupoDisponibleUsuario), "Info");

    // Exito paso 2
    txtCedula->Disable();
    btnValCedula->Disable();
    
    txtTelefono->Enable(true);
    btnValTelefono->Enable(true);
    txtTelefono->SetFocus();
}

void ReservaDialog::OnValTelefono(wxCommandEvent& event) {
    wxString val = txtTelefono->GetValue().Trim(true).Trim(false);
    if (!Reserva::validarTelefono(val.ToStdString())) {
        wxMessageBox("Error: Telefono invalido. Debe tener 10 digitos y empezar en 09.", "Validacion");
        txtTelefono->SetFocus();
    } else {
        // Exito paso 3
        txtTelefono->Disable();
        btnValTelefono->Disable();
        
        txtCorreo->Enable(true);
        btnValCorreo->Enable(true);
        txtCorreo->SetFocus();
    }
}

void ReservaDialog::OnValCorreo(wxCommandEvent& event) {
    wxString val = txtCorreo->GetValue().Trim(true).Trim(false);
    if (!Reserva::validarCorreo(val.ToStdString())) {
        wxMessageBox("Error: Formato de correo invalido. Use ejemplo@dominio.com", "Validacion");
        txtCorreo->SetFocus();
    } else {
        // Exito paso 4 -> Habilitar selecci�n final
        txtCorreo->Disable();
        btnValCorreo->Disable();
        
        cmbLocalidad->Enable(true);
        
        // Configuramos el spinner segun el cupo restante
        spinAsientos->Enable(true);
        if (cupoDisponibleUsuario > 0) {
            spinAsientos->SetRange(1, cupoDisponibleUsuario);
            spinAsientos->SetValue(1);
        } else {
             // Caso hipotetico, no deberia llegar aqui por la validacion de cedula
             spinAsientos->Enable(false);
        }

        btnSave->Enable(true); // Ya puede guardar
        btnSave->SetFocus();
    }
}

void ReservaDialog::OnSave(wxCommandEvent& event) {
    // Validacion final de cupos (negocio)
    // Si bien paso las validaciones de formato, falta ver si al sumar los asientos nuevos se pasa del limite
    MyApp* app = (MyApp*)wxApp::GetInstance();
    // La verificacion de cupos por localidad la hace agregarReserva internamente y retorna nullptr si falla.
    
    // Preventivo: si el valor ingresado excede el cupo permitido, avisar y ajustar
    int seleccionados = spinAsientos->GetValue();
    if (seleccionados > cupoDisponibleUsuario) {
        wxMessageBox(wxString::Format(
            "Atencion: ha ingresado %d asientos, excede el maximo permitido (%d).\nSe ajusto automaticamente al valor permitido.",
            seleccionados, cupoDisponibleUsuario),
            "Validacion", wxOK | wxICON_INFORMATION);
        spinAsientos->SetValue(cupoDisponibleUsuario);
        spinAsientos->SetFocus();
        return; // mantener el dialogo abierto para que el usuario confirme
    }

    // Todo listo
    EndModal(wxID_OK);
}

wxString ReservaDialog::getNombres() const { return txtNombres->GetValue(); }
wxString ReservaDialog::getCedula() const { return txtCedula->GetValue(); }
wxString ReservaDialog::getTelefono() const { return txtTelefono->GetValue(); }
wxString ReservaDialog::getCorreo() const { return txtCorreo->GetValue(); }
wxString ReservaDialog::getLocalidad() const { return cmbLocalidad->GetValue(); }
int ReservaDialog::getAsientos() const { return spinAsientos->GetValue(); }
