#ifndef AVL_H
#define AVL_H
#include "../sgbd.h"

void inicializar_avl(arvore_avl *raiz);
void imprimir_elemento_avl(arvore_avl raiz, tabela * tab);
arvore_avl inserir_avl (arvore_avl raiz, tipo_dado_avl* valor, int *cresceu);
arvore_avl remover_avl (arvore_avl raiz, char *valor, int *diminuiu);
arvore_avl rotacao(arvore_avl pivo);
void salvar_auxiliar_avl(arvore_avl raiz, FILE *arq);
void in_order_avl(arvore_avl raiz, tabela *tab);

#endif
