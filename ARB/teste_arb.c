#include "arb.h"
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
    arvore a;
    inicializar(&a);
    int opcao, valor, ret;
    // menu();
    // while (1){
    //     scanf("%d", &opcao);
    //     switch(opcao) {
    //         case 1: // inserir
    //             scanf("%d", &valor);
    //             a = inserir(a, valor);
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
    //             a = remover(a, valor);
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

    // adicionar(36, &a);
    // adicionar(40, &a);
    // adicionar(42, &a);
    // adicionar(40, &a);
    // adicionar(35, &a);
    // adicionar(40, &a);

    
    adicionar(1, &a);
    adicionar(4, &a);
    adicionar(7, &a);
    adicionar(10, &a);
    adicionar(13, &a);
    adicionar(16, &a);
    adicionar(19, &a);
    adicionar(22, &a);
    adicionar(25, &a);
    adicionar(28, &a);
    adicionar(60, &a);
    adicionar(57, &a);
    adicionar(54, &a);
    adicionar(51, &a);
    adicionar(48, &a);
    adicionar(45, &a);
    adicionar(42, &a);
    adicionar(39, &a);
    adicionar(36, &a);
    adicionar(33, &a);
    // a = inserir(a, 7);
    // a = inserir(a, 9);
    // a = inserir(a, 10);
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
    // printf("\n");

    // printf("SOMA: %d\n", somatorio(a));
    return 0;
}
