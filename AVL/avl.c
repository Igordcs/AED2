#include "avl.h"
#include <stdlib.h>
#include <stdio.h>

arvore rotacao_simplesesquerda(arvore pivo) {
    arvore novoPai, t2;
    novoPai = pivo->dir;
    t2 = novoPai->esq;

    novoPai->esq = pivo;
    pivo->dir = t2;
    return novoPai;
}

arvore rotacao_simplesdireita(arvore pivo) {
    arvore novoPai, t2;
    novoPai = pivo->esq;
    t2 = novoPai->dir;
    novoPai->dir = pivo;
    pivo->esq = t2;
    return novoPai;
}

arvore rotacao_dupladireita(arvore pivo) {
    pivo->esq = rotacao_simplesesquerda(pivo->esq);
    return rotacao_simplesdireita(pivo);
}

arvore rotacao_duplaesquerda(arvore pivo) {
    pivo->dir = rotacao_simplesdireita(pivo->dir);
    return rotacao_simplesesquerda(pivo);
}

arvore rotacao(arvore pivo) {
    if (pivo->fb > 0) { 
        // arvore desbalanceada pra direita
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
                pivo->fb = 0;
                pivo->dir->fb = 1;
            } else if (pivo->dir->esq->fb == 1) {
                pivo->fb = -1;
                pivo->dir->fb = 0;
            }
            pivo->dir->esq->fb = 0;
            pivo = rotacao_duplaesquerda(pivo);
        }
    } else {
        // arvore desbalanceada pra esquerda
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

int qtdpar(arvore raiz) {
    if (raiz == NULL)
        return 0;

    int contador = 0;
    if (raiz->valor % 2 == 0) {
        contador++;       
    }
    contador += qtdpar(raiz->esq);
    contador += qtdpar(raiz->dir);
    return contador;
}

void dobrar_valores(arvore raiz) {
    if (raiz != NULL) {
        raiz->valor *= 2;
        dobrar_valores(raiz->dir);
        dobrar_valores(raiz->esq);
    }
}

arvore inserir (arvore raiz, int valor, int *cresceu){
    //caso base
    if(raiz == NULL) {
        //1. Alocar espaço em memória
        arvore nova = (arvore) malloc(sizeof(struct no));
        //2. Inicializar o novo nó
        nova->valor = valor;
        nova->esq = NULL;
        nova->dir = NULL;
        nova->fb = 0;
        *cresceu = 1;
        //3. Retornar o ponteiro para a raiz (relativa) da nova árvore
        return nova;
    }
    //caso indutivo
    else {
        if(valor > raiz->valor) {
            raiz->dir = inserir(raiz->dir, valor, cresceu);
            if(*cresceu) {
                switch (raiz->fb)
                {
                    case 0:
                        raiz->fb = 1;
                        *cresceu = 1;
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
            raiz->esq = inserir(raiz->esq, valor, cresceu);
            if(*cresceu) {
                switch (raiz->fb)
                {
                    case 0:
                        raiz->fb = -1;
                        *cresceu = 1;
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

arvore remover (arvore raiz, int valor, int *diminuiu) {
    printf("status: [%d:%d] \n", raiz->valor, raiz->fb);
    if (raiz == NULL) {
        *diminuiu = 0;
        return raiz;
    }

    if (valor == raiz->valor) {
        *diminuiu = 1;
        // 1º caso: elemento não tem filho
        if (raiz->esq == NULL && raiz->dir == NULL){
            free(raiz);
            return NULL;
        }
        // 2º caso: elemento possui 1 filho
        if(raiz->esq != NULL && raiz->dir == NULL) {
            arvore aux = raiz->esq;
            free(raiz);
            return aux;
        }
        // 2º caso: elemento possui 1 filho (SIMÉTRICO)
        if(raiz->esq == NULL && raiz->dir != NULL) {
            arvore aux = raiz->dir;
            free(raiz);
            return aux;
        }
        // 3º caso: elemento possui 2 filhos
        if (raiz->esq != NULL && raiz->dir != NULL) {
            arvore aux = raiz->esq;
            // encontra o maior elemento da esquerda do valor
            while(aux && aux->dir != NULL){
                aux = aux->dir;
            }
            // troca o valor da raiz com o aux
            raiz->valor = aux->valor;
            aux->valor = valor;
            raiz->esq = remover(raiz->esq, valor, diminuiu);
            if(*diminuiu) {
                if(raiz->fb == -1) {
                    raiz->fb = 0;
                    *diminuiu = 0;
                } else if (raiz->fb == 0) {
                    raiz->fb = 1;
                    *diminuiu = 0;
                } 
            }
        }
    } else {
        if (valor < raiz->valor) {
            raiz->esq = remover(raiz->esq, valor, diminuiu);
            if(*diminuiu) {
                switch (raiz->fb)
                {
                case 0:
                    raiz->fb = 1;
                    *diminuiu = 0;
                break;
                case 1:
                    if(raiz->dir->fb == 0) {
                        *diminuiu = 0;
                    } else *diminuiu = 1;
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
        if (valor > raiz->valor){
            raiz->dir = remover(raiz->dir, valor, diminuiu);
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
                    if(raiz->esq->fb == 0) {
                        *diminuiu = 0;
                    }else *diminuiu = 1;
                    return rotacao(raiz);
                break;
                default:
                    break;
                }
            }
        }
    }
    return raiz;
}
void reverso(arvore raiz){
    if(raiz != NULL) {
        reverso(raiz->dir);
        printf("[%d]", raiz->valor);
        reverso(raiz->esq);
    }
}

void preorder(arvore raiz){
    //caso indutivo...
    if(raiz != NULL) {
        //pre-order: processa raiz, depois esq e direita
        printf("[%d], fb: %d; ", raiz->valor, raiz->fb);

        //... chamadas recursivas
        preorder(raiz->esq);
        preorder(raiz->dir);
    }

    //caso base, fim da recursão
    else {
        //imprimir uma árvore vazia é não fazer nada
    }
}

void inorder(arvore raiz){
    if(raiz != NULL) {
        inorder(raiz->esq);
        printf("[%d]", raiz->valor);
        inorder(raiz->dir);
    }
}

void descendentes(arvore raiz, int valor) {
    if(raiz == NULL)
        return;

    if (valor > raiz->valor) 
        return descendentes(raiz->dir, valor);
    else if(valor < raiz->valor) 
        return descendentes(raiz->esq, valor);
    else {
        inorder(raiz->esq);
        inorder(raiz->dir);
    }
}

void antecessor(arvore raiz, int valor) {
    if(raiz == NULL) {
        printf("-1\n");
        return;
    }

    if (busca(raiz, valor) == 0){
        printf("-1\n");
        return;
    }

    arvore temp, candidato;
    temp = raiz;
    candidato = NULL;

    if (temp->valor == valor) {
        temp = temp->esq;
    }

    while(temp && temp->valor != valor) {
        if(valor > temp->valor){
            candidato = temp;
            temp = temp->dir;
        }else {
            temp = temp->esq;
        }
    }

    if(temp->valor == valor || temp == NULL){
        printf("-1\n");
        return;
    }
    
    printf("%d\n", candidato->valor);
}

arvore podar(arvore raiz, int valor) {
    if (raiz == NULL)
        return NULL;

    if (valor > raiz->valor)
        raiz->dir = podar(raiz->dir, valor);
    else if(valor < raiz->valor)
        raiz->esq = podar(raiz->esq, valor);
    else {
        podar(raiz->esq, valor);
        podar(raiz->dir, valor);
        free(raiz);
        return NULL;
    }
    return raiz;
}


void posorder(arvore raiz) {
    if(raiz != NULL) {
        posorder(raiz->esq);
        posorder(raiz->dir);
        printf("[%d]", raiz->valor);
    }
}

/*int somatorio(arvore raiz){
    int soma = 0;

    if(raiz != NULL) {
        soma += raiz->valor +
            somatorio(raiz->esq) +
            somatorio(raiz->dir);
    }
    return soma;
}*/

int somatorio(arvore raiz){
    return (raiz==NULL) ? 0 : raiz->valor + somatorio(raiz->esq) + somatorio(raiz->dir);
}

int altura(arvore raiz){
    if (raiz == NULL)
        return -1;

    int esq = altura(raiz->esq);
    int dir = altura(raiz->dir);
    
    if (esq > dir)
        return esq + 1;
    else return dir + 1;
}

int somaPar(arvore raiz) {
    if (raiz == NULL)
        return 0;

    int somador = 0;
    if (raiz->valor % 2 == 0) {
        somador += raiz->valor;                         
    }
    somador += somaPar(raiz->esq);
    somador += somaPar(raiz->dir);
    return somador;
}

int busca(arvore raiz, int valor) {
    if (raiz == NULL)
        return 0;
    
    if(raiz->valor == valor)
        return 1;
    
    if (valor > raiz->valor)
        return busca(raiz->dir, valor);
    else
        return busca(raiz->esq, valor);
}

void pai(arvore raiz, int valor) {
    if (raiz == NULL){
        printf("-1\n");
        return;
    }

    if(raiz->esq != NULL && raiz->esq->valor == valor){
        printf("%d\n", raiz->valor);
        return;
    }
    else if(raiz->dir != NULL && raiz->dir->valor == valor){
        printf("%d\n", raiz->valor);
        return;
    }

    if (valor > raiz->valor)
        return pai(raiz->dir, valor);
    else
        return pai(raiz->esq, valor);
}