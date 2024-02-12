#include "arb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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