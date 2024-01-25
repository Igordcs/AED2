#include "arb.h"
#include <stdlib.h>
#include <stdio.h>

arvore no_null;

void inicializar(arvore *raiz) {
	*raiz = NULL;
	no_null = (arvore) malloc(sizeof(struct no_bst));
	no_null->cor = DUPLO_PRETO;
	no_null->valor = 0;
    no_null->esq = NULL;
    no_null->dir = NULL;
    no_null->pai = NULL;
}

enum cor cor(arvore elemento) {
    enum cor c;
    if(elemento == NULL)
        return PRETO;
    else return elemento->cor;
}

int eh_filho_esquerdo(arvore elemento) {
    return (elemento->pai != NULL && elemento == elemento->pai->esq);
}

int eh_raiz(arvore elemento) {
    return elemento->pai == NULL;
}

arvore irmao(arvore elemento) {
    if (eh_filho_esquerdo(elemento))
        return elemento->pai->dir;
    else
        return elemento->pai->esq;
}

arvore tio(arvore elemento) {
    return irmao(elemento->pai);
}

// Utiliza-se *raiz pois é um ponteiro para ponteiro, ou seja, manipulação por referência
void ajustar(arvore *raiz, arvore elemento) {
    // Árvore desbalanceada pois o pai é rubro e o elemento adiiconado também
    while(cor(elemento->pai) == VERMELHO && cor(elemento) == VERMELHO) {
        // primeiro caso: O tio também é vermelho
        if (cor(tio(elemento)) == VERMELHO) {
            // pai e tio --> PRETO e avô rubro
            tio(elemento)->cor = PRETO;
            elemento->pai->cor = PRETO;
            elemento->pai->pai->cor = VERMELHO;

            // Essa mudança pode ocasionar em uma sequência rubro-rubro no avô
            // continua a verificação a partir do avô
            elemento = elemento->pai->pai;
            continue;
        }
        // Caso 2: rotação simples direita
        if(eh_filho_esquerdo(elemento) && eh_filho_esquerdo(elemento->pai)) {
            rotacao_simples_direita(raiz, elemento->pai->pai);
            elemento->pai->cor = PRETO;
            elemento->pai->dir->cor = VERMELHO;
            continue;
        }
        // caso 2 Simétrico: Rotação simples Esquerda 
        if(!eh_filho_esquerdo(elemento) && !eh_filho_esquerdo(elemento->pai)) {
            rotacao_simples_esquerda(raiz, elemento->pai->pai);
            elemento->pai->cor = PRETO;
            elemento->pai->esq->cor = VERMELHO;
            continue;
        }
        // caso 3: Rotação dupla Direita
        if(!eh_filho_esquerdo(elemento) && eh_filho_esquerdo(elemento->pai)) {
            rotacao_simples_esquerda(raiz, elemento->pai);
            // Reduz ao caso 2
            elemento = elemento->esq;
            continue;
        }
        if(eh_filho_esquerdo(elemento) && !eh_filho_esquerdo(elemento->pai)) {
            rotacao_simples_direita(raiz, elemento->pai);
            // Reduz ao caso 2 simétrico
            elemento = elemento->dir;
            continue;
        }
    }
    (*raiz)->cor = PRETO;
}

void rotacao_simples_direita(arvore *raiz, arvore pivo) {
    arvore u, t1;
    u = pivo->esq;
    t1 = u->dir;

    // para atualizar a referência da raiz à sub-arvore resultante, é necessário
    // sabe se o pivo era filho esquerdo ou direito.
    int posicao_pivo = eh_filho_esquerdo(pivo);

    // atualização de ponteiros
    pivo->esq = t1;
    if (t1 != NULL)
        t1->pai = pivo;
    u->dir = pivo;
    u->pai = pivo->pai;
    pivo->pai = u;

    // se não existe árvore acima de u, então u passa a ser a raiz da árvore
    if(eh_raiz(u)) 
        *raiz = u;
    else // Se existe, então precisamos da posição que o pivô ocupava em relação ao seu pai
        if (posicao_pivo)
            u->pai->esq = u;
        else
            u->pai->dir = u;
} 

void rotacao_simples_esquerda(arvore *raiz, arvore pivo) {
    arvore u, t1;
    u = pivo->dir;
    t1 = u->esq;

    // para atualizar a referência da raiz à sub-arvore resultante, é necessário
    // sabe se o pivo era filho esquerdo ou direito.
    int posicao_pivo = eh_filho_esquerdo(pivo);

    // atualização de ponteiros
    pivo->dir = t1;
    if (t1 != NULL)
        t1->pai = pivo;
    u->esq = pivo;
    u->pai = pivo->pai;
    pivo->pai = u;

    // se não existe árvore acima de u, então u passa a ser a raiz da árvore
    if(eh_raiz(u)) 
        *raiz = u;
    else // Se existe, então precisamos da posição que o pivô ocupava em relação ao seu pai
        if (posicao_pivo)
            u->pai->esq = u;
        else
            u->pai->dir = u;
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

// o parametro raiz é um ponteiro para ponteiro
void adicionar (int valor, arvore *raiz){
    arvore posicao, pai, novo;
    posicao = *raiz;
    pai = NULL;
    
    // achar a posição certa para adicionar o novo elemento
    while (posicao != NULL)
    {
        pai = posicao;
        if(valor > posicao->valor) 
            posicao = posicao->dir;
        else
            posicao = posicao->esq;
    }

    // alocar o novo elemento
    novo = (arvore) malloc(sizeof(struct no_bst));
    novo->valor = valor;
    novo->cor = VERMELHO;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = pai;

    // Se o elemento for a raiz, então o ponteiro da raiz aponta para o elemento
    if(eh_raiz(novo)){
        novo->cor = PRETO;
        *raiz = novo;
    } else {
        // se o novo elemento não for a raiz, é necessário fazer a ligação dele com seu pai
        if (valor > pai->valor)
            pai->dir = novo;
        else
            pai->esq = novo;
    }

    // após inserir, ajustamos desbalanceamentos na árvore
    ajustar(raiz, novo);
}

arvore remover (arvore raiz, int valor) {
    if (raiz == NULL)
        return raiz;

    if (valor > raiz->valor)
        raiz->dir = remover(raiz->dir, valor);
    else if (valor < raiz->valor)
        raiz->esq = remover(raiz->esq, valor);
    else {
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
            raiz->esq = remover(raiz->esq, valor);
        }
    }

    return raiz;
}
void reverso(arvore raiz){
    if(raiz != NULL) {
        reverso(raiz->dir);
        imprimir_elemento(raiz);
        reverso(raiz->esq);
    }
}

void preorder(arvore raiz){
    //caso indutivo...
    if(raiz != NULL) {
        //pre-order: processa raiz, depois esq e direita
        imprimir_elemento(raiz);

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
        imprimir_elemento(raiz);
        inorder(raiz->dir);
    }
}

void imprimir_elemento(arvore raiz) {
	switch(raiz->cor){
		case PRETO:
			printf("\x1b[30m[%d]\x1b[0m", raiz->valor);
			break;
		case VERMELHO:
			printf("\x1b[31m[%d]\x1b[0m", raiz->valor);
			break;
		case DUPLO_PRETO:
			printf("\x1b[32m[%d]\x1b[0m", raiz->valor);
			break;
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

