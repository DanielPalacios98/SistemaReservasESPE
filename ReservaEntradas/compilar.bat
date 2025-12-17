@echo off
cd /d "%~dp0"

if not exist bin\Debug (
    mkdir bin\Debug
)

echo Cerrando cualquier instancia del programa...
taskkill /f /im ReservaEntradas.exe >nul 2>&1

echo Compilando DLL...
g++ -shared -o lib\SistemaReservas.dll lib\SistemaReservas.cpp -Ilib\include

if errorlevel 1 (
    echo.
    echo *** ERROR DE COMPILACION DLL ***
    pause
    exit /b 1
)

echo Compilando ejecutable...

g++ main.cpp -Llib -lSistemaReservas -o bin\Debug\ReservaEntradas.exe

if errorlevel 1 (
    echo.
    echo *** ERROR DE COMPILACION ***
    pause
    exit /b 1
)

echo.
echo === FIN DE COMPILACION ===

echo.
echo Ejecutando ReservaEntradas.exe...
echo.

bin\Debug\ReservaEntradas.exe

echo.
echo Programa finalizado.
pause
