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
  TreeNode *newNode = createTreeNode(key, value);
  if (newNode == NULL) {
    // Manejar el error de asignación de memoria
    return;
  }

  // Si el árbol está vacío, el nuevo nodo se convierte en la raíz
  if (tree->root == NULL) {
    tree->root = newNode;
    return;
  }

  TreeNode *current = tree->root;
  TreeNode *parent = NULL;

  // Recorrer el árbol para encontrar la posición adecuada para insertar el
  // nuevo nodo
  while (current != NULL) {
    parent = current;
    if (tree->lower_than(newNode->pair->key, current->pair->key) < 0) {
      current = current->left;
    } else {
      current = current->right;
    }
  }

  // Insertar el nuevo nodo como hijo del nodo adecuado
  if (tree->lower_than(newNode->pair->key, parent->pair->key) < 0) {
    parent->left = newNode;
  } else {
    parent->right = newNode;
  }
*current = *tree->root;
parent = NULL;

while (current != NULL) {
  parent = current;
  if (tree->lower_than(newNode->pair, current->pair) < 0) {
    current = current->left;
  } else {
    current = current->right;
  }
}
}

TreeNode *minimum(TreeNode *x) { return NULL; }

void removeNode(TreeMap *tree, TreeNode *node) {}

void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) { return NULL; }

Pair *upperBound(TreeMap *tree, void *key) { return NULL; }

Pair *firstTreeMap(TreeMap *tree) { return NULL; }

Pair *nextTreeMap(TreeMap *tree) { return NULL; }
