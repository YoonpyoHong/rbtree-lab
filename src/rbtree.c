#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
//43.200.8.216 aws ip

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t*)calloc(1, sizeof(node_t));
  nil -> color = RBTREE_BLACK;
  nil -> parent = NULL;
  nil -> left = NULL;
  nil -> right = NULL;

  p -> nil = nil;
  p -> root = p->nil;
  return p;
}

node_t *left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil){
    y -> left -> parent = x;
  }
  y-> parent = x->parent;
  if (x-> parent == t->nil){
    t->root = y;
  }else if(x== x->parent->left){
    x -> parent ->left = y;
  }else{
    x -> parent -> right = y;
  }
  y->left = x;
  x->parent = y;
  return y;
}

node_t *right_rotate(rbtree *t, node_t *x){
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil){
    y->right->parent = x;
  }
  y-> parent = x->parent;
  if (x-> parent == t->nil){
    t->root = y;
  }else if(x == x->parent->left){
    x -> parent ->left = y;
  }else{
    x -> parent ->right = y;
  }
  y->right = x;
  x->parent = y;
  return y;
}

void delete_rbtree(rbtree *t) {
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

void delete_node(rbtree *t, node_t* x){
  if(x == t -> nil){
    return;
  }
  delete_node(t, x->left);
  delete_node(t, x->right);
  free(x);
  return;
}

void rbtree_insert_fixup(rbtree* t, node_t* z){
  while (z -> parent-> color == RBTREE_RED){
    if (z -> parent == z-> parent -> parent -> left){
      node_t *y = z-> parent -> parent -> right;
      if (y->color == RBTREE_RED){
        z -> parent ->color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent ->parent ->color = RBTREE_RED;
        z = z->parent->parent;
      }else {
        if (z == z -> parent ->right){
          z = z->parent;
          left_rotate(t,z);
        }
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, z -> parent ->parent);
      }
    }else{
      node_t *y = z-> parent -> parent -> left;
      if (y->color == RBTREE_RED){
        z -> parent ->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z -> parent -> parent ->color = RBTREE_RED;
        z = z->parent->parent;
      }else {
        if (z == z -> parent ->left){
          z = z->parent;
          right_rotate(t,z); 
        }
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        left_rotate(t, z -> parent -> parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *y = t -> nil;
  node_t *x = t -> root;
  while(x != t->nil){
    y = x;
    if (key < x-> key){
      x = x->left;
    }else{
      x = x->right;
    }
  }
  node_t *z = (node_t*)calloc(1,sizeof(node_t));
  z -> parent = y;
  if ( y == t-> nil ){
    t-> root = z;
  }else if (key < y-> key){
    y -> left = z;
  }else{
    y -> right = z;
  }
  z-> left = t -> nil;
  z-> right = t -> nil;
  z-> key = key;
  z-> color = RBTREE_RED;
  rbtree_insert_fixup(t, z);
  return z;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *x = t ->root;
  while (x != t->nil){
    if (key < x->key){
      x = x->left;
    }else if(key > x->key){
      x = x->right;
    }else{
      break;
    }
  }
  if (x == t->nil){
    return NULL;
  }else{
    return x;
  }
}

void print_rbtree(const rbtree *t, const node_t *x){
  if (x == t -> nil){
    printf("nil\n");
    return;
  }
  printf("key = %d\n", x->key);
  printf("left: ");
  print_rbtree(t, x->left);
  printf("right: ");
  print_rbtree(t, x->right);
}

node_t *rbtree_min(const rbtree *t) {
  node_t *x = t -> root;
  while(x -> left != t-> nil){
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *x = t -> root;
  while(x -> right != t-> nil){
    x = x->right;
  }
  return x;
}

void rbtree_transplant(rbtree*t, node_t *x, node_t *y){
  if (x -> parent == t -> nil){
    t-> root = y;
  }else if (x == x -> parent ->left){
    x->parent->left = y;
  }else{
    x->parent->right =y;
  }
  y->parent = x->parent;
}

void rbtree_erase_fixup(rbtree*t, node_t *z){
  while(z != t-> root && z-> color == RBTREE_BLACK){
    if(z== z-> parent -> left){
      node_t *w = z -> parent -> right;
      if(w -> color == RBTREE_RED){
        w-> color = RBTREE_BLACK;
        z -> parent -> color = RBTREE_RED;
        left_rotate(t, z-> parent);
        w = z -> parent -> right;
      }
      if(w-> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK){
        w -> color = RBTREE_RED;
        z = z-> parent;
      }
      else{
        if(w-> right -> color == RBTREE_BLACK){
          w -> left -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          right_rotate(t, w);
          w=  z -> parent ->right;
        }
        w -> color = z -> parent -> color;
        z-> parent -> color = RBTREE_BLACK;
        w -> right -> color = RBTREE_BLACK;
        left_rotate(t, z -> parent);
        z = t -> root;
      }
    }
    else{
      node_t *w = z -> parent -> left;
      if(w -> color == RBTREE_RED){
        w-> color = RBTREE_BLACK;
        z -> parent -> color = RBTREE_RED;
        right_rotate(t, z-> parent);
        w = z -> parent -> left;
      }
      if(w-> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK){
        w -> color = RBTREE_RED;
        z = z-> parent;
      }
      else{
        if(w-> left -> color == RBTREE_BLACK){
          w -> right -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          left_rotate(t, w);
          w=  z -> parent ->left;
        }
        w -> color = z -> parent -> color;
        z-> parent -> color = RBTREE_BLACK;
        w -> left -> color = RBTREE_BLACK;
        right_rotate(t, z -> parent);
        z = t -> root;
      }
    }
  }
  z -> color = RBTREE_BLACK;
}


int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y = p;
  node_t *x = (node_t*)malloc(sizeof(node_t));
  color_t y_original_color = y -> color;
  if (p -> left == t -> nil){
    node_t* x = p ->right;
    rbtree_transplant(t, p, p -> right);
  }else if(p->right == t->nil){
    node_t* x = p ->right;
    rbtree_transplant(t, p, p->left);
  }else{
    y = y-> right;
    while(y-> left != t->nil){
      y = y->left;
    }
    y_original_color = y->color;
    node_t* x = y->right;
    if(y->parent == p){
      x->parent = y;
    }
    else{
      rbtree_transplant(t, y, y->right);
      y -> right = p -> right;
      y-> right ->parent = y;
    } 
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p -> color;
  }
  free(p);
  if(y_original_color == RBTREE_BLACK){
    rbtree_erase_fixup(t, x);
  }
  free(x);
  return 0;
}
int rbtree_recursive_search(const rbtree *t, key_t *arr, const size_t n, const node_t *now, int idx){
  if (now -> left != t->nil){
    idx = rbtree_recursive_search(t, arr, n, now->left, idx);
  }
  arr[idx] = now->key;
  idx += 1;
  if (now -> right != t->nil){
    idx = rbtree_recursive_search(t, arr, n, now->right, idx);
  }
  return idx;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  rbtree_recursive_search(t, arr, n, t->root, 0);
  return 0;
}
