# Modelo UML (Texto)

## Módulos
- Dominio: `Reserva`, `Usuario`
- Estructuras: `NodoReserva`, `ListaReserva`, `BSTReservas`
- Servicios: `HashTableUsuarios`
- Aplicación: `main`

## Clases y responsabilidades
- Reserva: entidad de reserva con `idReserva`, `nombres`, `cedula`, `telefono`, `correo`, `localidad`, `numAsientos`. Valida campos (nombres, cédula, teléfono, correo, localidad) y muestra detalle.
- Usuario: datos base asociados a una `cedula` (nombres, telefono, correo) para reutilización en reservas.
- NodoReserva: nodo de lista circular simple con puntero a `Reserva` y a `NodoReserva` siguiente.
- ListaReserva: lista circular de `Reserva`.
  - Inserción, eliminación por ID, búsqueda por nombre/telefono.
  - Persistencia: `cargarDesdeArchivo`, `guardarEnArchivo`.
  - Recorridos y ordenamiento sin `vector`: iteración directa o arreglo dinámico temporal.
  - Integración: `construirBST(BSTReservas&)` inserta IDs en el árbol.
- BSTReservas: árbol binario de búsqueda por `idReserva`.
  - Inserción y búsqueda (`buscar(int)` retorna `Reserva*`).
  - Destrucción controlada de nodos.
- HashTableUsuarios: tabla hash por `cedula`.
  - Buckets: arreglo `NodoUsuario**` con encadenamiento.
  - Operaciones: `insertar`, `existe`, `obtener`.

## Relaciones
- `ListaReserva` posee nodos `NodoReserva` y referencia a `Reserva` (no la destruye al eliminar el nodo si otra estructura aún la usa).
- `BSTReservas` referencia `Reserva` insertada desde `ListaReserva` (no duplica contenidos, usa punteros).
- `HashTableUsuarios` almacena `Usuario` accesible por `cedula` y se consulta desde `main` para prellenar datos al registrar reservas.
- `main` orquesta:
  - Carga reservas desde archivo → construye `BSTReservas` desde la lista.
  - Menu de operaciones: registrar, listar, ordenar, buscar (por ID vía BST, nombre/telefono vía lista).

## Memoria y ownership
- `Reserva` se crea en `ListaReserva` al registrar y se enlaza en `NodoReserva`.
- `BSTReservas` mantiene punteros a `Reserva` (no copia). Liberación centralizada al vaciar la lista.
- `HashTableUsuarios` crea y mantiene `Usuario` por cuenta y riesgo propio.

## Paquetes y estructura de carpetas
- lib/include: headers (`Reserva.h`, `Usuario.h`, `NodoReserva.h`, `ListaReserva.h`, `BSTReservas.h`, `HashTableUsuarios.h`).
- lib/: fuentes (`Reserva.cpp`, `NodoReserva.cpp`, `ListaReserva.cpp`).
- bin/Debug: artefacto ejecutable (`ReservaEntradas.exe`).
- UML/: documentación textual (`UML.md`).
- Raíz: `main.cpp`, `compilar.bat`, `reservas.txt`.

## Diagrama (texto simplificado)
- ListaReserva o--- NodoReserva --> Reserva
- ListaReserva --> BSTReservas (construcción por ID)
- main --> ListaReserva, BSTReservas, HashTableUsuarios
- HashTableUsuarios o--- Usuario (key: cedula)
