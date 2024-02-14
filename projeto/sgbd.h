#ifndef SGBD_H
#define SGBD_H
#include "stdio.h"

typedef enum cores {VERMELHO, PRETO, DUPLO_PRETO} cores;

typedef struct poke_info {
    int poke_number;
    int poke_total_status;
    char *poke_name;
    char *poke_type1;
    char *poke_type2;
} poke_info;

typedef struct indice {
    int chave;
    int indice;
} tipo_dado;

typedef struct indice_avl {
    char poke_name[80]  ;
    int indice; 
} tipo_dado_avl;

typedef struct indice_rb {
    int poke_total_status;
    int indice; 
} tipo_dado_rb;

typedef struct no_bst {
    tipo_dado *dado;
    struct no_bst *esq, *dir;
} no_bst;

typedef struct no_avl {
    tipo_dado_avl *dado;
    int fb;
    struct no_avl *esq, *dir;
} no_avl;

typedef struct no_rb {
    tipo_dado_rb *dado;
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

typedef struct tab_realoc {
    FILE *arquivo_dados;
    arvore_bst indice_bst;
} tab_realoc;

poke_info* ler_dados();

void adicionar_pokemon(tabela *tabela, poke_info *pokemon);
void in_order(tabela *tab);
void tirar_enter(char *string);
void salvar_arquivo(tabela *tab);
void finalizar(tabela *tabela);
void busca_bst(tabela *tab, arvore_bst raiz, int chave);
void busca_avl(tabela *tab, arvore_avl raiz, char *nome);
void busca_rb(tabela *tab, arvore_rb raiz, int total_status);
void remover_indice(tabela *tabela, int chave);
void split_string(char *string, poke_info *pokemon);
void imprimir_elemento_bst(arvore_bst raiz, tabela * tab);

int inicializar_tabela(tabela *tabela);

#endif