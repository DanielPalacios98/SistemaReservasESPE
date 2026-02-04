# Sistema de Reservas ESPE (Rama Mongo + CMake)

Esta rama (eature/mongo-cmake) introduce un sistema de construcci�n moderno basado en **CMake** y gesti�n de dependencias con **vcpkg**, adem�s de integraci�n completa con **MongoDB Atlas**.

## Requisitos Previos

Para compilar y ejecutar esta versi�n, necesitas:

1.  **Visual Studio Build Tools 2022** (o Visual Studio Community):
    *   Componente "Desarrollo para el escritorio con C++".
    *   **Importante:** Aseg�rate de instalar el **Windows SDK 10** o 11.
2.  **CMake** (versi�n 3.21 o superior).
3.  **vcpkg**: Gestor de paquetes de C++ de Microsoft.

## Configuraci�n del Entorno (vcpkg)

Si a�n no tienes vcpkg instalado:

\\\powershell
git clone https://github.com/microsoft/vcpkg.git c:\vcpkg
cd c:\vcpkg
.\bootstrap-vcpkg.bat
# Agrega c:\vcpkg al PATH de tu sistema (opcional pero recomendado)
\\\

## Compilaci�n

El proyecto utiliza un manifiesto (\cpkg.json\) para descargar e instalar autom�ticamente las dependencias (\mongo-cxx-driver\, \
lohmann-json\, \wxwidgets\, etc.).

1.  Abre una terminal en la carpeta \ReservaEntradas\:
    \\\powershell
    cd ReservaEntradas
    \\\

2.  **Configura el proyecto** (esto descargar� y compilar� las dependencias, puede tardar varios minutos la primera vez):
    \\\powershell
    cmake -S . -B build -DUSE_MONGO=ON -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
    # Ajusta la ruta a vcpkg.cmake seg�n donde hayas instalado vcpkg
    \\\

3.  **Compila**:
    \\\powershell
    cmake --build build --config Release
    \\\

## Configuraci�n de MongoDB

Para conectar a tu cluster de Atlas:

1.  Renombra el archivo \config.example.json\ a \config.json\.
2.  Ed�talo con tu string de conexi�n:
    \\\json
    {
      "backend": "mongo",
      "mongoUri": "mongodb+srv://TU_USUARIO:TU_PASSWORD@cluster0.xxxxx.mongodb.net/?retryWrites=true&w=majority",
      "mongoDb": "Proyecto",
      "mongoCollection": "ReservaEntradas"
    }
    \\\
    *Nota: \config.json\ est� ignorado por git para proteger tus credenciales.*

## Ejecuci�n

\\\powershell
cd build/Release
.\ReservaEntradas.exe
\\\

## Estructura del Proyecto

*   \src/\, \include/\: C�digo fuente refactorizado.
*   \lib/\: Implementaciones de repositorios (Mongo, JSON) y modelos.
*   \cpkg.json\: Manifiesto de dependencias.
*   \CMakeLists.txt\: Configuraci�n de construcci�n.

## Notas

*   Si la conexi�n a Mongo falla, el sistema intentar� guardar en JSON local como fallback.
*   El binario generado es dependiente de las DLLs en su misma carpeta (copiadas autom�ticamente tras el build).
