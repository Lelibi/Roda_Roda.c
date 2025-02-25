#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h> 
#include <math.h>   

/* Estruturas */
typedef struct {
    char pista[16 + 1]; 
    int qtd; 
    char palavras[3][16 + 1]; 
} registro;

/* Variaveis globais */
registro reg; 
float premios[12];
FILE *Arq; 
float saldos[3] = {0, 0, 0}; 
int jogadorAtual = 0; 
char palavrasAdivinhadas[3][16 + 1]; 
int i, j; 

/* Funcoes auxiliares */
void carregarPremios() {
    Arq = fopen("PREMIOS.DAT", "rb"); // Abre o arquivo de prêmios em modo leitura binária
    if (!Arq) {  // Verifica se o arquivo foi aberto corretamente
        printf("\nErro ao abrir PREMIOS.DAT!");
        exit(1); // Encerra o programa em caso de erro
    }
    fread(&premios, sizeof(float), 12, Arq); // Lê os valores dos prêmios para o array
    fclose(Arq); // Fecha o arquivo
}

// Troca as palavras por "_"
void inicializarPalavrasAdivinhadas() {
    for(i = 0; i < reg.qtd; i++) {
        // Substitui cada letra da palavra por '_', indicando que ainda não foi descoberta
        for(j = 0; j < strlen(reg.palavras[i]); j++) {
            palavrasAdivinhadas[i][j] = '_';
        }
    }
}

// Conta o nomero de registros em cada vetor e sorteia palavra
void sortearPalavra() {
    int totalRegistros, sorteado;
    Arq = fopen("PALAVRAS.DAT", "rb"); // Abre o arquivo de palavras
    if (!Arq) {
        printf("\nErro ao abrir PALAVRAS.DAT!");
        exit(1);
    }

    // Conta quantos registros existem no arquivo
    fseek(Arq, 0, SEEK_END);
    totalRegistros = ftell(Arq) / sizeof(registro); 
    rewind(Arq); 

    // Sorteia um índice aleatório dentro do número de registros
    srand(time(NULL)); 
    int num_ale = rand();
    sorteado = num_ale % totalRegistros;

    // Posiciona o ponteiro do arquivo no registro sorteado e lê os dados
    fseek(Arq, sorteado * sizeof(registro), SEEK_SET);
    fread(&reg, sizeof(registro), 1, Arq);
    fclose(Arq);

    inicializarPalavrasAdivinhadas(); // Inicializa as palavras ocultas
}   

// Soretia o valor do prêmio 
float sortearPremio() {
    int sorteado = rand() % 12; 
    return premios[sorteado];   
}

// Exibe cada letra já descoberta ou _ caso ainda não tenha sido adivinhada
void exibirPalavrasAdivinhadas() {
    for (i = 0; i < reg.qtd; i++) { // Percorre as palavras sorteadas
        printf("\n\t\t");
        for (j = 0; j < strlen(reg.palavras[i]); j++) { // Exibe cada letra oculta
            printf("%c ", palavrasAdivinhadas[i][j]); 
        }
        printf("\n");
    }
}

// Layout tela: jogadores, saldo
void exibirJogadores() {
    printf("\n\t%s\t%s \t%s\n", "Jogador 1", "Jogador 2", "Jogador 3");
    printf("\t%s\t%s \t%s\n", "=========", "=========", "=========");
    printf("\tR$ %.2f \tR$ %.2f \tR$ %.2f\n", saldos[0], saldos[1], saldos[2]);
}

// Comparar a palavra do registro com a palavra digitada
int verificarPalavraCompleta() {
    for (i = 0; i < reg.qtd; i++) {
        if (strcmp(palavrasAdivinhadas[i], reg.palavras[i]) != 0) {
            return 0; 
        }
    }
    return 1; // Todas as palavras foram completadas
}

