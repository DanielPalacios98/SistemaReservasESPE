#include "RedBlackTreeReservas.h"

void RedBlackTreeReservas::leftRotate(RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void RedBlackTreeReservas::rightRotate(RBNode* x) {
    RBNode* y = x->left;
    x->left = y->right;
    if (y->right != nullptr) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void RedBlackTreeReservas::insertFixup(RBNode* z) {
    while (z->parent != nullptr && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            if (y != nullptr && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            if (y != nullptr && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    if (root) root->color = BLACK;
}

void RedBlackTreeReservas::insertar(const std::string& key, Reserva* r) {
    RBNode* z = new RBNode(key, r);
    RBNode* y = nullptr;
    RBNode* x = root;
    while (x != nullptr) {
        y = x;
        if (key < x->key) x = x->left;
        else if (key > x->key) x = x->right;
        else {
            // Clave ya existe, agregar reserva al vector
            x->reservas.push_back(r);
            delete z;
            return;
        }
    }
    z->parent = y;
    if (y == nullptr) root = z;
    else if (key < y->key) y->left = z;
    else y->right = z;
    insertFixup(z);
}

RBNode* RedBlackTreeReservas::search(RBNode* node, const std::string& key) const {
    if (node == nullptr || node->key == key) return node;
    if (key < node->key) return search(node->left, key);
    return search(node->right, key);
}

std::vector<Reserva*> RedBlackTreeReservas::buscar(const std::string& key) const {
    RBNode* node = search(root, key);
    return node ? node->reservas : std::vector<Reserva*>();
}
