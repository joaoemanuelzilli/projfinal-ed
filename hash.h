#ifndef HASH_H
#define HASH_H

#define TAM_HASH 53
#define MAX_NOME 51

typedef struct NoHash {
    char nome[MAX_NOME];
    int indice;
    struct NoHash *prox;
} NoHash;

typedef struct {
    NoHash *tabela[TAM_HASH];
} Hash;

void hash_iniciar(Hash *h);
unsigned int djb2(const char *s);
int hash_posicao(const char *nome);
void hash_inserir(Hash *h, const char *nome, int indice);
int hash_buscar(Hash *h, const char *nome);
void hash_liberar(Hash *h);

#endif
