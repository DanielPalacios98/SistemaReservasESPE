#pragma once
#include "Usuario.h"
#include <string>
#include <vector>
using namespace std;

class HashTableUsuarios {
private:
    vector<vector<Usuario>> buckets;
    size_t capacidad;

    size_t hashCedula(const string& cedula) const {
        unsigned long h = 0;
        for (char c : cedula) {
            h = h * 31 + static_cast<unsigned char>(c);
        }
        return h % capacidad;
    }

public:
    explicit HashTableUsuarios(size_t cap = 101)
        : buckets(cap), capacidad(cap) {}

    bool insertar(const Usuario& u) {
        size_t idx = hashCedula(u.getCedula());
        auto& lista = buckets[idx];
        for (const auto& existente : lista) {
            if (existente.getCedula() == u.getCedula()) {
                return false; // cédula duplicada
            }
        }
        lista.push_back(u);
        return true;
    }

    bool existe(const string& cedula) const {
        size_t idx = hashCedula(cedula);
        const auto& lista = buckets[idx];
        for (const auto& u : lista) {
            if (u.getCedula() == cedula) return true;
        }
        return false;
    }

    const Usuario* obtener(const string& cedula) const {
        size_t idx = hashCedula(cedula);
        const auto& lista = buckets[idx];
        for (const auto& u : lista) {
            if (u.getCedula() == cedula) return &u;
        }
        return nullptr;
    }
};
