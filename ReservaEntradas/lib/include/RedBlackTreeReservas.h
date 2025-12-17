#pragma once
#include "Reserva.h"
#include <string>
#include <vector>

enum Color { RED, BLACK };

struct RBNode {
    std::string key;  // Clave: teléfono
    std::vector<Reserva*> reservas;  // Lista de reservas para esta clave
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;

    RBNode(std::string k, Reserva* r) : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {
        reservas.push_back(r);
    }
};

class RedBlackTreeReservas {
private:
    RBNode* root;

    void leftRotate(RBNode* x);
    void rightRotate(RBNode* x);
    void insertFixup(RBNode* z);
    RBNode* search(RBNode* node, const std::string& key) const;

public:
    RedBlackTreeReservas() : root(nullptr) {}
    ~RedBlackTreeReservas();

    void insertar(const std::string& key, Reserva* r);
    std::vector<Reserva*> buscar(const std::string& key) const;
};