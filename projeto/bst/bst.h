#ifndef BST_H
#define BST_H
#include "../sgbd.h"

void inicializar_bst (arvore_bst *raiz);
arvore_bst inserir_bst (arvore_bst raiz, tipo_dado* valor);
arvore_bst remover_bst (arvore_bst raiz, int chave);
void in_order_bst(arvore_bst raiz, tabela *tab);
void imprimir_elemento_bst(arvore_bst raiz, tabela * tab);
void salvar_auxiliar_bst(arvore_bst raiz, FILE *arq);
tipo_dado* indice_sucessor(arvore_bst raiz, int tamanho);

#endif