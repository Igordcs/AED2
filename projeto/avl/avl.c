#include "avl.h"
#include "stdio.h"
#include "stdlib.h"

void inicializar_avl(arvore_avl *raiz) {
    *raiz = NULL;
}

arvore_avl rotacao_simplesesquerda(arvore_avl pivo) {
    arvore_avl novoPai, t2;
    novoPai = pivo->dir;
    t2 = novoPai->esq;

    novoPai->esq = pivo;
    pivo->dir = t2;
    return novoPai;
}

arvore_avl rotacao_simplesdireita(arvore_avl pivo) {
    arvore_avl novoPai, t2;
    novoPai = pivo->esq;
    t2 = novoPai->dir;
    novoPai->dir = pivo;
    pivo->esq = t2;
    return novoPai;
}

arvore_avl rotacao_dupladireita(arvore_avl pivo) {
    pivo->esq = rotacao_simplesesquerda(pivo->esq);
    return rotacao_simplesdireita(pivo);
}

arvore_avl rotacao_duplaesquerda(arvore_avl pivo) {
    pivo->dir = rotacao_simplesdireita(pivo->dir);
    return rotacao_simplesesquerda(pivo);
}

arvore_avl rotacao(arvore_avl pivo) {
    if (pivo->fb > 0) { 
        // arvore_avl desbalanceada pra direita
        if(pivo->dir->fb >= 0) { 
            // rotação simples esquerda
            if (pivo->dir->fb == 0) {
                pivo->fb = 1;
                pivo->dir->fb = -1;
            }else {
                pivo->fb = 0;
                pivo->dir->fb = 0;
            }
            pivo = rotacao_simplesesquerda(pivo);
        } else { 
            //rotação dupla esquerda
            if (pivo->dir->esq->fb == 0) {
                pivo->fb = 0;
                pivo->dir->fb = 0;
            } else if(pivo->dir->esq->fb == -1) {
                pivo->dir->fb = 1;
                pivo->fb = 0;
            } else if (pivo->dir->esq->fb == 1) {
                pivo->fb = -1;
                pivo->dir->fb = 0;
            }
            pivo->dir->esq->fb = 0;
            pivo = rotacao_duplaesquerda(pivo);
        }
    } else {
        // arvore_avl desbalanceada pra esquerda
        if(pivo->esq->fb <= 0) { 
            // rotação simples direita
            if (pivo->esq->fb == 0) {
                pivo->fb = -1;
                pivo->esq->fb = 1;
            }else {
                pivo->fb = 0;
                pivo->esq->fb = 0;
            }
            pivo = rotacao_simplesdireita(pivo);
        } else {
            //rotação dupla direita
            if(pivo->esq->dir->fb == 0) {
                pivo->fb = 0;
                pivo->esq->fb = 0;
            }else if(pivo->esq->dir->fb == 1) {
                pivo->fb = 0;
                pivo->esq->fb = -1;
            }else if(pivo->esq->dir->fb == -1) {
                pivo->fb = 1;
                pivo->esq->fb = 0;
            }
            pivo->esq->dir->fb = 0;
            pivo = rotacao_dupladireita(pivo);
        }
    }
    return pivo;
}

arvore_avl inserir_avl (arvore_avl raiz, tipo_dado *valor, int *cresceu){
    //caso base
    if(raiz == NULL) {
        //1. Alocar espaço em memória
        arvore_avl nova = (arvore_avl) malloc(sizeof(struct no_avl));
        //2. Inicializar o novo nó
        nova->dado = valor;
        nova->esq = NULL;
        nova->dir = NULL;
        nova->fb = 0;
        *cresceu = 1;
        //3. Retornar o ponteiro para a raiz (relativa) da nova árvore
        return nova;
    }
    //caso indutivo
    else {
        if(valor->chave > raiz->dado->chave) {
            raiz->dir = inserir_avl(raiz->dir, valor, cresceu);
            if(*cresceu == 1) {
                switch (raiz->fb)
                {
                    case 0:
                        raiz->fb = 1;
                    break;
                    case 1:
                        *cresceu = 0;
                        return rotacao(raiz);
                    break;
                    case -1:
                        raiz->fb = 0;
                        *cresceu = 0;
                    break;
                    default:
                        break;
                }
            }
        } else {
            raiz->esq = inserir_avl(raiz->esq, valor, cresceu);
            if(*cresceu == 1) {
                switch (raiz->fb)
                {
                    case 0:
                        raiz->fb = -1;
                    break;
                    case 1:
                        raiz->fb = 0;
                        *cresceu = 0;
                    break;
                    case -1:
                        *cresceu = 0;
                        return rotacao(raiz);
                    break;
                    default:
                        break;
                }
            }
        }
        return raiz;
    }
}

