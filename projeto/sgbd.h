#ifndef SGBD_H
#define SGBD_H
#include "stdio.h"

typedef enum cores {VERMELHO, PRETO, DUPLO_PRETO} cores;

typedef struct pokemon {
    int poke_number;
    int poke_total_status;
    char poke_name[80];
    char poke_type1[36];
    char poke_type2[36];
} poke_info;

typedef struct indice {
    int chave;
    int indice;
} tipo_dado;

typedef struct no_bst {
    tipo_dado *dado;
    struct no_bst *esq, *dir;
} no_bst;

typedef struct no_avl {
    tipo_dado *dado;
    int fb;
    struct no_avl *esq, *dir;
} no_avl;

typedef struct no_rb {
    tipo_dado *dado;
    cores cor;
    struct no_rb *esq, *dir, *pai;
} no_rb;

typedef no_bst* arvore_bst;
typedef no_avl* arvore_avl;
typedef no_rb* arvore_rb;

typedef struct tabela {
    FILE *arquivo_dados;
    arvore_bst indice_bst;
    arvore_avl indice_avl;
    arvore_rb indice_rb;
} tabela;

poke_info* ler_dados();

void adicionar_pokemon(tabela *tabela, poke_info *pokemon);
void in_order(arvore_rb raiz, tabela *tab);
void tirar_enter(char *string);
void salvar_arquivo(tabela *tab);
void finalizar(tabela *tabela);
void busca(tabela *tab, arvore_avl raiz, int chave);
void remover_indice(tabela *tabela, int chave);

int inicializar_tabela(tabela *tabela);

#endif