/*
    To-do
    d) Os dados dos registros devem ser armazenados em um arquivo binário sequencial, conforme exemplificado no projeto "arquivo 3"; // FEITO
    e) Cada índice deve ser salvo em um arquivo próprio, conforme exemplificado no projeto "arquivo 1". OBS: durante a execução do programa, os índices devem ser mantidos em memória RAM, sendo passados para o arquivo apenas quando o usuário escolher a opção "sair" do programa;
    f) Implemente a função de adicionar registro, a qual insere o registro no arquivo de dados e atualiza os três índices; 
    g) Implemente a função de remover registro, que remove apenas as referências ao registro nos índices, sem a necessidade de modificar o arquivo de dados;
    h) Implemente uma função de busca para cada índice. Esta função deve imprimir na tela todos os dados satélite do registro referenciado pelo índice passado como parâmetro;
    i) Implemente uma função para exibir os registros ordenados de acordo com cada um dos índices (inorder);
    j) Implemente a função main que permite ao usuário utilizar o sistema com todas as opções de funções implementadas;
*/
#include "sgbd.h"
#include "string.h"
#include <stdlib.h>
#include "avl/avl.h"
#include "arb/arb.h"

// --------------- BST -----------------------
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

/*
    i) Implemente uma função para exibir os registros ordenados de acordo com cada 
    um dos índices (inorder);
*/

void in_order_bst(arvore_bst raiz, tabela *tab) {
	if(raiz != NULL) {
		in_order_bst(raiz->esq, tab);
		imprimir_elemento_bst(raiz, tab);
		in_order_bst(raiz->dir, tab);
	}
}

/*
    e) Cada índice deve ser salvo em um arquivo próprio, conforme exemplificado no projeto "arquivo 1". 
    OBS: durante a execução do programa, os índices devem ser mantidos em memória RAM, sendo passados 
    para o arquivo apenas quando o usuário escolher a opção "sair" do programa;
*/

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
    
    // Cria um buffer de tamanho 1024 pra ler os registros
    char *dado = (char *) malloc(sizeof(char) * 1024);
    if (fgets(dado, 1024, tab->arquivo_dados) != NULL) {
        // força a liberar espaço na string
        dado = strdup(dado);
        tirar_enter(dado); // fgets lê com o \n, então é necessário retirar
        split_string(dado, temp); // separa os campos pela ',' e preenche por referencia temp
        printf("[%d, %s, %s, %s, %d]\n",temp->poke_number,
                                        temp->poke_name,  
                                        temp->poke_type1, 
                                        temp->poke_type2,
                                        temp->poke_total_status);
        free(dado);
    }else printf("Falha ao ler dados");
	free(temp);
}

