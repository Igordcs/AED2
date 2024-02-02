#ifndef BST_H
#define BST_H
#include "../sgbd.h"

void inicializar_bst (arvore_bst *raiz);
arvore_bst inserir_bst (arvore_bst raiz, tipo_dado* valor);
arvore_bst remover_bst (arvore_bst raiz, int chave);
void imprimir_elemento_bst(arvore_bst raiz, tabela * tab);

#endif