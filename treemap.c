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

void removeNode(TreeMap *tree, TreeNode *node) {
  if (tree == NULL || node == NULL)
    return;

  // Caso 1: Nodo sin hijos
  if (node->left == NULL && node->right == NULL) {
    if (node->parent != NULL) {
      if (node->parent->left == node)
        node->parent->left = NULL;
      else
        node->parent->right = NULL;
    } else {
      // Si el nodo a eliminar es la raíz
      tree->root = NULL;
    }
    free(node->pair);
    free(node);
  }
  // Caso 2: Nodo con un solo hijo
  else if (node->left == NULL || node->right == NULL) {
    TreeNode *child = (node->left != NULL) ? node->left : node->right;
    if (node->parent != NULL) {
      if (node->parent->left == node)
        node->parent->left = child;
      else{
        node->parent->right = child;
      child->parent = node->parent; }
    }else {
      // Si el nodo a eliminar es la raíz
      tree->root = child;
      child->parent = NULL;
    }
    free(node->pair);
    free(node);
  }
  // Caso 3: Nodo con dos hijos
  else {
    // Encontrar el sucesor inmediato
    TreeNode *successor = minimum(node->right);

    // Copiar el par de clave-valor del sucesor al nodo actual
    node->pair = successor->pair;

    // Eliminar el sucesor (puede ser un nodo con 0 o 1 hijo)
    removeNode(tree, successor);
  }
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
  if (tree == NULL|| tree->current == NULL) return NULL;
  TreeNode *aux = tree->current;
  if(aux->right != NULL) {
    aux = aux->right;
    while(aux->left != NULL) aux = aux->left;
  }
  else{
    while (aux->parent != NULL && aux->parent->pair->value < aux->pair->value) aux = aux->parent;
  }
  tree->current = aux;
  return aux->pair; }
