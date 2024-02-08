#include "avl.h"
#include <stdlib.h>
#include <stdio.h>

void menu() {
    printf("\n1 - inserir\n");
    printf("2 - preorder\n");
    printf("3 - inorder\n");
    printf("4 - posorder\n");
    printf("5 - reverso\n");
    printf("6 - qtdPar\n");
    printf("7 - antecessor\n");
    printf("8 - pai\n");
    printf("9 - remover\n");
    printf("10 - somaPar\n");
    printf("11 - podar\n");
    printf("12 - dobro\n");
    printf("13 - busca\n");
    printf("14 - descendentes\n");
    printf("15 - altura\n");
    printf("99 - sair\n");
}

int main(int argc, char * argv[]) {
    //declarar e inicializar como árvore vazia
    arvore a = NULL;
    int opcao, valor, ret, cresceu, diminuiu;
    menu();
    // while (1){
    //     scanf("%d", &opcao);
    //     switch(opcao) {
    //         case 1: // inserir
    //             scanf("%d", &valor);
    //             a = inserir(a, valor, &cresceu);
    //         break;
    //         case 2: // preorder
    //             preorder(a);
    //             printf("\n");
    //         break;
    //         case 3: // inorder
    //             inorder(a);
    //             printf("\n");
    //         break;
    //         case 4: // posorder
    //             posorder(a);
    //             printf("\n");
    //         break;
    //         case 5: // reverso
    //             reverso(a);
    //             printf("\n");
    //         break;
    //         case 6: // QtdPar
    //             ret = qtdpar(a);
    //             printf("%d\n", ret);
    //         break;
    //         case 7: // antecessor
    //             scanf("%d", &valor);
    //             antecessor(a, valor);
    //         break;
    //         case 8: // pai
    //             scanf("%d", &valor);
    //             pai(a, valor);
    //         break;
    //         case 9: // remover
    //             scanf("%d", &valor);
    //             a = remover(a, valor, &diminuiu);
    //         break;
    //         case 10: // somaPar
    //             ret = somaPar(a);
    //             printf("%d\n", ret);
    //         break;
    //         case 11: // podar
    //             scanf("%d", &valor);
    //             a = podar(a, valor);
    //         break;
    //         case 12: // dobro
    //             dobrar_valores(a);
    //         break;
    //         case 13: // busca
    //             scanf("%d", &valor);
    //             ret = busca(a, valor);
    //             printf("%d\n", ret);
    //         break;
    //         case 14: // descendentes
    //             scanf("%d", &valor);
    //             descendentes(a, valor);
    //             printf("\n");
    //         break;
    //         case 15: // altura
    //             ret = altura(a);
    //             printf("%d\n", ret);
    //         break;
    //         case 99: // SAIR
    //             exit(0);
    //         break;
    //         default:
    //             printf("OPÇÃO INVALIDA");
    //         break;
    // }}
    a = inserir(a, 1, &cresceu);
    a = inserir(a, 4, &cresceu);
    a = inserir(a, 7, &cresceu);
    a = inserir(a, 10, &cresceu);
    a = inserir(a, 13, &cresceu);
    a = inserir(a, 16, &cresceu);
    a = inserir(a, 19, &cresceu);
    a = inserir(a, 22, &cresceu);
    a = inserir(a, 25, &cresceu);
    a = inserir(a, 28, &cresceu);
    a = inserir(a, 60, &cresceu);
    a = inserir(a, 57, &cresceu);
    a = inserir(a, 54, &cresceu);
    a = inserir(a, 51, &cresceu);
    a = inserir(a, 48, &cresceu);
    a = inserir(a, 45, &cresceu);
    a = inserir(a, 42, &cresceu);
    a = inserir(a, 39, &cresceu);
    a = inserir(a, 36, &cresceu);
    a = inserir(a, 33, &cresceu);
    a = remover(a, 45, &diminuiu);
    a = remover(a, 39, &diminuiu);
    a = remover(a, 36, &diminuiu);
    a = remover(a, 28, &diminuiu);
    a = remover(a, 1, &diminuiu);
    a = remover(a, 7, &diminuiu);
    a = remover(a, 13, &diminuiu);
    a = remover(a, 19, &diminuiu);
    a = remover(a, 57, &diminuiu);
    a = remover(a, 60, &diminuiu);
    // preorder(a);
    a = remover(a, 25, &diminuiu);
    a = inserir(a, 18, &cresceu);
    a = remover(a, 16, &diminuiu);
    // preorder(a);
    // preorder(a);
    // preorder(a);
    // preorder(a);
    // preorder(a);
    // preorder(a);
    // preorder(a);
    // a = inserir(a, 27);
    // a = inserir(a, 12);
    // a = inserir(a, 21);
    // a = inserir(a, 30);
    // a = inserir(a, 2);

    printf("PRE-ORDER: ");
    preorder(a);
    // printf("\nIN-ORDER: ");
    // inorder(a);
    // printf("\nPOS-ORDER: ");
    // posorder(a);
    printf("\n");

    // printf("SOMA: %d\n", somatorio(a));
    return 0;
}
