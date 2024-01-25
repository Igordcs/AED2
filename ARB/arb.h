#ifndef ARB_H
#define ARB_H

enum cor { VERMELHO, PRETO, DUPLO_PRETO };

typedef int tipo_dado;

typedef struct no_bst {
	tipo_dado valor;
	enum cor cor;
	struct no_bst *esq, *dir, *pai;
} no_bst;

typedef no_bst * arvore;

void inicializar(arvore *raiz);

void adicionar (int valor, arvore *raiz) ;
void imprimir_elemento(arvore raiz);
void rotacao_simples_direita(arvore *raiz, arvore pivo);
void rotacao_simples_esquerda(arvore *raiz, arvore pivo);
arvore remover (arvore raiz, int valor) ;
arvore podar (arvore raiz, int valor) ;

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
