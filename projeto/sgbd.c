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

void inicializar_indices(tabela *tab) {
    inicializar_avl(&tab->indice_avl);
    inicializar_rb(&tab->indice_rb);
    inicializar_bst(&tab->indice_bst);
}

void carregar_arquivos(tabela *tab) {
    tipo_dado *temp;
    FILE *arq;
    arq = fopen("indices_avl.dat", "rb");
    int cresceu;
    if(arq != NULL) {
        temp = (tipo_dado*) malloc(sizeof(tipo_dado));
        while(fread(temp, sizeof(tipo_dado), 1, arq)) {
            tab->indice_avl = inserir_avl(tab->indice_avl, temp, &cresceu);
            temp = (tipo_dado*) malloc(sizeof(tipo_dado));
        }
        fclose(arq);
    }

    arq = fopen("indices_arb.dat", "rb");
    if(arq != NULL) {
        temp = (tipo_dado*) malloc(sizeof(tipo_dado));
        while(fread(temp, sizeof(tipo_dado), 1, arq)) {
            adicionar_rb(temp, &(tab->indice_rb));
            temp = (tipo_dado*) malloc(sizeof(tipo_dado));
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

// Carregar os valores de um arquivo "indices.dat" para cada árvore índice
void carregar_indices(tabela *tab) {
    carregar_arquivos(tab);
}

int inicializar_tabela(tabela *tabela) {
    inicializar_indices(tabela);
    tabela->arquivo_dados = fopen("dados.dat", "a+b");
    carregar_indices(tabela);

    if (tabela->arquivo_dados != NULL)
        return 1;
    else
        return -1;
}

void adicionar_indice(tabela *tab, tipo_dado *novo) {
    int cresceu;
    tab->indice_bst = inserir_bst(tab->indice_bst, novo);
    tab->indice_avl = inserir_avl(tab->indice_avl, novo, &cresceu);
    adicionar_rb(novo, &(tab->indice_rb));
}

void remover_indice(tabela *tab, int chave) {
    int diminuiu;
    tab->indice_bst = remover_bst(tab->indice_bst, chave);
    tab->indice_avl = remover_avl(tab->indice_avl, chave, &diminuiu);
    remover_rb(chave, &(tab->indice_rb));
}

poke_info* ler_dados() {
    poke_info *novo = (poke_info*) malloc(sizeof(poke_info));
    getchar();
    printf("Nome: ");
    fgets(novo->poke_name, 80, stdin);
    tirar_enter(novo->poke_name);

    printf("Tipo: ");
    fgets(novo->poke_type1, 36, stdin);
    tirar_enter(novo->poke_type1);

    printf("Segundo Tipo: ");
    fgets(novo->poke_type2, 36, stdin);
    tirar_enter(novo->poke_type2);

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
	in_order_bst(tab->indice_bst, tab);
	in_order_avl(tab->indice_avl, tab);
	in_order_rb(tab->indice_rb, tab);
}

void adicionar_pokemon(tabela *tabela, poke_info *pokemon) {
    if (tabela->arquivo_dados != NULL) {
        tipo_dado *novo_dado = (tipo_dado *) malloc(sizeof(tipo_dado));
        novo_dado->chave = pokemon->poke_number;

        // desloca o fluxo para o fim do arquivo
        fseek(tabela->arquivo_dados, 0L, SEEK_END);
        novo_dado->indice = ftell(tabela->arquivo_dados);

        fwrite(pokemon, sizeof(poke_info), 1, tabela->arquivo_dados);
        // Chama a função responsável por adicionar em cada índice
        adicionar_indice(tabela, novo_dado);
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

void busca(tabela *tab, arvore_avl raiz, int chave) {
    if (raiz == NULL)
        return;
    
    if(raiz->dado->chave == chave)
        // imprimir_elemento(raiz, tab);
        
    
    if (chave > raiz->dado->chave)
        busca(tab, raiz->dir, chave);
    else
        busca(tab, raiz->esq, chave);
}