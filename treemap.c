#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  Pair *pair;
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
};

struct TreeMap {
  TreeNode *root;
  TreeNode *current;
  int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2) {
  if (tree->lower_than(key1, key2) == 0 && tree->lower_than(key2, key1) == 0)
    return 1;
  else
    return 0;
}

TreeNode *createTreeNode(void *key, void *value) {
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
  if (new == NULL)
    return NULL;
  new->pair = (Pair *)malloc(sizeof(Pair));
  new->pair->key = key;
  new->pair->value = value;
  new->parent = new->left = new->right = NULL;
  return new;
}

TreeMap *createTreeMap(int (*lower_than)(void *key1, void *key2)) {
  TreeMap *map = (TreeMap *)malloc(sizeof(TreeMap));
  if (map == NULL) {
    return NULL;
  }
  map->root = NULL;
  map->current = map->root;
  map->lower_than = lower_than;

  return map;
}

void insertTreeMap(TreeMap *tree, void *key, void *value) {
  if (tree == NULL || key == NULL || value == NULL)
    return;
  if (tree->root == NULL) {
    tree->root = createTreeNode(key, value);
  } else {
    TreeNode *aux = tree->root;
    while (aux != NULL) {
      if (is_equal(tree, key, aux->pair->key)) {
        return;
      } else {
        if (tree->lower_than(key, aux->pair->key) == 1) {
          if (aux->left == NULL) {
            aux->left = createTreeNode(key, value);
            aux->left->parent = aux;
            tree->current = aux->left;
          }
          aux = aux->left;
        } else {
          if (aux->right == NULL) {
            aux->right = createTreeNode(key, value);
            aux->right->parent = aux;
            tree->current = aux->right;
          }
          aux = aux->right;
        }
      }
    }
  }
}

TreeNode *minimum(TreeNode *x) {
  TreeNode *z = x;
  if (z == NULL)
    return NULL;
  while (z->left != NULL)
    z = z->left;
  return z;
}

void removeNode(TreeMap * tree, TreeNode* node) {
  if (node == NULL)
    return;

  if (node->left == NULL && node->right == NULL) {
    if (node->parent == NULL) {
      // Si el nodo es la raíz
      tree->root = NULL;
    } 
    else {
    // Eliminar referencia del padre al nodo
      if (node->parent->left == node)
        node->parent->left = NULL;
      else
        node->parent->right = NULL;
    }
    free(node); // Liberar memoria del nodo
    return;
  }

  // Caso 2: El nodo tiene un solo hijo
  if (node->left == NULL || node->right == NULL) {
    TreeNode* child = (node->left != NULL) ? node->left : node->right;

    // Actualizar el padre del hijo
    child->parent = node->parent;

    // Actualizar la raíz si es necesario
    if (node->parent == NULL)
      tree->root = child;
    else {
      // Actualizar la referencia del padre al hijo
      if (node->parent->left == node)
        node->parent->left = child;
      else
        node->parent->right = child;
    }

    free(node); // Liberar memoria del nodo
    return;
  }

  // Caso 3: El nodo tiene dos hijos
  // En este caso, necesitas encontrar el sucesor en orden del nodo para reemplazarlo.
  TreeNode* successor = node->right;
  while (successor->left != NULL)
    successor = successor->left;

  // Copiar el par del sucesor al nodo que se está eliminando
  node->pair = successor->pair;

  // Eliminar el sucesor recursivamente
  removeNode(tree, successor);
}



void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) { 
    TreeNode *aux = tree->root;
    while (aux != NULL){
        if(is_equal(tree,key, aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        }
        else{
            if(tree->lower_than(key, aux->pair->key)) aux = aux->left;
            else aux= aux->right;
    
        }
    }
    return NULL; }

Pair *upperBound(TreeMap *tree, void *key) { return NULL; }

Pair *firstTreeMap(TreeMap *tree) { 
    TreeNode *aux = tree->root;
    while(aux->left != NULL){
        aux = aux->left;
    }
    tree->current = aux;
    return aux->pair;
}

Pair *nextTreeMap(TreeMap *tree) {
  if (tree == NULL || tree->current == NULL) return NULL;
  TreeNode *aux = tree->current;

  // Si el nodo tiene un subárbol derecho, el sucesor será el nodo más a la izquierda de ese subárbol
  if (aux->right != NULL) {
    aux = aux->right;
    while (aux->left != NULL) aux = aux->left;
    tree->current = aux; // Actualizar el nodo actual en el árbol
    return aux->pair; // Retornar el par clave-valor del sucesor
  }

  // Si el nodo no tiene subárbol derecho, el sucesor será el primer ancestro derecho
  while (aux->parent != NULL && aux->parent->right == aux) aux = aux->parent;
  aux = aux->parent;
  if (aux != NULL) {
    tree->current = aux; // Actualizar el nodo actual en el árbol
    return aux->pair; // Retornar el par clave-valor del sucesor
  }

  // Si no hay sucesor, retornar NULL
  return NULL;
}
