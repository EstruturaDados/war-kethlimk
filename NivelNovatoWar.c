#include <stdio.h>
#include <string.h>

/*
  Cadastro de Territórios no jogo War
  Objetivo: Permitir o cadastro de 5 territórios com informações de
            nome, cor do exército e quantidade de tropas.
 
*/

/* 
  Definição da struct de Territorio
*/
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// Função para limpar o buffer do teclado (stdin)
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    struct Territorio territorios[5]; // Vetor para armazenar 5 territórios

    printf("=== Cadastro de Territórios ===\n\n");

    // Cadastro dos 5 territórios
    for (int i = 0; i < 5; i++) {
        printf("Cadastro do território %d:\n", i + 1);

        // Leitura do nome do território 
        printf("Digite o nome do território: ");
        scanf(" %[^\n]", territorios[i].nome); 
        limparBuffer(); 

        // Leitura da cor do exército 
        printf("Digite a cor do exército: ");
        scanf("%s", territorios[i].cor);
        limparBuffer(); 

        // Leitura da quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        limparBuffer(); 

        printf("\n"); // Linha em branco para melhor visualização das informações inseridas 
    }

    // Exibição dos dados cadastrados
    printf("\n=== Dados dos Territórios Cadastrados ===\n");
    for (int i = 0; i < 5; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
    }

    printf("\nCadastro finalizado com sucesso!\n");
    return 0;
}
