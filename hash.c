#include <stdlib.h>
#include <string.h>
#include "hash.h"

void hash_iniciar(Hash *h) {
    int i;
    for (i = 0; i < TAM_HASH; i++)
        h->tabela[i] = NULL;
}

unsigned int djb2(const char *s) {
    unsigned int h = 5381;
    while (*s)
        h = h * 33 + (unsigned char)*s++;
    return h;
}

int hash_posicao(const char *nome) {
    return djb2(nome) % TAM_HASH;
}

/* insere no inicio da lista da posicao */
void hash_inserir(Hash *h, const char *nome, int indice) {
    int pos = hash_posicao(nome);
    NoHash *novo = malloc(sizeof(NoHash));
    strcpy(novo->nome, nome);
    novo->indice = indice;
    novo->prox = h->tabela[pos];
    h->tabela[pos] = novo;
}

/* devolve o indice do vertice ou -1 se nao achar */
int hash_buscar(Hash *h, const char *nome) {
    NoHash *p = h->tabela[hash_posicao(nome)];
    while (p != NULL) {
        if (strcmp(p->nome, nome) == 0)
            return p->indice;
        p = p->prox;
    }
    return -1;
}

void hash_liberar(Hash *h) {
    int i;
    for (i = 0; i < TAM_HASH; i++) {
        NoHash *p = h->tabela[i];
        while (p != NULL) {
            NoHash *aux = p->prox;
            free(p);
            p = aux;
        }
        h->tabela[i] = NULL;
    }
}
