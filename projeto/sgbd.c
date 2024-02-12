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
#include "bst/bst.h"

tab_realoc retab;

tipo_dado* existe_posicao_livre(int tamanho_elemento) {
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
    arq = fopen("indices_avl.dat", "rb");

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

    arq = fopen("indices_arb.dat", "rb");
    if(arq != NULL) {
        temp_arb = (tipo_dado_rb*) malloc(sizeof(tipo_dado_rb));
        while(fread(temp_arb, sizeof(tipo_dado_rb), 1, arq)) {
            adicionar_rb(temp_arb, &(tab->indice_rb));
            temp_arb = (tipo_dado_rb*) malloc(sizeof(tipo_dado_rb));
        }
        fclose(arq);
    }

    arq = fopen("indices_bst.dat", "rb");
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
    tabela->arquivo_dados = fopen("dados.dat", "r+b");
    retab.arquivo_dados = fopen("removidos.dat", "a+b");
    carregar_arquivos(tabela);

    if(tabela->arquivo_dados == NULL) {
        tabela->arquivo_dados = fopen("dados.dat", "wb");
        fclose(tabela->arquivo_dados);
        tabela->arquivo_dados = fopen("dados.dat", "r+b");
    }

    if (tabela->arquivo_dados != NULL)
        return 1;
    else
        return -1;
}

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
    char *buffer = (char *) malloc(sizeof(char) * 1024);
    fgets(buffer, 1024, tab->arquivo_dados);
    buffer = strdup(buffer);
    tirar_enter(buffer);
    split_string(buffer, pokemon);

    // remove a referência ao dado nos 3 indices baseado em suas chaves
    int diminuiu;
    tab->indice_bst = remover_bst(tab->indice_bst, chave);
    tab->indice_avl = remover_avl(tab->indice_avl, pokemon->poke_name, &diminuiu);
    remover_rb(pokemon->poke_total_status, &(tab->indice_rb));
    raiz->dado->chave = tamanho(pokemon);
    retab.indice_bst = inserir_bst(retab.indice_bst, raiz->dado);
}

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

void in_order(tabela *tab) {
    // in_order_bst(retab.indice_bst, tab);
    // printf("\n");
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
        // copia o conteúdo de pokemon->pokename para o novo_avl
        novo_dado->chave = pokemon->poke_number;
        strcpy(novo_avl->poke_name, pokemon->poke_name);
        novo_arb->poke_total_status = pokemon->poke_total_status;
        // verifica se na tabela de realocação tem algum elemento
        int qtd_caracter = tamanho(pokemon);
        tipo_dado* posicao_livre = existe_posicao_livre(qtd_caracter);
        if (posicao_livre != NULL) 
            fseek(tab->arquivo_dados, posicao_livre->indice, SEEK_SET); // desloca o fluxo para uma posição livre
        else fseek(tab->arquivo_dados, 0L, SEEK_END); // desloca o fluxo para o fim do arquivo

        // atualiza os indices com a posição em que foi adicionado
        novo_dado->indice = ftell(tab->arquivo_dados);
        novo_avl->indice = ftell(tab->arquivo_dados);
        novo_arb->indice = ftell(tab->arquivo_dados);

        // escreve o pokemon no arquivo tab->arquivo_dados
        int r = fprintf(tab->arquivo_dados, "%d,%s,%s,%s,%d\n", 
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
            retab.indice_bst = remover_bst(retab.indice_bst, posicao_livre->chave);
        }
    }
}

/*
    e) Cada índice deve ser salvo em um arquivo próprio, conforme exemplificado no projeto "arquivo 1". 
    OBS: durante a execução do programa, os índices devem ser mantidos em memória RAM, sendo passados 
    para o arquivo apenas quando o usuário escolher a opção "sair" do programa;

    FEITO!!
*/  

void salvar_arquivo(tabela *tab) {
    FILE *arq;
    arq = fopen("indices_bst.dat", "wb");
    if(arq != NULL) {
        salvar_auxiliar_bst(tab->indice_bst, arq);
		fclose(arq);
    }

    arq = fopen("indices_avl.dat", "wb");
    if(arq != NULL) {
        salvar_auxiliar_avl(tab->indice_avl, arq);
		fclose(arq);
    }

    arq = fopen("indices_arb.dat", "wb");
    if(arq != NULL) {
        salvar_auxiliar_rb(tab->indice_rb, arq);
		fclose(arq);
    }
    // fecha arquivo da tabela de realocação pra reescrever
    fclose(retab.arquivo_dados);
    arq = fopen("removidos.dat", "wb");
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