// Seleciona o venedor com base no maior saldo
void verificarVencedor() {
    int vencedor = 0; 
    for (i = 1; i < 3; i++) {
        if (saldos[i] > saldos[vencedor]) {
            vencedor = i; 
        }
    }
    
    // Exibe a posição (pelo maior saldo) do vencedor na tabela
    /* if (vencedor == 0) {
        printf("\tVENCEDOR\n");
    } else if (vencedor == 1) {
        printf("\t\t\tVENCEDOR\n");
    } else if (vencedor == 2) {
        printf("\t\t\t\t\tVENCEDOR\n");
    }*/
    
    // Exibe a posição (acertou a palavra) do vencedor na tabela
    if (jogadorAtual == 0) {
        printf("\tVENCEDOR\n");
    } else if (jogadorAtual == 1) {
        printf("\t\t\tVENCEDOR\n");
    } else if (jogadorAtual == 2) {
        printf("\t\t\t\t\tVENCEDOR\n");
    }
    
    // Exibe o vencedor e o valor ganho
    // printf("\n\tO vencedor é o Jogador %d com R$ %.2f!\n", vencedor + 1, saldos[vencedor]);
}

void jogarRodada() {
    static float premioCongelado = 0.0; // Variável para armazenar o prêmio congelado 
    float premio;

    // Se o prêmio não estiver congelado, sorteie um novo prêmio
    if (premioCongelado == 0.0) {
        premio = sortearPremio();
    } else {
        premio = premioCongelado; // Usar o prêmio congelado
    }

    // Exibir "Roda a Roda!" somente se o prêmio não for "Passa a vez" ou "Perde tudo"
    if (premio != 0.00 && fabs(premio - 0.01) >= 0.0001) {
        printf("\n\tRoda a Roda!  (jogador %d)\n", jogadorAtual + 1);
    }

    // Caso o jogador tire "Passa a vez"
    if (premio == 0.00) {
        printf("\n\tPASSA A VEZ!");
        printf("\t(Conferir valor R$ %.2f) ", premio);
        printf("\n\tA vez será passada para o jogador %d.", (jogadorAtual + 1) % 3 + 1);
        jogadorAtual = (jogadorAtual + 1) % 3; // Passa a vez para o próximo jogador
        premioCongelado = 0.0; // Reseta o prêmio congelado
        return;
    } else if (fabs(premio - 0.01) < 0.0001) { // Caso o jogador tire "Perde tudo"
        printf("\n\tPERDEU TUDO!");
        printf("\t(Conferir valor R$ %.2f) ", premio);
        saldos[jogadorAtual] = 0; // Zera o saldo do jogador
        printf("\n\tA vez será passada para o jogador %d.", (jogadorAtual + 1) % 3 + 1);
        jogadorAtual = (jogadorAtual + 1) % 3;
        premioCongelado = 0.0; // Resetar o prêmio congelado
        return;
    }

    // Contar quantas letras ainda não foram descobertas
    int letrasRestantes = 0;
    for (i = 0; i < reg.qtd; i++) { 
        for (j = 0; j < strlen(reg.palavras[i]); j++) {
            if (palavrasAdivinhadas[i][j] == '_') {
                letrasRestantes++;
            }
        }
    }

    // Se restam 3 ou menos letras, permite a tentativa de adivinhar a palavra completa
    if (letrasRestantes <= 3) {
        if (premioCongelado == 0.0) {
            premioCongelado = premio;
        }

        int tentativas = 0;
        while (tentativas < 3) {
            printf("\n\tJogador %d: Roda a Roda! \n\tFaltam %d letras para completar as palavras.\n", jogadorAtual + 1, letrasRestantes);
            printf("\tVoce tem 5 segundos para pensar e depois digitar as palavras completas...\n");

            // Contagem regressiva para digitação
            int t = 5;
            for (t; t > 0; t--) {
                printf("\t%d...", t);
                fflush(stdout);
                sleep(1);
            }
            printf("\n\tConferir prêmio: %.2f", premioCongelado);
            printf("\n\tValendo R$ %.2f, a palavra e:\n", premioCongelado + saldos[jogadorAtual]);
            
            // Solicita todas as palavras do jogador
            char resposta[reg.qtd][16 + 1]; // Matriz para armazenar respostas
			for (i = 0; i < reg.qtd; i++) {
   			 	printf("\tPalavra %d: ", i + 1);
    			scanf("%16s", resposta[i]); // Lê a palavra sem ultrapassar 16 caracteres

    		// Converte toda a string para maiúsculas corretamente
   			 	for (j = 0; resposta[i][j] != '\0'; j++) {
       		 		resposta[i][j] = toupper(resposta[i][j]);
    			}
			}

            // Verifica se o jogador acertou todas as palavras
            int acertouTodas = 1;
            for (i = 0; i < reg.qtd; i++) {
                if (strcmp(resposta[i], reg.palavras[i]) != 0) {
                    acertouTodas = 0;
                    break;
                }
            }

            if (acertouTodas) {
                saldos[jogadorAtual] += premioCongelado;
                for (i = 0; i < reg.qtd; i++) {
                    strcpy(palavrasAdivinhadas[i], reg.palavras[i]);
                }
                premioCongelado = 0.0;
                return;
            } else {
                jogadorAtual = (jogadorAtual + 1) % 3;
                tentativas++;
            }
            getchar();
            system("cls");
            printf("\n\n\tA palavra está associada com: %s\n", reg.pista);
            exibirPalavrasAdivinhadas();
            exibirJogadores();
        }

        // Se todos erraram, encerra o jogo
        system("cls");
        printf("\n\tNENHUM JOGADOR VENCEU.\n");
        printf("\n\tSaldos Finais.\n");
        premioCongelado = 0.0;
        exibirJogadores();
        exit(0);
    }

    // Jogador escolhe uma letra
    char letra;
    printf("\tUma letra por: R$ %.2f: ", premio);
    scanf(" %c", &letra);
    letra = toupper(letra);

    int acertou = 0;
    for (i = 0; i < reg.qtd; i++) {
        for (j = 0; j < strlen(reg.palavras[i]); j++) {
            if (toupper(reg.palavras[i][j]) == letra && palavrasAdivinhadas[i][j] == '_') {
                palavrasAdivinhadas[i][j] = reg.palavras[i][j];
                acertou = 1;
                saldos[jogadorAtual] += premio;
            }
        }
    }

    if (!acertou) {
        printf("\tNão existe letra %c na palavra.\n", letra);
        printf("\tA vez será passada para o jogador %d.", (jogadorAtual + 1) % 3 + 1);
        jogadorAtual = (jogadorAtual + 1) % 3;
        getchar();
    }
}

