# Sistema de Reservas ESPE (Rama Mongo + CMake)

Esta rama (eature/mongo-cmake) introduce un sistema de construcción moderno basado en **CMake** y gestión de dependencias con **vcpkg**, además de integración completa con **MongoDB Atlas**.

## Requisitos Previos

Para compilar y ejecutar esta versión, necesitas:

1.  **Visual Studio Build Tools 2022** (o Visual Studio Community):
    *   Componente "Desarrollo para el escritorio con C++".
    *   **Importante:** Asegúrate de instalar el **Windows SDK 10** o 11.
2.  **CMake** (versión 3.21 o superior).
3.  **vcpkg**: Gestor de paquetes de C++ de Microsoft.

## Configuración del Entorno (vcpkg)

Si aún no tienes vcpkg instalado:

\\\powershell
git clone https://github.com/microsoft/vcpkg.git c:\vcpkg
cd c:\vcpkg
.\bootstrap-vcpkg.bat
# Agrega c:\vcpkg al PATH de tu sistema (opcional pero recomendado)
\\\

## Compilación

El proyecto utiliza un manifiesto (\cpkg.json\) para descargar e instalar automáticamente las dependencias (\mongo-cxx-driver\, \
lohmann-json\, \wxwidgets\, etc.).

1.  Abre una terminal en la carpeta \ReservaEntradas\:
    \\\powershell
    cd ReservaEntradas
    \\\

2.  **Configura el proyecto** (esto descargará y compilará las dependencias, puede tardar varios minutos la primera vez):
    \\\powershell
    cmake -S . -B build -DUSE_MONGO=ON -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
    # Ajusta la ruta a vcpkg.cmake según donde hayas instalado vcpkg
    \\\

3.  **Compila**:
    \\\powershell
    cmake --build build --config Release
    \\\

## Configuración de MongoDB

Para conectar a tu cluster de Atlas:

1.  Renombra el archivo \config.example.json\ a \config.json\.
2.  Edítalo con tu string de conexión:
    \\\json
    {
      "backend": "mongo",
      "mongoUri": "mongodb+srv://TU_USUARIO:TU_PASSWORD@cluster0.xxxxx.mongodb.net/?retryWrites=true&w=majority",
      "mongoDb": "Proyecto",
      "mongoCollection": "ReservaEntradas"
    }
    \\\
    *Nota: \config.json\ está ignorado por git para proteger tus credenciales.*

## Ejecución

\\\powershell
cd build/Release
.\ReservaEntradas.exe
\\\

## Estructura del Proyecto

*   \src/\, \include/\: Código fuente refactorizado.
*   \lib/\: Implementaciones de repositorios (Mongo, JSON) y modelos.
*   \cpkg.json\: Manifiesto de dependencias.
*   \CMakeLists.txt\: Configuración de construcción.

## Notas

*   Si la conexión a Mongo falla, el sistema intentará guardar en JSON local como fallback.
*   El binario generado es dependiente de las DLLs en su misma carpeta (copiadas automáticamente tras el build).
