#include "sgbd.h"
#include "stdio.h"
#include "stdlib.h"

int main(void) {
    tabela tb;
    int ret, opcao, chave;
    char nome[80];
    ret = inicializar_tabela(&tb);

    while(1) {
        scanf("%d", &opcao);
        switch (opcao)
        {
            case 1: // ADICIONAR POKEMON
                adicionar_pokemon(&tb, ler_dados());
                break;
            case 2: // Imprimir elementos
                in_order(&tb);
                break;
            case 3: // BUSCAR ELEMENTO por chave (BST)
                scanf("%d", &chave);
                busca_bst(&tb, tb.indice_bst, chave);
                break;
            case 4: // BUSCAR ELEMENTO por nome (AVL)
                getchar();
                fgets(nome, 80, stdin);
                tirar_enter(nome);
                busca_avl(&tb, tb.indice_avl, nome);
                break;
            case 5: // BUSCAR ELEMENTO por status (ARB)
                scanf("%d", &chave);
                busca_rb(&tb, tb.indice_rb, chave);
                break;
            case 6: // REMOVER ELEMENTO
                scanf("%d", &chave);
                remover_indice(&tb, chave);
                break;
            case 99: // SAIR
                finalizar(&tb);
                exit(99);
                break;
        
        default:
            break;
        }
    }

    return 1;
}