arvore_avl remover_avl (arvore_avl raiz, int chave, int *diminuiu) {
    if (raiz == NULL)
        return raiz;

    if (chave > raiz->dado->chave){
        raiz->dir = remover_avl(raiz->dir, chave, diminuiu);
        if(*diminuiu) {
            switch (raiz->fb)
            {
            case 0:
                raiz->fb = -1;
                *diminuiu = 0;
            break;
            case 1:
                raiz->fb = 0;
                *diminuiu = 1;
            break;
            case -1:
                return rotacao(raiz);
            break;
            default:
                break;
            }
        }
    }

    else if (chave < raiz->dado->chave) {
        raiz->esq = remover_avl(raiz->esq, chave, diminuiu);
        if(*diminuiu) {
            switch (raiz->fb)
            {
            case 0:
                raiz->fb = 1;
                *diminuiu = 0;
            break;
            case 1:
                return rotacao(raiz);
            break;
            case -1:
                raiz->fb = 0;
                *diminuiu = 1;
            break;
            default:
                break;
            }
        }
    }
    else {
        // 1º caso: elemento não tem filho
        if (raiz->esq == NULL && raiz->dir == NULL){
            *diminuiu = 1;
            free(raiz);
            return NULL;
        }
        // 2º caso: elemento possui 1 filho
        if(raiz->esq != NULL && raiz->dir == NULL) {
            *diminuiu = 1;
            arvore_avl aux = raiz->esq;
            free(raiz);
            return aux;
        }
        // 2º caso: elemento possui 1 filho (SIMÉTRICO)
        if(raiz->esq == NULL && raiz->dir != NULL) {
            *diminuiu = 1;
            arvore_avl aux = raiz->dir;
            free(raiz);
            return aux;
        }
        // 3º caso: elemento possui 2 filhos
        if (raiz->esq != NULL && raiz->dir != NULL) {
            arvore_avl aux = raiz->esq;
            // encontra o maior elemento da esquerda do valor
            while(aux && aux->dir != NULL){
                aux = aux->dir;
            }
            // troca o valor da raiz com o aux
            raiz->dado = aux->dado;
            aux->dado->chave = chave;
            raiz->esq = remover_avl(raiz->esq, chave, diminuiu);
        }
    }

    return raiz;
}

void reverso(arvore_avl raiz){
    if(raiz != NULL) {
        reverso(raiz->dir);
        printf("[%d]", raiz->dado);
        reverso(raiz->esq);
    }
}

void preorder(arvore_avl raiz){
    //caso indutivo...
    if(raiz != NULL) {
        //pre-order: processa raiz, depois esq e direita
        printf("[%d]", raiz->dado);

        //... chamadas recursivas
        preorder(raiz->esq);
        preorder(raiz->dir);
    }

    //caso base, fim da recursão
    else {
        //imprimir uma árvore vazia é não fazer nada
    }
}


void posorder(arvore_avl raiz) {
    if(raiz != NULL) {
        posorder(raiz->esq);
        posorder(raiz->dir);
        printf("[%d]", raiz->dado);
    }
}

int altura(arvore_avl raiz){
    if (raiz == NULL)
        return -1;

    int esq = altura(raiz->esq);
    int dir = altura(raiz->dir);
    
    if (esq > dir)
        return esq + 1;
    else return dir + 1;
}

void pai(arvore_avl raiz, int chave) {
    if (raiz == NULL){
        printf("-1\n");
        return;
    }

    if(raiz->esq != NULL && raiz->esq->dado->chave == chave){
        printf("%d\n", raiz->dado->chave);
        return;
    }
    else if(raiz->dir != NULL && raiz->dir->dado->chave == chave){
        printf("%d\n", raiz->dado->chave);
        return;
    }

    if (chave > raiz->dado->chave)
        return pai(raiz->dir, chave);
    else
        return pai(raiz->esq, chave);
}

void in_order_avl(arvore_avl raiz, tabela *tab) {
	if(raiz != NULL) {
		in_order_avl(raiz->esq, tab);
		imprimir_elemento_avl(raiz, tab);
		in_order_avl(raiz->dir, tab);
	}
}

void salvar_auxiliar_avl(arvore_avl raiz, FILE *arq) {
    if(raiz != NULL) {
        fwrite(raiz->dado, sizeof(tipo_dado), 1, arq);
        salvar_auxiliar_avl(raiz->esq, arq);
        salvar_auxiliar_avl(raiz->dir, arq);
    }
}

void imprimir_elemento_avl(arvore_avl raiz, tabela * tab) {
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

