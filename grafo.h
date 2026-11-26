#ifndef GRAFO_H
#define GRAFO_H

#define MAX_CIDADES 50
#define MAX_NOME 51

/* no da lista de adjacencia */
typedef struct Aresta {
    int destino;
    int km;
    struct Aresta *prox;
} Aresta;

typedef struct {
    char nome[MAX_NOME];
    Aresta *adj;
} Cidade;

typedef struct {
    Cidade cidades[MAX_CIDADES];
    int n;
} Grafo;

void grafo_iniciar(Grafo *g);
int grafo_add_cidade(Grafo *g, const char *nome);
void grafo_add_estrada(Grafo *g, int a, int b, int km);
void grafo_vizinhos(Grafo *g, int a);
void grafo_rota(Grafo *g, int a, int b);
void grafo_alcance(Grafo *g, int a);
void grafo_regioes(Grafo *g);
void grafo_liberar(Grafo *g);

#endif
