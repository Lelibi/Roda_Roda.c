#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Estrutura do registro */
typedef struct {
    char pista[16 + 1];
    int qtd;
    char palavras[3][16 + 1];
} registro;

int main() {
    FILE *Arq;
    registro reg;

    /* Dados para gravar no arquivo */
    registro dados[] = {
        {"Vegetal", 2, {"ACELGA", "ALFACE", ""}},
        {"Automovel", 3, {"MOTOR", "EMBREAGEM", "ESCAPAMENTO"}},
        {"Cozinha", 3, {"PRATO", "PANELA", "FOGAO"}},
        {"Reptil", 1, {"JARARACA", "", ""}},
        {"Mamifero", 2, {"BALEIA", "MACACO", ""}}
    };
    int n = sizeof(dados) / sizeof(dados[0]);

    /* Cria e abre o arquivo PALAVRAS.DAT para gravação */
    Arq = fopen("PALAVRAS.DAT", "wb");
    if (Arq == NULL) {
        printf("\nErro ao criar o arquivo PALAVRAS.DAT!");
        exit(1);
    }

    /* Grava os registros no arquivo */
    int i = 0;
    for (i; i < n; i++) {
        fwrite(&dados[i], sizeof(registro), 1, Arq);
        if (ferror(Arq)) {
            printf("\nErro ao gravar no arquivo!");
            fclose(Arq);
            exit(1);
        }
    }

    /* Fecha o arquivo */
    fclose(Arq);

    printf("\nArquivo PALAVRAS.DAT criado com sucesso!");
    return 0;
}

