@echo off
REM Compila todos los archivos fuente y deja el ejecutable en bin/Debug/
g++ -Iinclude main.cpp src/ListaReserva.cpp src/NodoReserva.cpp src/Reserva.cpp -o bin/Debug/ReservaEntradas.exe
IF EXIST bin\Debug\ReservaEntradas.exe (
    echo Compilacion exitosa.
    bin\Debug\ReservaEntradas.exe
) ELSE (
    echo Error en la compilacion.
)
pause
