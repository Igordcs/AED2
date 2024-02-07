#include "bst.h"
#include <stdlib.h>

void inicializar_bst (arvore_bst *raiz) {
    *raiz = NULL;
}

arvore_bst inserir_bst (arvore_bst raiz, tipo_dado* valor){
    //caso base
    if(raiz == NULL) {
        //1. Alocar espaço em memória
        arvore_bst nova = (arvore_bst) malloc(sizeof(struct no_bst));
        //2. Inicializar o novo nó
        nova->dado = valor;
        nova->esq = NULL;
        nova->dir = NULL;
        //3. Retornar o ponteiro para a raiz (relativa) da nova árvore
        return nova;
    }
    //caso indutivo
    else {
        if(valor->chave > raiz->dado->chave) {
            raiz->dir = inserir_bst(raiz->dir, valor);
        } else {
            raiz->esq = inserir_bst(raiz->esq, valor);
        }
        return raiz;
    }
}

arvore_bst remover_bst (arvore_bst raiz, int chave) {
    if (raiz == NULL)
        return raiz;

    if (chave > raiz->dado->chave)
        raiz->dir = remover_bst(raiz->dir, chave);
    else if (chave < raiz->dado->chave)
        raiz->esq = remover_bst(raiz->esq, chave);
    else {
        // 1º caso: elemento não tem filho
        if (raiz->esq == NULL && raiz->dir == NULL){
            free(raiz);
            return NULL;
        }
        // 2º caso: elemento possui 1 filho
        if(raiz->esq != NULL && raiz->dir == NULL) {
            arvore_bst aux = raiz->esq;
            free(raiz);
            return aux;
        }
        // 2º caso: elemento possui 1 filho (SIMÉTRICO)
        if(raiz->esq == NULL && raiz->dir != NULL) {
            arvore_bst aux = raiz->dir;
            free(raiz);
            return aux;
        }
        // 3º caso: elemento possui 2 filhos
        if (raiz->esq != NULL && raiz->dir != NULL) {
            arvore_bst aux = raiz->esq;
            // encontra o maior elemento da esquerda do valor
            while(aux && aux->dir != NULL){
                aux = aux->dir;
            }
            // troca o valor da raiz com o aux
            raiz->dado = aux->dado;
            aux->dado->chave = chave;
            raiz->esq = remover_bst(raiz->esq, chave);
        }
    }

    return raiz;
}

void in_order_bst(arvore_bst raiz, tabela *tab) {
	if(raiz != NULL) {
		in_order_bst(raiz->esq, tab);
		imprimir_elemento_bst(raiz, tab);
		in_order_bst(raiz->dir, tab);
	}
}

void salvar_auxiliar_bst(arvore_bst raiz, FILE *arq) {
    if(raiz != NULL) {
        fwrite(raiz->dado, sizeof(tipo_dado), 1, arq);
        salvar_auxiliar_bst(raiz->esq, arq);
        salvar_auxiliar_bst(raiz->dir, arq);
    }
}

void imprimir_elemento_bst(arvore_bst raiz, tabela * tab) {
	poke_info * temp = (poke_info *) malloc (sizeof(poke_info));
    temp->poke_number = 1000;
    printf("Indice: %d\n", raiz->dado->indice);
    fseek(tab->arquivo_dados, raiz->dado->indice, SEEK_SET);

    int r = fread(temp, sizeof(poke_info), 1, tab->arquivo_dados);
	if(r != EOF)
        printf("[%d, %s, %d, %s, %s]\n", r, 
                                    temp->poke_name, 
                                    temp->poke_total_status, 
                                    temp->poke_type1, 
                                    temp->poke_type2);
    else
        printf("Falha ao ler dados");

	free(temp);
}