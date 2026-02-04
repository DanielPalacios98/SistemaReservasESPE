#pragma once
#include "Usuario.h"
#include <string>

class NodoUsuario {
public:
    Usuario usuario;
    NodoUsuario* next;
    explicit NodoUsuario(const Usuario& u) : usuario(u), next(nullptr) {}
};

class HashTableUsuarios {
private:
    NodoUsuario** buckets;
    size_t capacidad;

    size_t hashCedula(const std::string& cedula) const {
        unsigned long h = 0;
        for (char c : cedula) {
            h = h * 31 + static_cast<unsigned char>(c);
        }
        return h % capacidad;
    }

public:
    explicit HashTableUsuarios(size_t cap = 101)
        : buckets(nullptr), capacidad(cap) {
        buckets = new NodoUsuario*[capacidad];
        for (size_t i = 0; i < capacidad; ++i) buckets[i] = nullptr;
    }

    ~HashTableUsuarios() {
        for (size_t i = 0; i < capacidad; ++i) {
            NodoUsuario* curr = buckets[i];
            while (curr) {
                NodoUsuario* next = curr->next;
                delete curr;
                curr = next;
            }
        }
        delete[] buckets;
    }

    bool insertar(const Usuario& u) {
        size_t idx = hashCedula(u.getCedula());
        NodoUsuario* curr = buckets[idx];
        while (curr) {
            if (curr->usuario.getCedula() == u.getCedula()) {
                return false; // cédula duplicada
            }
            curr = curr->next;
        }
        NodoUsuario* nuevo = new NodoUsuario(u);
        nuevo->next = buckets[idx];
        buckets[idx] = nuevo;
        return true;
    }

    bool existe(const std::string& cedula) const {
        size_t idx = hashCedula(cedula);
        NodoUsuario* curr = buckets[idx];
        while (curr) {
            if (curr->usuario.getCedula() == cedula) return true;
            curr = curr->next;
        }
        return false;
    }

    const Usuario* obtener(const std::string& cedula) const {
        size_t idx = hashCedula(cedula);
        NodoUsuario* curr = buckets[idx];
        while (curr) {
            if (curr->usuario.getCedula() == cedula) return &curr->usuario;
            curr = curr->next;
        }
        return nullptr;
    }
};