/* Funcao principal */
int main() {
	setlocale(LC_ALL, "Portuguese");
    carregarPremios();
    sortearPalavra();
    
    // Esperar pelo Enter antes de mostrar o restante
    printf("\n\n\tRRRR    OOO    DDDD     AAA   	 	 RRRR    OOO    DDDD     AAA   \n");
    printf("\tR   R  O   O   D   D   A   A      	  R   R  O   O   D   D   A   A  \n");
    printf("\tRRRR   O   O   D   D  AAAAAAA     A 	  RRRR   O   O   D   D  AAAAAAA \n");
    printf("\tR  R   O   O   D   D  A     A     	  R  R   O   O   D   D  A     A \n");
    printf("\tR   R   OOO    DDDD   A     A     	  R   R   OOO    DDDD   A     A \n");
    printf("\n\n\t Vamos iniciar?");

	getchar(); // Aguarda o Enter do jogador
    system("cls");  // Comando para Windows

    printf("\n\n\tA palavra está associada com: %s\n\n", reg.pista);
    exibirJogadores();

    // Esperar pelo Enter antes de mostrar o restante
    printf("\n\n\tPressione Enter para Rodar...");
    getchar(); // Aguarda o Enter do jogador
	system("cls");  // Comando para Windows

    while (!verificarPalavraCompleta()) {
        printf("\n\n\tA palavra está associada com: %s\n", reg.pista);
        exibirPalavrasAdivinhadas();
        exibirJogadores();
        jogarRodada();
        getchar(); // Aguarda o Enter do jogador
        system("cls");  // Comando para Windows

    }

    printf("\n\n\tParabéns! A palavra foi completada!\n");
    exibirPalavrasAdivinhadas();
    exibirJogadores();
    verificarVencedor();

    return 0;
}


