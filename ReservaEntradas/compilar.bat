@echo off
cd /d "%~dp0"

if not exist bin\Debug (
    mkdir bin\Debug
)

echo Cerrando cualquier instancia del programa...
taskkill /f /im ReservaEntradas.exe >nul 2>&1

echo Compilando ejecutable...

g++ main.cpp lib\Reserva.cpp lib\NodoReserva.cpp lib\ListaReserva.cpp lib\mvc\ConsoleView.cpp lib\mvc\Controller.cpp -Ilib\include -o bin\Debug\ReservaEntradas.exe

if errorlevel 1 (
    echo.
    echo *** ERROR DE COMPILACION ***
    pause
    exit /b 1
)

echo.
echo === FIN DE COMPILACION ===

echo.
REM Si se pasa el argumento build-only, no ejecutar el programa
if /I "%~1"=="build-only" (
    echo.
    echo Modo build-only: no se ejecuta el programa.
    goto :EOF
)

echo Ejecutando ReservaEntradas.exe...
echo.

bin\Debug\ReservaEntradas.exe

echo.
echo Programa finalizado.
pause
