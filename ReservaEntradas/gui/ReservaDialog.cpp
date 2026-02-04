#include "ReservaDialog.h"
#include "App.h" // Para acceder a ListaReserva y validar cupos
#include "../lib/include/Reserva.h"

enum {
    ID_Save = 1001,
    ID_ValNom = 2001,
    ID_ValCed = 2002,
    ID_ValTel = 2003,
    ID_ValCor = 2004,
    ID_ValAsientos = 2005
};

wxBEGIN_EVENT_TABLE(ReservaDialog, wxDialog)
    EVT_BUTTON(ID_Save, ReservaDialog::OnSave)
    EVT_BUTTON(ID_ValNom, ReservaDialog::OnValNombre)
    EVT_BUTTON(ID_ValCed, ReservaDialog::OnValCedula)
    EVT_BUTTON(ID_ValTel, ReservaDialog::OnValTelefono)
    EVT_BUTTON(ID_ValCor, ReservaDialog::OnValCorreo)
    EVT_BUTTON(ID_ValAsientos, ReservaDialog::OnValAsientos)
    EVT_CLOSE(ReservaDialog::OnClose)
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
    btnValAsientos = new wxButton(this, ID_ValAsientos, "Validar");
    btnValAsientos->Enable(false);
    formSizer->Add(btnValAsientos, 0);

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

    // Validacion de negocio: Intentar bloquear la cedula en la BD (transaccional)
    MyApp* app = (MyApp*)wxApp::GetInstance();
    IReservaRepository* repo = app->getRepository();
    
    // validarYBloquearCedula() valida cupo AND coloca un lock temporal de 30 segundos
    if (!repo->validarYBloquearCedula(val.ToStdString())) {
        wxMessageBox("Error: La cedula no puede registrarse.\n"
                     "Razon: Ya tiene maximo de asientos (5) O esta siendo utilizada en otro dispositivo.\n"
                     "Intente en unos segundos.", "Validacion");
        txtCedula->SetFocus();
        return; 
    }

    // Lock exitoso, almacenar cédula para desbloquear si se cancela
    cedulaBloqueada = val;
    
    wxMessageBox("Cedula validada y bloqueada temporalmente en la nube.\n"
                 "Puede continuar con el registro. Si cancela, el bloqueo se liberara automaticamente.",
                 "Info");

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
        // Exito paso 4 -> Habilitar seleccion final
        txtCorreo->Disable();
        btnValCorreo->Disable();
        
        cmbLocalidad->Enable(true);
        spinAsientos->Enable(true);
        btnValAsientos->Enable(true);
        
        spinAsientos->SetFocus();
        wxMessageBox("Ahora seleccione localidad y asientos, luego presione 'Validar' para verificar disponibilidad en tiempo real.", "Info");
    }
}

void ReservaDialog::OnValAsientos(wxCommandEvent& event) {
    MyApp* app = (MyApp*)wxApp::GetInstance();
    IReservaRepository* repo = app->getRepository();
    wxString cedula = txtCedula->GetValue();
    int asientosSeleccionados = spinAsientos->GetValue();
    
    // PASO 1: Verificar si la cedula sigue bloqueada (que nadie mas la este usando)
    // Intentar bloquear de nuevo para asegurar exclusividad
    if (!repo->validarYBloquearCedula(cedula.ToStdString())) {
        wxMessageBox("ERROR: Esta cedula esta siendo utilizada en este momento por otro usuario/PC.\n\n"
                     "Otra persona esta registrando una reserva con esta misma cedula.\n"
                     "Espere unos segundos e intente nuevamente.",
                     "Cedula en Uso", wxOK | wxICON_ERROR);
        return;
    }
    
    // PASO 2: VALIDACION EN TIEMPO REAL: Consultar BD inmediatamente
    int actualesEnBD = repo->contarAsientos(cedula.ToStdString());
    
    if (actualesEnBD == -1) {
        // Modo local, usar lista en memoria
        actualesEnBD = app->getLista().contarAsientosPorCedula(cedula.ToStdString());
    }
    
    int totalDespuesDeGuardar = actualesEnBD + asientosSeleccionados;
    
    if (totalDespuesDeGuardar > 5) {
        // Desbloquear antes de rechazar
        repo->desbloquearCedula(cedula.ToStdString());
        cedulaBloqueada = ""; // Limpiar para que OnClose no intente desbloquear
        
        wxMessageBox(wxString::Format(
            "ERROR: Esta cedula actualmente tiene %d asientos registrados en la base de datos.\n"
            "Si guarda %d asientos mas, superaria el limite de 5 asientos.\n\n"
            "Posiblemente otro usuario registro mas asientos mientras usted llenaba el formulario.\n"
            "Cierre este formulario y presione 'Recargar' para sincronizar.",
            actualesEnBD, asientosSeleccionados),
            "Cupo Insuficiente", wxOK | wxICON_ERROR);
        
        // Sugerir cupo disponible
        int disponible = 5 - actualesEnBD;
        if (disponible > 0) {
            wxMessageBox(wxString::Format("Esta cedula solo puede registrar %d asiento(s) mas.", disponible), "Info");
        } else {
            wxMessageBox("Esta cedula ya alcanzo el limite maximo de 5 asientos.", "Limite Alcanzado");
        }
        return;
    }
    
    // TODO BIEN: Actualizar bloqueo y habilitar guardar
    cedulaBloqueada = cedula; // Actualizar para mantener el bloqueo
    
    wxMessageBox(wxString::Format(
        "Validacion exitosa y cedula bloqueada.\n\n"
        "Asientos actuales en BD: %d\n"
        "Asientos a registrar: %d\n"
        "Total despues de guardar: %d/5\n\n"
        "Presione 'Guardar Reserva' AHORA para completar.\n"
        "Si demora, el bloqueo expirara en 30 segundos.",
        actualesEnBD, asientosSeleccionados, totalDespuesDeGuardar),
        "Cupo Confirmado", wxOK | wxICON_INFORMATION);
    
    btnValAsientos->Disable();
    spinAsientos->Disable();
    cmbLocalidad->Disable();
    btnSave->Enable(true);
    btnSave->SetFocus();
}

void ReservaDialog::OnSave(wxCommandEvent& event) {
    // La validación ya se hizo en OnValAsientos, solo cerrar el diálogo
    EndModal(wxID_OK);
}

wxString ReservaDialog::getNombres() const { return txtNombres->GetValue(); }
wxString ReservaDialog::getCedula() const { return txtCedula->GetValue(); }
wxString ReservaDialog::getTelefono() const { return txtTelefono->GetValue(); }
wxString ReservaDialog::getCorreo() const { return txtCorreo->GetValue(); }
wxString ReservaDialog::getLocalidad() const { return cmbLocalidad->GetValue(); }
int ReservaDialog::getAsientos() const { return spinAsientos->GetValue(); }
void ReservaDialog::OnClose(wxCloseEvent& event) {
    // Si se cierra el dialogo sin guardar, desbloquear la cedula
    if (!cedulaBloqueada.IsEmpty()) {
        MyApp* app = (MyApp*)wxApp::GetInstance();
        app->getRepository()->desbloquearCedula(cedulaBloqueada.ToStdString());
    }
    event.Skip(); // Permitir que se cierre normalmente
}