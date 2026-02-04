#include <string>
#include <iostream>
// NO usamos <algorithm> ni <vector> para mantenerlo simple y manual
#include "ListaReserva.h"

// Función auxiliar para convertir a minuscula un char
// La hacemos manual para no depender de librerias complejas
char aMinuscula(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

// Implementación de Shell Sort para caracteres
// Lógica: Ordenamiento por inserción con saltos (gaps) decrecientes
// Complejidad aprox: O(n^1.5) dependiendo de la secuencia de gaps
string ListaReserva::shellSortChars(string nombre) {
    if (nombre.empty()) return "";
    
    // Copiamos el string a una variable local para trabajar
    string s = nombre;
    int n = s.length();
    
    // 1. Asignacion dinamica de memoria para trabajar los caracteres
    // Usamos punteros puros. 'buffer' apunta al primer caracter.
    char* buffer = new char[n + 1]; 
    
    // Copiamos manualmente y convertimos a minusculas
    for(int i = 0; i < n; i++) {
        // Aritmetica de punteros: *(buffer + i) es igual a buffer[i]
        // Guardamos la version minuscula para ordenar
        *(buffer + i) = aMinuscula(s.at(i));
    }
    *(buffer + n) = '\0'; // Caracter nulo al final

    // 2. Algoritmo Shell Sort
    // Gap empieza en n/2 y se reduce a la mitad en cada vuelta
    for (int gap = n/2; gap > 0; gap /= 2) {
        // Hacemos una especie de insercion para los elementos a distancia 'gap'
        for (int i = gap; i < n; i++) {
            char temp = *(buffer + i);
            int j;
            
            // Desplazamos elementos mientras sean mayores que 'temp'
            // Acceso sin corchetes: *(buffer + j - gap)
            for (j = i; j >= gap && *(buffer + j - gap) > temp; j -= gap) {
                *(buffer + j) = *(buffer + j - gap);
            }
            *(buffer + j) = temp;
        }
    }

    // 3. Restaurar mayuscula inicial (Estetica) y crear el string resultado
    // Si hay caracteres, ponemos el primero en mayuscula si es letra
    if (n > 0) {
        char primera = *buffer; // Lo mismo que buffer[0]
        if (primera >= 'a' && primera <= 'z') {
            *buffer = primera - ('a' - 'A');
        }
    }

    // Convertimos el buffer de char a string de C++
    string result = "";
    for(int i = 0; i < n; i++) {
        result += *(buffer + i);
    }
    
    delete[] buffer; // IMPORTANTE: Liberar la memoria manual
    return result;
}

// Vuelca la lista circular a un arreglo dinamico de punteros
// Se usa para poder ordenar linealmente sin romper la lista circular
Reserva** ListaReserva::getReservasArray(int& n) {
    n = 0;
    if (!head) return nullptr;
    
    // Paso 1: Contar cuantos nodos hay en la lista circular
    NodoReserva* temp = head;
    do {
        n++;
        temp = temp->next;
    } while (temp != head);
    
    // Paso 2: Crear el arreglo de punteros dinamicamente
    // Reserva** significa un puntero a un puntero de Reserva
    Reserva** arreglo = new Reserva*[n];
    
    // Paso 3: Llenar el arreglo usando aritmetica de punteros
    temp = head;
    for(int i = 0; i < n; i++) {
        // *(arreglo + i) accede a la posicion i
        *(arreglo + i) = temp->reserva;
        temp = temp->next;
    }
    
    return arreglo;
}