tipo_dado* indice_sucessor(arvore_bst raiz, int tamanho) {
    arvore_bst atual = raiz;
    arvore_bst sucessor = NULL;

    while (atual != NULL) {
        if (atual->dado->chave > tamanho) {
            sucessor = atual;
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    if (sucessor == NULL) {
        return NULL;  // Não há sucessor
    } else {
        return sucessor->dado;
    }
}
// ---------------- BST -----------------


// ---------------- AVL -----------------

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

arvore_avl inserir_avl (arvore_avl raiz, tipo_dado_avl *valor, int *cresceu){
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
        if(strcmp(valor->poke_name, raiz->dado->poke_name) > 0) {
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

arvore_avl remover_avl (arvore_avl raiz, char *valor, int *diminuiu) {
    if (raiz == NULL) {
        *diminuiu = 0;
        return raiz;
    }

    if (strcmp(valor, raiz->dado->poke_name) == 0) {
        *diminuiu = 1;
        // 1º caso: elemento não tem filho
        if (raiz->esq == NULL && raiz->dir == NULL){
            free(raiz);
            return NULL;
        }
        // 2º caso: elemento possui 1 filho
        if(raiz->esq != NULL && raiz->dir == NULL) {
            arvore_avl aux = raiz->esq;
            free(raiz);
            return aux;
        }
        // 2º caso: elemento possui 1 filho (SIMÉTRICO)
        if(raiz->esq == NULL && raiz->dir != NULL) {
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
            strcpy(aux->dado->poke_name, valor);
            // raiz->dado->poke_name = aux->dado->poke_name;
            // aux->dado->poke_name = valor;
            raiz->esq = remover_avl(raiz->esq, valor, diminuiu);
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
        if (strcmp(valor, raiz->dado->poke_name) < 0) {
            raiz->esq = remover_avl(raiz->esq, valor, diminuiu);
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
        if (strcmp(valor, raiz->dado->poke_name) > 0){
            raiz->dir = remover_avl(raiz->dir, valor, diminuiu);
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

/*
    i) Implemente uma função para exibir os registros ordenados de acordo com cada 
    um dos índices (inorder);

    a chave é o nome, então ordem crescente do nome
*/

void in_order_avl(arvore_avl raiz, tabela *tab) {
	if(raiz != NULL) {
		in_order_avl(raiz->esq, tab);
		imprimir_elemento_avl(raiz, tab);
		in_order_avl(raiz->dir, tab);
	}
}

/*
    e) Cada índice deve ser salvo em um arquivo próprio, conforme exemplificado no projeto "arquivo 1". 
    OBS: durante a execução do programa, os índices devem ser mantidos em memória RAM, sendo passados 
    para o arquivo apenas quando o usuário escolher a opção "sair" do programa;
*/

void salvar_auxiliar_avl(arvore_avl raiz, FILE *arq) {
    if(raiz != NULL) {
        fwrite(raiz->dado, sizeof(tipo_dado_avl), 1, arq);
        salvar_auxiliar_avl(raiz->esq, arq);
        salvar_auxiliar_avl(raiz->dir, arq);
    }
}

void imprimir_elemento_avl(arvore_avl raiz, tabela * tab) {
	poke_info * temp = (poke_info *) malloc (sizeof(poke_info));
    temp->poke_number = 1000;
    printf("Indice: %d\n", raiz->dado->indice);
    fseek(tab->arquivo_dados, raiz->dado->indice, SEEK_SET);

    // Cria um buffer de tamanho 1024 pra ler os registros
    char *dado = (char *) malloc(sizeof(char) * 1024);
    if (fgets(dado, 1024, tab->arquivo_dados) != NULL) {
        // força a liberar espaço na string
        dado = strdup(dado);
        tirar_enter(dado); // fgets lê com o \n, então é necessário retirar
        split_string(dado, temp); // separa os campos pela ',' e preenche por referencia temp
        printf("[%d, %s, %s, %s, %d]\n",temp->poke_number,
                                        temp->poke_name,  
                                        temp->poke_type1, 
                                        temp->poke_type2,
                                        temp->poke_total_status);
        free(dado);
    }else printf("Falha ao ler dados");

	free(temp);
}

// ---------------- AVL -----------------

// ---------------- ARB -----------------
arvore_rb no_null;

void inicializar_rb(arvore_rb *raiz) {
	*raiz = NULL;
	no_null = (arvore_rb) malloc(sizeof(struct no_rb));
	no_null->cor = DUPLO_PRETO;
	no_null->dado = NULL;
    no_null->esq = NULL;
    no_null->dir = NULL;
    no_null->pai = NULL;
}

void imprimir_elemento_rb(arvore_rb raiz, tabela * tab) {
	poke_info * temp = (poke_info *) malloc (sizeof(poke_info));
    temp->poke_number = 1000;
    printf("Indice: %d\n", raiz->dado->indice);
    fseek(tab->arquivo_dados, raiz->dado->indice, SEEK_SET);

    // Cria um buffer de tamanho 1024 pra ler os registros
    char *dado = (char *) malloc(sizeof(char) * 1024);
    if (fgets(dado, 1024, tab->arquivo_dados) != NULL) {
        // força a liberar espaço na string
        dado = strdup(dado);
        tirar_enter(dado); // fgets lê com o \n, então é necessário retirar
        split_string(dado, temp); // separa os campos pela ',' e preenche por referencia temp
        switch(raiz->cor){
            case PRETO:
                printf("\x1b[30m[%d, %s, %s, %s, %d]\x1b[0m\n",
                                                temp->poke_number,
                                                temp->poke_name, 
                                                temp->poke_type1,
                                                temp->poke_type2,
                                                temp->poke_total_status);
                break;
            case VERMELHO:
                printf("\x1b[31m[%d, %s, %s, %s, %d]\x1b[0m\n",
                                                temp->poke_number,
                                                temp->poke_name, 
                                                temp->poke_type1,
                                                temp->poke_type2,
                                                temp->poke_total_status);
                break;
            case DUPLO_PRETO:
                printf("\x1b[32m[%d, %s, %s, %s, %d]\x1b[0m\n", temp->poke_number,
                                                temp->poke_name, 
                                                temp->poke_type1,
                                                temp->poke_type2,
                                                temp->poke_total_status);
                break;
        }
        free(dado);
    }else printf("Falha ao ler dados");

	free(temp);
}

cores cor(arvore_rb elemento) {
    cores c;
    if(elemento == NULL)
        return PRETO;
    else return elemento->cor;
}

int eh_filho_esquerdo(arvore_rb elemento) {
    return (elemento->pai != NULL && elemento == elemento->pai->esq);
}

int eh_raiz(arvore_rb elemento) {
    return elemento->pai == NULL;
}

arvore_rb irmao(arvore_rb elemento) {
    if (eh_filho_esquerdo(elemento))
        return elemento->pai->dir;
    else
        return elemento->pai->esq;
}

arvore_rb tio(arvore_rb elemento) {
    return irmao(elemento->pai);
}

// Utiliza-se *raiz pois é um ponteiro para ponteiro, ou seja, manipulação por referência
void ajustar(arvore_rb *raiz, arvore_rb elemento) {
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

void rotacao_simples_direita(arvore_rb *raiz, arvore_rb pivo) {
    arvore_rb u, t1;
    u = pivo->esq;
    t1 = u->dir;

    // para atualizar a referência da raiz à sub-arvore_rb resultante, é necessário
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

void rotacao_simples_esquerda(arvore_rb *raiz, arvore_rb pivo) {
    arvore_rb u, t1;
    u = pivo->dir;
    t1 = u->esq;

    // para atualizar a referência da raiz à sub-arvore_rb resultante, é necessário
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

// o parametro raiz é um ponteiro para ponteiro
void adicionar_rb (tipo_dado_rb *valor, arvore_rb *raiz){
    arvore_rb posicao, pai, novo;
    posicao = *raiz;
    pai = NULL;
    
    // achar a posição certa para adicionar o novo elemento
    while (posicao != NULL)
    {
        pai = posicao;
        if(valor->poke_total_status > posicao->dado->poke_total_status) 
            posicao = posicao->dir;
        else
            posicao = posicao->esq;
    }

    // alocar o novo elemento
    novo = (arvore_rb) malloc(sizeof(struct no_rb));
    novo->dado = valor;
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
        if (valor->poke_total_status > pai->dado->poke_total_status)
            pai->dir = novo;
        else
            pai->esq = novo;
    }

    // após inserir, ajustamos desbalanceamentos na árvore
    ajustar(raiz, novo);
}

tipo_dado_rb* maior_elemento(arvore_rb raiz) {
    if(raiz == NULL)
        return NULL;
    if(raiz->dir == NULL)
        return raiz->dado;
    else
        return maior_elemento(raiz->dir);
}

void remover_rb (int chave, arvore_rb *raiz) {
    arvore_rb posicao;
    posicao = *raiz;

    while (posicao != NULL)
    {
        if (chave == posicao->dado->poke_total_status) {
            // ELEMENTO POSSUI DOIS FILHOS, LOGO, SUBSTITUI O VALOR PELO SUCESSOR E APAGA O SUCESSOR.
            if(posicao->esq != NULL && posicao->dir != NULL) {
                posicao->dado = maior_elemento(posicao->esq);
                chave = posicao->dado->poke_total_status;
            }
        
            // 1º caso: elemento não tem filho
            if (posicao->esq == NULL && posicao->dir == NULL){
                // só existe o nó raiz
                if(eh_raiz(posicao)){
                    *raiz = NULL;
                    free(posicao);
                    break;
                }
                // Se o elemento nao tem filhos e é vermelho, basta removê-lo
                if(posicao->cor == VERMELHO) {
                    if(eh_filho_esquerdo(posicao))
                        posicao->pai->esq = NULL;
                    else
                        posicao->pai->dir = NULL;
                    free(posicao);
                    break;
                } else {
                    // se o elemento é preto, substitui pelo duplo_preto e reajusta a árvore
                    no_null->cor = DUPLO_PRETO;
                    no_null->pai = posicao->pai;
                    if(eh_filho_esquerdo(posicao))
                        posicao->pai->esq = no_null;
                    else
                        posicao->pai->dir = no_null;
                    free(posicao);
                    reajustar(raiz, no_null);
                    break;
                }
            }
            // 2º caso: elemento possui 1 filho (ESQUERDO)
            if(posicao->esq != NULL && posicao->dir == NULL) {
                posicao->esq->pai = posicao->pai;
                posicao->esq->cor = PRETO;
                if(eh_raiz(posicao)) 
                    *raiz = posicao->esq;
                else {
                    if(eh_filho_esquerdo(posicao))
                        posicao->pai->esq = posicao->esq;
                    else
                        posicao->pai->dir = posicao->esq;
                }
                free(posicao);
                break;
            }
            // 2º caso: elemento possui 1 filho (DIREITO)
            if(posicao->esq == NULL && posicao->dir != NULL) {
                posicao->dir->pai = posicao->pai;
                posicao->dir->cor = PRETO;
                if(eh_raiz(posicao)) 
                    *raiz = posicao->dir;
                else {
                    if(eh_filho_esquerdo(posicao))
                        posicao->pai->esq = posicao->dir;
                    else
                        posicao->pai->dir = posicao->dir;
                }
                free(posicao);
                break;
            }
        }
        if(chave > posicao->dado->poke_total_status)
            posicao = posicao->dir;
        else
            posicao = posicao->esq;
    }
}

void reajustar(arvore_rb *raiz, arvore_rb elemento) {
    // Elemento é raiz, não pode ser da cor vermelha
    if (eh_raiz(elemento)) {
        elemento->cor = PRETO;
        // a raiz só possui o no_null;
        if(elemento == no_null)
            *raiz = NULL;
        return;
    }
    
    if (cor(elemento->pai) == PRETO &&
        cor(irmao(elemento)) == VERMELHO &&
        cor(irmao(elemento)->dir) == PRETO &&
        cor(irmao(elemento)->esq) == PRETO ) {
        if(eh_filho_esquerdo(elemento))
            rotacao_simples_esquerda(raiz, elemento->pai);
        else
            rotacao_simples_direita(raiz, elemento->pai);
        elemento->pai->pai->cor = PRETO;
        elemento->pai->cor = VERMELHO;

        // o Caso 2 não corrige o duplo_preto
        reajustar(raiz, elemento);
        return;
    }

    // caso 3: pai e irmão é preto e seus filhos são preto
    // caso 3 não resolve o duplo preto
    if(cor(elemento->pai) == PRETO && 
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->esq) == PRETO &&
        cor(irmao(elemento)->dir) == PRETO) {
        irmao(elemento)->cor = VERMELHO;
        elemento->pai->cor = DUPLO_PRETO;
        retira_duplo_preto(raiz, elemento);
        // pai vira duplo_preto e propaga essa mudança
        reajustar(raiz, elemento->pai);
        return;
    }

    // caso 4: pai vermelho, irmão e filhos pretos
    // resolve duplo preto
    if(cor(elemento->pai) == VERMELHO && 
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->esq) == PRETO &&
        cor(irmao(elemento)->dir) == PRETO) {
        elemento->pai->cor = PRETO;
        irmao(elemento)->cor = VERMELHO;
        retira_duplo_preto(raiz, elemento);
        return;
    }

    // Caso 5a: elemento eh filho esquerdo
    //  PAI -> PRETO, IRMÃO -> PRETO, IRMÃO->DIR == PRETO E ESQ == VERMELHO
    // não resolve duplo_preto
    if(eh_filho_esquerdo(elemento) && 
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->dir) == PRETO &&
        cor(irmao(elemento)->esq) == VERMELHO) {
        irmao(elemento)->cor = VERMELHO;
        irmao(elemento)->esq->cor = PRETO;
        rotacao_simples_direita(raiz, irmao(elemento));
        reajustar(raiz, elemento);
        return;
    }

    // Caso 5b: elemento não eh filho esquerdo
    //  PAI -> PRETO, IRMÃO -> PRETO, IRMÃO->DIR == PRETO E ESQ == VERMELHO
    // não resolve duplo_preto
    if(!eh_filho_esquerdo(elemento) && 
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->dir) == VERMELHO &&
        cor(irmao(elemento)->esq) == PRETO) {
        irmao(elemento)->cor = VERMELHO;
        irmao(elemento)->dir->cor = PRETO;
        rotacao_simples_esquerda(raiz, irmao(elemento));
        reajustar(raiz, elemento);
        return;
    }

    // Caso 6a: PAI TANTO FAZ, IRMÃO: PRETO, filho dir vermelho
    // RESOLVE DUPLO_PRETO
    // irmão pega cor do pai, pai fica preto, FILHO vermelho fica preto
    if (eh_filho_esquerdo(elemento) &&
        cor(irmao(elemento)) == PRETO && 
        cor(irmao(elemento)->dir) == VERMELHO ) {
        irmao(elemento)->cor = elemento->pai->cor;
        elemento->pai->cor = PRETO;
        irmao(elemento)->dir->cor = PRETO;
        rotacao_simples_esquerda(raiz, elemento->pai);
        retira_duplo_preto(raiz, elemento);
        return;
    }

    // Caso 6b: PAI TANTO FAZ, IRMÃO: PRETO, filho dir vermelho
    // RESOLVE DUPLO_PRETO
    // irmão pega cor do pai, pai fica preto, FILHO vermelho fica preto
    if (!eh_filho_esquerdo(elemento) &&
        cor(irmao(elemento)) == PRETO && 
        cor(irmao(elemento)->esq) == VERMELHO) {
        irmao(elemento)->cor = elemento->pai->cor;
        elemento->pai->cor = PRETO;
        irmao(elemento)->esq->cor = PRETO;
        rotacao_simples_direita(raiz, elemento->pai);
        retira_duplo_preto(raiz, elemento);
        return;
    }
}

void retira_duplo_preto(arvore_rb *raiz, arvore_rb elemento) {
    if(elemento == no_null)
        if(eh_filho_esquerdo(elemento))
            elemento->pai->esq = NULL;
        else
            elemento->pai->dir = NULL;
    else
        elemento->cor = PRETO;
}

/*
    e) Cada índice deve ser salvo em um arquivo próprio, conforme exemplificado no projeto "arquivo 1". 
    OBS: durante a execução do programa, os índices devem ser mantidos em memória RAM, sendo passados 
    para o arquivo apenas quando o usuário escolher a opção "sair" do programa;
*/

void salvar_auxiliar_rb(arvore_rb raiz, FILE *arq) {
    if(raiz != NULL) {
        fwrite(raiz->dado, sizeof(tipo_dado_rb), 1, arq);
        salvar_auxiliar_rb(raiz->esq, arq);
        salvar_auxiliar_rb(raiz->dir, arq);
    }
}

/*
    i) Implemente uma função para exibir os registros ordenados de acordo com cada 
    um dos índices (inorder);
*/

void in_order_rb(arvore_rb raiz, tabela *tab) {
	if(raiz != NULL) {
		in_order_rb(raiz->esq, tab);
		imprimir_elemento_rb(raiz, tab);
		in_order_rb(raiz->dir, tab);
	}
}

// ---------------- ARB -----------------

/*
    Retorna o tipo_dado, da posição livre pois a estratégia para ter menos linhas de código foi:
    utilizar a mesma estrutura da bst, onde o dado->indice é onde começa o registro deletado e 
    dado->chave é a quantidade de caracter do registro deletado.

    é necessário fazer essas verificações pois se um elemento maior fosse adicionado ele "comeria"
    boa parte do próximo registro.
*/

tab_realoc retab;

tipo_dado* busca_posicao_livre(int tamanho_elemento) {
    if (retab.indice_bst == NULL)
        return NULL;
    if(tamanho_elemento > retab.indice_bst->dado->chave) {
        // encontra o sucessor do tamanho_elemento
        tipo_dado* sucessor = indice_sucessor(retab.indice_bst, tamanho_elemento);
        if(sucessor == NULL)
            return NULL;
        
        return sucessor;
    }
    return retab.indice_bst->dado;
}

int tamanho (poke_info *pokemon) {
    char *buffer = (char *) malloc(sizeof(char) * 1024);
    int qtd_caracter = sprintf(buffer, "%d,%s,%s,%s,%d\n", pokemon->poke_number,
                                                        pokemon->poke_name,
                                                        pokemon->poke_type1,
                                                        pokemon->poke_type2,
                                                        pokemon->poke_total_status);
    free(buffer);
    return qtd_caracter;
}

void inicializar_indices(tabela *tab) {
    inicializar_avl(&tab->indice_avl);
    inicializar_rb(&tab->indice_rb);
    inicializar_bst(&tab->indice_bst);
}

void carregar_arquivos(tabela *tab) {
    tipo_dado *temp;
    tipo_dado_avl *temp_avl;
    tipo_dado_rb *temp_arb;
    FILE *arq;
    arq = fopen("./arquivos/indices_avl.dat", "rb");

    if(retab.arquivo_dados != NULL) {
        temp = (tipo_dado*) malloc(sizeof(tipo_dado));
        while(fread(temp, sizeof(tipo_dado), 1, retab.arquivo_dados)) {
            retab.indice_bst = inserir_bst(retab.indice_bst, temp);
            temp = (tipo_dado*) malloc(sizeof(tipo_dado));
        }
    }

    if(arq != NULL) {
        int cresceu;
        temp_avl = (tipo_dado_avl*) malloc(sizeof(tipo_dado_avl));
        while(fread(temp_avl, sizeof(tipo_dado_avl), 1, arq)) {
            tab->indice_avl = inserir_avl(tab->indice_avl, temp_avl, &cresceu);
            temp_avl = (tipo_dado_avl*) malloc(sizeof(tipo_dado_avl));
        }
        fclose(arq);
    }

    arq = fopen("./arquivos/indices_arb.dat", "rb");
    if(arq != NULL) {
        temp_arb = (tipo_dado_rb*) malloc(sizeof(tipo_dado_rb));
        while(fread(temp_arb, sizeof(tipo_dado_rb), 1, arq)) {
            adicionar_rb(temp_arb, &(tab->indice_rb));
            temp_arb = (tipo_dado_rb*) malloc(sizeof(tipo_dado_rb));
        }
        fclose(arq);
    }

    arq = fopen("./arquivos/indices_bst.dat", "rb");
    if(arq != NULL) {
        temp = (tipo_dado*) malloc(sizeof(tipo_dado));
        while(fread(temp, sizeof(tipo_dado), 1, arq)) {
            tab->indice_bst = inserir_bst(tab->indice_bst, temp);
            temp = (tipo_dado*) malloc(sizeof(tipo_dado));
        }
        fclose(arq);
    }
}

int inicializar_tabela(tabela *tabela) {
    inicializar_indices(tabela);
    inicializar_bst(&retab.indice_bst);
    tabela->arquivo_dados = fopen("./arquivos/dados.dat", "r+b");
    // elementos removidos serão salvos nesse arquivo
    retab.arquivo_dados = fopen("./arquivos/removidos.dat", "a+b"); 
    carregar_arquivos(tabela);

    // Tive que utilizar o modo r+b para ler, então quando não há o arquivo, é necessário criá-lo
    if(tabela->arquivo_dados == NULL) {
        tabela->arquivo_dados = fopen("./arquivos/dados.dat", "wb");
        fclose(tabela->arquivo_dados);
        tabela->arquivo_dados = fopen("./arquivos/dados.dat", "r+b");
    }

    if (tabela->arquivo_dados != NULL)
        return 1;
    else
        return -1;
}

/*
    g) Implemente a função de remover registro, que remove apenas as referências ao registro nos índices,
    sem a necessidade de modificar o arquivo de dados;
*/

void remover_indice(tabela *tab, int chave) {
    poke_info *pokemon = (poke_info *) malloc(sizeof(poke_info));   
    arvore_bst raiz = tab->indice_bst;
    // localizar o registro procurado
    while(raiz != NULL) {
        if(chave > raiz->dado->chave) {
            raiz = raiz->dir;
            continue;
        } 
        else if (chave < raiz->dado->chave) {
            raiz = raiz->esq;
            continue;
        }
        break;
    }

    // verifica se o registro existe
    if (raiz == NULL) {
        printf("Nenhum registro foi encontrado");
        return;
    }

    // desloca o fluxo do arquivo para onde o registro está salvo no arquivo e lê o arquivo
    fseek(tab->arquivo_dados, raiz->dado->indice, SEEK_SET);
    // estratégia de buffer para ler o registro que vai ser removido
    char *buffer = (char *) malloc(sizeof(char) * 1024);
    fgets(buffer, 1024, tab->arquivo_dados);
    buffer = strdup(buffer);
    tirar_enter(buffer);
    // como o registro agora é delimitado pelo caracter ',' é necessário uma estratégia para seus valores
    split_string(buffer, pokemon); 

    // remove a referência ao dado nos 3 indices baseado em suas chaves
    int diminuiu;
    tab->indice_bst = remover_bst(tab->indice_bst, chave);
    tab->indice_avl = remover_avl(tab->indice_avl, pokemon->poke_name, &diminuiu);
    remover_rb(pokemon->poke_total_status, &(tab->indice_rb));
    raiz->dado->chave = tamanho(pokemon);
    retab.indice_bst = inserir_bst(retab.indice_bst, raiz->dado);
}

/*
    Utilização da estratégia de buffer para alocação dinâmica de strings
    usamos o fgets() para pegar a string até o '\n' e tiramos o \n
    Usamos strdump pra criar uma "cópia" do conteúdo verdadeiro do buffer;
*/

poke_info* ler_dados() {
    poke_info *novo = (poke_info*) malloc(sizeof(poke_info));
    char *buffer = (char *) malloc(sizeof(char) * 256);

    getchar();
    printf("Nome: ");
    fgets(buffer, 255, stdin);
    tirar_enter(buffer);
    novo->poke_name = strdup(buffer);

    printf("Tipo: ");
    fgets(buffer, 255, stdin);
    tirar_enter(buffer);
    novo->poke_type1 = strdup(buffer);

    printf("Segundo Tipo: ");
    fgets(buffer, 255, stdin);
    tirar_enter(buffer);
    novo->poke_type2 = strdup(buffer);
    free(buffer);

    printf("Total Status: ");
    scanf("%d", &novo->poke_total_status);
    
    printf("Numero: ");
    scanf("%d", &novo->poke_number);
    return novo;
}

void tirar_enter(char *string){
    string[strlen(string) - 1] = '\0';
}

/*
    i) Implemente uma função para exibir os registros ordenados de acordo com cada 
    um dos índices (inorder);
*/

void in_order(tabela *tab) {
	in_order_bst(tab->indice_bst, tab);
    printf("\n");
	in_order_avl(tab->indice_avl, tab);
    printf("\n");
	in_order_rb(tab->indice_rb, tab);
}

void adicionar_pokemon(tabela *tab, poke_info *pokemon) {
    if (tab->arquivo_dados != NULL) {
        tipo_dado *novo_dado = (tipo_dado *) malloc(sizeof(tipo_dado));
        tipo_dado_avl *novo_avl = (tipo_dado_avl *) malloc(sizeof(tipo_dado_avl));
        tipo_dado_rb *novo_arb = (tipo_dado_rb *) malloc(sizeof(tipo_dado_rb));
        int espacos = 0;
        // copia o conteúdo de pokemon->pokename para o novo_avl
        novo_dado->chave = pokemon->poke_number;
        strcpy(novo_avl->poke_name, pokemon->poke_name);
        novo_arb->poke_total_status = pokemon->poke_total_status;
        // verifica se na tabela de realocação tem algum elemento
        int qtd_caracter = tamanho(pokemon);
        // busca por uma posição (sem uso) em que o elemento possa ser adicionado
        tipo_dado* posicao_livre = busca_posicao_livre(qtd_caracter);
        if (posicao_livre != NULL) {
            // cálculo de espaços brancos para substituir o elemento que antes estava nessa posição
            espacos = posicao_livre->chave - qtd_caracter; 
            // desloca o fluxo para uma posição livre
            fseek(tab->arquivo_dados, posicao_livre->indice, SEEK_SET); 
        }else fseek(tab->arquivo_dados, 0L, SEEK_END); // desloca o fluxo para o fim do arquivo

        // atualiza os indices com a posição em que foi adicionado
        novo_dado->indice = ftell(tab->arquivo_dados);
        novo_avl->indice = ftell(tab->arquivo_dados);
        novo_arb->indice = ftell(tab->arquivo_dados);
        
        if(espacos > 0) // se o tamanho para adicionar o registro for maior, preenche com espaço branco
            fprintf(tab->arquivo_dados, "%*s", espacos, "");

        // escreve o pokemon no arquivo tab->arquivo_dados
        fprintf(tab->arquivo_dados, "%d,%s,%s,%s,%d\n",
                                    pokemon->poke_number, 
                                    pokemon->poke_name,
                                    pokemon->poke_type1,
                                    pokemon->poke_type2,
                                    pokemon->poke_total_status);
        int cresceu;
        // Chama a função responsável por adicionar em cada índice
        tab->indice_bst = inserir_bst(tab->indice_bst, novo_dado);
        tab->indice_avl = inserir_avl(tab->indice_avl, novo_avl, &cresceu);
        adicionar_rb(novo_arb, &(tab->indice_rb));
        if (posicao_livre != NULL) { 
            // se inseriu em uma posição livre, então tira o elemento do indice da retab
            retab.indice_bst = remover_bst(retab.indice_bst, posicao_livre->chave);
        }
    }
}

/*
    e) Cada índice deve ser salvo em um arquivo próprio, conforme exemplificado no projeto "arquivo 1". 
    OBS: durante a execução do programa, os índices devem ser mantidos em memória RAM, sendo passados 
    para o arquivo apenas quando o usuário escolher a opção "sair" do programa;
*/  

void salvar_arquivo(tabela *tab) {
    FILE *arq;
    arq = fopen("./arquivos/indices_bst.dat", "wb");
    if(arq != NULL) {
        salvar_auxiliar_bst(tab->indice_bst, arq);
		fclose(arq);
    }

    arq = fopen("./arquivos/indices_avl.dat", "wb");
    if(arq != NULL) {
        salvar_auxiliar_avl(tab->indice_avl, arq);
		fclose(arq);
    }

    arq = fopen("./arquivos/indices_arb.dat", "wb");
    if(arq != NULL) {
        salvar_auxiliar_rb(tab->indice_rb, arq);
		fclose(arq);
    }
    // fecha arquivo da tabela de realocação pra reescrever
    fclose(retab.arquivo_dados);
    arq = fopen("./arquivos/removidos.dat", "wb");
    if(arq != NULL) {
        salvar_auxiliar_bst(retab.indice_bst, arq);
        fclose(retab.arquivo_dados);
    }
}

void finalizar(tabela *tab) {
    fclose(tab->arquivo_dados);
    salvar_arquivo(tab);
}

/*
    h) Implemente uma função de busca para cada índice. Esta função deve imprimir 
    na tela todos os dados satélite do registro referenciado pelo índice passado 
    como parâmetro;
*/

void busca_bst(tabela *tab, arvore_bst raiz, int chave) {
    if (raiz == NULL)
        return;
    
    if(raiz->dado->chave == chave)
        imprimir_elemento_bst(raiz, tab);
        
    
    if (chave > raiz->dado->chave)
        busca_bst(tab, raiz->dir, chave);
    else
        busca_bst(tab, raiz->esq, chave);
}

void busca_avl(tabela *tab, arvore_avl raiz, char *nome) {
    if (raiz == NULL)
        return;
    
    if(strcmp(nome, raiz->dado->poke_name) == 0)
        imprimir_elemento_avl(raiz, tab);
        
    
    if (strcmp(nome, raiz->dado->poke_name) > 0)
        busca_avl(tab, raiz->dir, nome);
    else
        busca_avl(tab, raiz->esq, nome);
}

void busca_rb(tabela *tab, arvore_rb raiz, int total_status) {
    if (raiz == NULL)
        return;
    
    if(raiz->dado->poke_total_status == total_status)
        imprimir_elemento_rb(raiz, tab);
        
    
    if (total_status > raiz->dado->poke_total_status)
        busca_rb(tab, raiz->dir, total_status);
    else
        busca_rb(tab, raiz->esq, total_status);
}

/*
    Parte do ponto extra: L)
    A estratégia de separação dos campos foi delimitando-os por ','
    A função split_string(char *string, poke_info *pokemon) é responsável
    por separar cada campo, através do Strtok(string, delim) e preenche o
    struct pokemon passado por referência.

    o switch deve seguir a ordem lógica de como foi adicionado
*/
void split_string(char *string, poke_info *pokemon) {
    int contador = 0;
    char *token = strtok(string, ",");
    while(token != NULL) {
        switch (contador)
        {
            case 0:
                pokemon->poke_number = atoi(token);
                break;
            case 1:
                pokemon->poke_name = strdup(token);
                break;
            case 2:
                pokemon->poke_type1 = strdup(token);
                break;
            case 3:
                pokemon->poke_type2 = strdup(token);
                break;
            case 4:
                pokemon->poke_total_status = atoi(token);
                break;
        
            default:
                break;
        }
        token = strtok(NULL, ",");
        contador++;
    }
}