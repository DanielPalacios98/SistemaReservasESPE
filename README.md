# README.md – Sistema de Reservas ESPE

## Descripción

Este proyecto es un sistema de reservas de asientos implementado en C++ para eventos o auditorios.  
Permite gestionar entradas de usuarios, validar los datos, asignar localidades y controlar el uso de cupos de manera robusta y segura.

***

## Características principales

- Validación estricta de todos los datos de entrada
    - **Nombres y Apellidos:** Solo formato "Nombre Apellido", inicial mayúscula en cada palabra
    - **Cédula:** Algoritmo oficial para Ecuador
    - **Teléfono:** 10 dígitos, formato móvil ecuatoriano
    - **Correo:** Formato estándar
    - **Localidad:** Opciones fijas: palco, tribuna o general
    - **Asientos:** Entre 1-5 por reserva, máximo 5 por usuario / cupo máximo por localidad

- **Menú interactivo:**  
  - Registro de nueva reserva
  - Lista de reservas
  - Eliminación por ID
  - Mostrar reservas ordenadas por nombre/cédula (QuickSort, orden perfecto)
  - Validación que impide avanzar con datos erróneos

- **Estructura modular:**  
  - Archivos organizados en carpetas por tipo (`src`, `include`, `bin`)
  - Lista circular para manejo dinámico de reservas

- **Automatización:**  
  - Archivo `compilar.bat` para compilar y ejecutar el sistema con un solo clic

***

## Estructura del repositorio

```
SistemaReservasESPE/
  ├─ src/                 # Archivos fuente (.cpp)
  ├─ include/             # Archivos de cabecera (.h)
  ├─ bin/Debug/           # Ejecutable y archivos de salida
  ├─ compilar.bat         # Script de automatización
  ├─ reservas.txt         # Archivo de persistencia de reservas
  └─ README.md            # Este archivo
```

***

## Instrucciones de compilación y ejecución

**Requisitos:**  
- Windows, compilador g++ instalado  
- Editor recomendado: Visual Studio Code

**Compilación automática:**  
- Ejecuta `compilar.bat` (doble clic o desde terminal con `compilar.bat`)
    - Si la compilación es exitosa, inicia el programa automáticamente

***

## Uso

1. Ejecuta el sistema siguiendo las instrucciones anteriores
2. Ingresa los datos solicitados en el menú
3. Cada dato será validado; no podrás avanzar con valores erróneos
4. Puedes listar, ordenar o eliminar reservas desde el menú

***

## Ejemplo de entrada para registro

```
Nombres (ejemplo: Daniel Palacios): Daniel Palacios
Cedula (10 dígitos ecuatorianos): 1710999648
Telefono (10 dígitos, inicia con 09): 0979283932
Correo: daniel@gmail.com
Localidad (palco/tribuna/general): palco
Cantidad de asientos (1-5): 3
```

***

## Mejoras técnicas implementadas

- Flujo a prueba de errores y entradas maliciosas
- Criterios de validación ajustados a normativas reales ecuatorianas
- Ordenamiento seguro y eficiente por nombre y cédula usando QuickSort
- Mensajes y menú en español, listos para usuario final
- Código preparado para crecer (editar, buscar, reportar)

***

## Autor

Daniel Palacios  
Estudiante de Ingeniería de Desarrollo de Software – ESPE  
GitHub: [DanielPalacios98](https://github.com/DanielPalacios98/SistemaReservasESPE)

***

## Contacto y colaboración

¿Quieres mejorar este proyecto o aplicarlo en otra situación?  
Puedes forkear el repositorio, enviar un pull request o contactarme directamente por GitHub.

***

## Licencia

Este proyecto es académico y abierto; puedes usarlo y modificarlo libremente citando al autor.

***