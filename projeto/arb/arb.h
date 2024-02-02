#ifndef ARB_H
#define ARB_H
#include "../sgbd.h"

void inicializar_rb(arvore_rb *raiz);
void adicionar_rb (tipo_dado *valor, arvore_rb *raiz);
void remover_rb (int chave, arvore_rb *raiz);
void rotacao_simples_direita(arvore_rb *raiz, arvore_rb pivo);
void rotacao_simples_esquerda(arvore_rb *raiz, arvore_rb pivo);
void reajustar(arvore_rb *raiz, arvore_rb elemento);
void retira_duplo_preto(arvore_rb *raiz, arvore_rb elemento);
void imprimir_elemento_rb(arvore_rb raiz, tabela * tab);
#endif