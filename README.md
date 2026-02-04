# Sistema de Reservas ESPE (Mongo + CMake)

Esta rama usa CMake + vcpkg y una GUI con wxWidgets. Soporta dos backends:
- MongoDB Atlas (modo nube) si compilas con USE_MONGO=ON y configuras config.json
- JSON local (reservas.json) como modo offline / fallback

## Que hace actualmente

- GUI para crear, listar, recargar y eliminar reservas
- Validaciones basicas (nombres, cedula, telefono, correo, localidad, asientos)
- Regla de negocio: maximo 5 asientos por cedula (total acumulado)
- Si Mongo falla al iniciar, la app avisa y carga datos locales (reservas.json)
- Script para generar un "Dist" portable (exe + dlls + config/recursos)

## Requisitos para compilar en otra PC (Windows)

- Windows 10/11 x64
- Visual Studio 2022 (Build Tools o Community) con:
  - Desktop development with C++
  - Windows SDK 10 u 11
- CMake 3.21+
- Git
- vcpkg (recomendado en C:\vcpkg)

Notas:
- El proyecto CMake esta en la carpeta ReservaEntradas/.
- config.json esta ignorado por git (por credenciales).

## Instalar vcpkg (una sola vez)

PowerShell:

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
created by DP y SD
