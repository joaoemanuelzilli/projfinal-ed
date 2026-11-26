#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

#define INF 1000000000

/* usado para ordenar a saida pelo nome */
typedef struct {
    char *nome;
    int valor;
} Item;

/* fila simples para a bfs */
typedef struct {
    int itens[MAX_CIDADES];
    int ini, fim;
} Fila;

static void fila_iniciar(Fila *f) {
    f->ini = 0;
    f->fim = 0;
}

static int fila_vazia(Fila *f) {
    return f->ini == f->fim;
}

static void fila_inserir(Fila *f, int v) {
    f->itens[f->fim++] = v;
}

static int fila_remover(Fila *f) {
    return f->itens[f->ini++];
}

static int compara(const void *a, const void *b) {
    return strcmp(((Item *)a)->nome, ((Item *)b)->nome);
}

void grafo_iniciar(Grafo *g) {
    g->n = 0;
}

/* devolve o indice da cidade nova */
int grafo_add_cidade(Grafo *g, const char *nome) {
    strcpy(g->cidades[g->n].nome, nome);
    g->cidades[g->n].adj = NULL;
    return g->n++;
}

static void add_aresta(Grafo *g, int a, int b, int km) {
    Aresta *nova = malloc(sizeof(Aresta));
    nova->destino = b;
    nova->km = km;
    nova->prox = g->cidades[a].adj;
    g->cidades[a].adj = nova;
}

/* mao dupla, entao insere nos dois sentidos */
void grafo_add_estrada(Grafo *g, int a, int b, int km) {
    add_aresta(g, a, b, km);
    add_aresta(g, b, a, km);
}

void grafo_vizinhos(Grafo *g, int a) {
    Item v[MAX_CIDADES * 2];
    int qtd = 0, i;
    Aresta *p;

    for (p = g->cidades[a].adj; p != NULL && qtd < MAX_CIDADES * 2; p = p->prox) {
        v[qtd].nome = g->cidades[p->destino].nome;
        v[qtd].valor = p->km;
        qtd++;
    }
    qsort(v, qtd, sizeof(Item), compara);
    for (i = 0; i < qtd; i++)
        printf("%s %d\n", v[i].nome, v[i].valor);
}

/* dijkstra simples, procura o menor varrendo o vetor */
void grafo_rota(Grafo *g, int a, int b) {
    int dist[MAX_CIDADES], anterior[MAX_CIDADES], fechado[MAX_CIDADES];
    int caminho[MAX_CIDADES];
    int i, j, u, tam;
    Aresta *p;

    for (i = 0; i < g->n; i++) {
        dist[i] = INF;
        anterior[i] = -1;
        fechado[i] = 0;
    }
    dist[a] = 0;

    for (i = 0; i < g->n; i++) {
        u = -1;
        for (j = 0; j < g->n; j++)
            if (!fechado[j] && dist[j] != INF && (u == -1 || dist[j] < dist[u]))
                u = j;
        if (u == -1)
            break;
        fechado[u] = 1;

        for (p = g->cidades[u].adj; p != NULL; p = p->prox) {
            if (dist[u] + p->km < dist[p->destino]) {
                dist[p->destino] = dist[u] + p->km;
                anterior[p->destino] = u;
            }
        }
    }

    if (dist[b] == INF) {
        printf("SEM ROTA\n");
        return;
    }

    /* monta o trajeto de tras para frente */
    tam = 0;
    for (u = b; u != -1; u = anterior[u])
        caminho[tam++] = u;

    for (i = tam - 1; i >= 0; i--) {
        printf("%s", g->cidades[caminho[i]].nome);
        if (i > 0)
            printf(" -> ");
    }
    printf(" (%d km)\n", dist[b]);
}

/* bfs a partir de a */
void grafo_alcance(Grafo *g, int a) {
    int visitado[MAX_CIDADES] = {0};
    Item v[MAX_CIDADES];
    int qtd = 0, i, u;
    Fila f;
    Aresta *p;

    fila_iniciar(&f);
    fila_inserir(&f, a);
    visitado[a] = 1;

    while (!fila_vazia(&f)) {
        u = fila_remover(&f);
        for (p = g->cidades[u].adj; p != NULL; p = p->prox) {
            if (!visitado[p->destino]) {
                visitado[p->destino] = 1;
                fila_inserir(&f, p->destino);
                v[qtd].nome = g->cidades[p->destino].nome;
                v[qtd].valor = p->destino;
                qtd++;
            }
        }
    }

    qsort(v, qtd, sizeof(Item), compara);
    for (i = 0; i < qtd; i++) {
        if (i > 0)
            printf(" ");
        printf("%s", v[i].nome);
    }
    printf("\n");
}

static void dfs(Grafo *g, int u, int regiao[], int num) {
    Aresta *p;
    regiao[u] = num;
    for (p = g->cidades[u].adj; p != NULL; p = p->prox)
        if (regiao[p->destino] == 0)
            dfs(g, p->destino, regiao, num);
}

void grafo_regioes(Grafo *g) {
    int regiao[MAX_CIDADES] = {0};
    Item v[MAX_CIDADES];
    int total = 0, i, r;

    for (i = 0; i < g->n; i++) {
        v[i].nome = g->cidades[i].nome;
        v[i].valor = i;
    }
    qsort(v, g->n, sizeof(Item), compara);

    /* percorre em ordem alfabetica, assim as regioes ja saem ordenadas */
    for (i = 0; i < g->n; i++)
        if (regiao[v[i].valor] == 0)
            dfs(g, v[i].valor, regiao, ++total);

    printf("%d regioes\n", total);
    for (r = 1; r <= total; r++) {
        printf("Regiao %d:", r);
        for (i = 0; i < g->n; i++)
            if (regiao[v[i].valor] == r)
                printf(" %s", v[i].nome);
        printf("\n");
    }
}

void grafo_liberar(Grafo *g) {
    int i;
    for (i = 0; i < g->n; i++) {
        Aresta *p = g->cidades[i].adj;
        while (p != NULL) {
            Aresta *aux = p->prox;
            free(p);
            p = aux;
        }
        g->cidades[i].adj = NULL;
    }
    g->n = 0;
}
