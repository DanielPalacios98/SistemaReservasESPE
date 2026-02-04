#include "../lib/include/ListaReserva.h"
#include <iostream>

static int failures = 0;
static void assertEq(int a, int b, const char* msg){ if(a!=b){ std::cout << "FAIL: " << msg << " ("<<a<<" != "<<b<<")\n"; ++failures; } }

int main(){
    ListaReserva l;
    // Agregar tres reservas para la misma cedula con distintos asientos
    l.agregarReserva("Juan Perez", "1710000000", "0999999999", "juan@mail.com", "general", 2);
    l.agregarReserva("Juan Perez", "1710000000", "0999999999", "juan@mail.com", "general", 1);
    l.agregarReserva("Juan Perez", "1710000000", "0999999999", "juan@mail.com", "tribuna", 1);
    
    int total = l.contarAsientosPorCedula("1710000000");
    assertEq(total, 4, "Total asientos por cedula debe ser 4");

    // Validar maximo por usuario
    auto r = l.agregarReserva("Juan Perez", "1710000000", "0999999999", "juan@mail.com", "palco", 2);
    assertEq(r==nullptr ? 0:1, 0, "No debe permitir superar 5 asientos por usuario");

    // Validar cupo por localidad (usa constantes MAX_*). Intentar llenar general
    for(int i=0;i<30;i++) l.agregarReserva("A B", "1720000000", "0991111111", "a@b.com", "general", 1);
    auto r2 = l.agregarReserva("C D", "1730000000", "0992222222", "c@d.com", "general", 1);
    assertEq(r2==nullptr ? 0:1, 0, "No debe permitir exceder cupo de general");

    if(failures==0){ std::cout << "OK" << std::endl; return 0; }
    return 1;
}
