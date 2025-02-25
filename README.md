# Roda_Roda.c
Aplicação simulando o jogo Roda Roda do Silvio Santos 
# Roda a Roda - Jogo em C

Este é um jogo desenvolvido em C baseado no programa 'Roda a Roda' do SBT.

## Descrição
O jogo sorteia palavras e os jogadores devem adivinhá-las para acumular prêmios. A mecânica é semelhante ao jogo original da TV, onde os jogadores giram a roda, sorteiam prêmios e tentam acertar as letras ou palavras completas.

## Recursos
- Sorteio de palavras a partir de um arquivo externo (`PALAVRAS.DAT`).
- Sorteio de prêmios de um arquivo externo (`PREMIOS.DAT`).
- Exibição de palavras parcialmente reveladas conforme os jogadores acertam letras.
- Sistema de pontuação baseado nos prêmios sorteados.
- Determinação automática do vencedor com base no maior saldo acumulado.

## Requisitos
- Dev-C++ (foi usado para compilar e rodar o código)
- Sistema operacional Windows (devido ao uso de `system("cls")` para limpar a tela)
- Arquivos `PALAVRAS.DAT` e `PREMIOS.DAT` para armazenar palavras e prêmios.

## Como Jogar
1. Compile o código no Dev-C++.
2. Execute o jogo.
3. O jogo irá sortear uma palavra e os jogadores terão que adivinhar as letras.
4. Os jogadores giram a roda para sortear prêmios e escolhem letras para tentar completar a palavra.
5. O jogo termina quando a palavra for completada.
6. O jogador com o maior saldo é declarado vencedor.

## Observações
- O jogo pode ser adaptado para outros compiladores alterando a função `system("cls")` para `system("clear")` caso seja usado em sistemas Unix/Linux.

## Autor
Desenvolvido por @Lelibi perfil no GitHub.

