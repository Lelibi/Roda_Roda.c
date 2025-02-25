#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

void PremiosDefault(void) {
    float rpr[12] = {
        100.00,
        900.00,
        600.00,
        1000.00,
        0.00,
        400.00,
        200.00,
        800.00,
        0.01,
        300.0,
		500.0,
        700.0
    };

    FILE *Arq = fopen("PREMIOS.DAT", "wb");
    if (!Arq) {
        printf("\nERRO AO CRIAR PREMIOS.DAT!");
        exit(1);
    }

    fwrite(rpr, sizeof(rpr), 1, Arq);
    fclose(Arq);
    printf("\nArquivo PREMIOS.DAT criado com sucesso!");
}

int main() {
    setlocale(LC_ALL, "");
    PremiosDefault();
    return 0;
}

