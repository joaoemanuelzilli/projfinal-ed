#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "grafo.h"

Grafo g;
Hash h;

/* acha o indice pela hash e avisa se a cidade nao existe */
int buscar(const char *nome) {
    int i = hash_buscar(&h, nome);
    if (i == -1)
        printf("ERRO: cidade %s nao encontrada\n", nome);
    return i;
}

int main() {
    char linha[256], cmd[20], a[MAX_NOME], b[MAX_NOME];
    int km, ia, ib;

    grafo_iniciar(&g);
    hash_iniciar(&h);

    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        if (sscanf(linha, "%19s", cmd) != 1)
            continue;

        if (strcmp(cmd, "FIM") == 0)
            break;

        if (strcmp(cmd, "CIDADE") == 0) {
            if (sscanf(linha, "%*s %50s", a) != 1)
                continue;
            if (hash_buscar(&h, a) != -1)
                printf("ERRO: cidade %s ja cadastrada\n", a);
            else if (g.n == MAX_CIDADES)
                printf("ERRO: limite de cidades atingido\n");
            else
                hash_inserir(&h, a, grafo_add_cidade(&g, a));
        }
        else if (strcmp(cmd, "ESTRADA") == 0) {
            if (sscanf(linha, "%*s %50s %50s %d", a, b, &km) != 3 || km <= 0)
                continue;
            ia = buscar(a);
            if (ia == -1)
                continue;
            ib = buscar(b);
            if (ib == -1)
                continue;
            grafo_add_estrada(&g, ia, ib, km);
        }
        else if (strcmp(cmd, "HASH") == 0) {
            if (sscanf(linha, "%*s %50s", a) != 1)
                continue;
            printf("> %s\n", linha);
            if (buscar(a) != -1)
                printf("%s -> posicao %d\n", a, hash_posicao(a));
        }
        else if (strcmp(cmd, "VIZINHOS") == 0) {
            if (sscanf(linha, "%*s %50s", a) != 1)
                continue;
            printf("> %s\n", linha);
            ia = buscar(a);
            if (ia != -1)
                grafo_vizinhos(&g, ia);
        }
        else if (strcmp(cmd, "ROTA") == 0) {
            if (sscanf(linha, "%*s %50s %50s", a, b) != 2)
                continue;
            printf("> %s\n", linha);
            ia = buscar(a);
            if (ia == -1)
                continue;
            ib = buscar(b);
            if (ib == -1)
                continue;
            grafo_rota(&g, ia, ib);
        }
        else if (strcmp(cmd, "ALCANCE") == 0) {
            if (sscanf(linha, "%*s %50s", a) != 1)
                continue;
            printf("> %s\n", linha);
            ia = buscar(a);
            if (ia != -1)
                grafo_alcance(&g, ia);
        }
        else if (strcmp(cmd, "REGIOES") == 0) {
            printf("> %s\n", linha);
            grafo_regioes(&g);
        }
    }

    grafo_liberar(&g);
    hash_liberar(&h);
    return 0;
}
