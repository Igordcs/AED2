#ifndef AVL_H
#define AVL_H

typedef struct no {
    int valor;
    int fb;
    struct no *esq;
    struct no *dir;
} no;

typedef no* arvore;

arvore inserir (arvore raiz, int valor, int *cresceu) ;
arvore remover (arvore raiz, int valor, int* diminuiu) ;
arvore podar (arvore raiz, int valor) ;
arvore rotacao(arvore pivo);

void preorder(arvore raiz);
void inorder(arvore raiz);
void posorder(arvore raiz);
void reverso(arvore raiz);

int somatorio(arvore raiz);
int somaPar(arvore raiz);
int altura(arvore raiz);
int qtdpar(arvore raiz);
int busca(arvore raiz, int valor);

void dobrar_valores(arvore raiz);
void pai (arvore raiz, int valor);
void descendentes(arvore raiz, int valor);
void antecessor(arvore raiz, int valor);




#endif
