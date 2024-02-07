#include "sgbd.h"
#include "stdio.h"
#include "stdlib.h"

int main(void) {
    tabela tb;
    int ret, opcao, chave;
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
            case 3: // BUSCAR ELEMENTO
                scanf("%d", &chave);
                busca(&tb, tb.indice_avl, chave);
                break;
            case 4: // REMOVER ELEMENTO
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