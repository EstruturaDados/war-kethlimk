#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
    Simulação de Territórios e Ataques do War
    O objetivo é permitir o cadastro de territórios, a simulação de ataques entre eles
              e manipulação dinâmica de memória utilizando ponteiros.
*/

/* 
Definição da struct 
*/
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};
typedef struct Territorio Territorio;

/* 
Função para limpar o buffer 
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*  
Cadastro dos territórios 
*/
void cadastrarTerritorios(Territorio *mapa, int qtd) {
    printf("\n=== Cadastro de Territórios ===\n\n");
    for (int i = 0; i < qtd; i++) {
        printf("Cadastro do território %d:\n", i + 1);

        printf("Nome do território: ");
        scanf(" %[^\n]", mapa[i].nome);
        limparBuffer();

        printf("Cor do exército: ");
        scanf(" %s", mapa[i].cor);
        limparBuffer();

        printf("Quantidade inicial de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer();

        printf("\n");
    }
}

/* 
Exibição dos territórios
*/
void exibirTerritorios(Territorio *mapa, int qtd) {
    printf("\n=== Situação Atual dos Territórios ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor do exército: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
    printf("\n");
}

/* 
Exibir o ataque 
 */
void atacar(Territorio *atacante, Territorio *defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[ERRO] Não é possível atacar um território da mesma cor!\n");
        return;
    }

    if (atacante->tropas < 2) {
        printf("\n[ERRO] O território atacante precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }

    printf("\n--- Simulação do Ataque ---\n");
    int dadoAtacante = (rand() % 6) + 1; // Dado entre 1 e 6
    int dadoDefensor = (rand() % 6) + 1;

    printf("Dado do atacante (%s): %d\n", atacante->nome, dadoAtacante);
    printf("Dado do defensor (%s): %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\nO atacante venceu a batalha!\n");
        strcpy(defensor->cor, atacante->cor); // Mudança de dono
        defensor->tropas = atacante->tropas / 2; // Metade das tropas do atacante se movem
        atacante->tropas -= defensor->tropas; // Tropas saem do atacante
        printf("%s agora pertence ao exército %s!\n", defensor->nome, defensor->cor);
    } else {
        printf("\nO defensor resistiu ao ataque!\n");
        atacante->tropas -= 1; // Atacante perde 1 tropa
    }
}

/*
 Liberando a memória 
 */
void liberarMemoria(Territorio *mapa) {
    free(mapa);
    printf("\nMemória liberada com sucesso.\n");
}

/* 
FUNÇÃO PRINCIPAL
*/
int main() {
    srand(time(NULL)); // Garante aleatoriedade dos dados

    int qtdTerritorios;

    printf("=== Sistema de Territórios (WAR Estruturado) ===\n\n");
    printf("Informe a quantidade de territórios a serem cadastrados: ");
    scanf("%d", &qtdTerritorios);
    limparBuffer();

    // Alocação dinâmica para o vetor de territórios
    Territorio *mapa = (Territorio *) calloc(qtdTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Cadastro inicial
    cadastrarTerritorios(mapa, qtdTerritorios);

    int opcao;
    do {
        printf("\n=== Menu Principal ===\n");
        printf("1. Exibir territórios\n");
        printf("2. Realizar ataque\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                exibirTerritorios(mapa, qtdTerritorios);
                break;
            case 2: {
                int indiceAtacante, indiceDefensor;

                exibirTerritorios(mapa, qtdTerritorios);

                printf("\nEscolha o número do território ATACANTE: ");
                scanf("%d", &indiceAtacante);
                limparBuffer();

                printf("Escolha o número do território DEFENSOR: ");
                scanf("%d", &indiceDefensor);
                limparBuffer();

                if (indiceAtacante < 1 || indiceAtacante > qtdTerritorios ||
                    indiceDefensor < 1 || indiceDefensor > qtdTerritorios) {
                    printf("\n[ERRO] Índices inválidos!\n");
                } else {
                    atacar(&mapa[indiceAtacante - 1], &mapa[indiceDefensor - 1]);
                }
                break;
            }
            case 3:
                printf("\nSaindo do programa...\n");
                break;
            default:
                printf("\n[ERRO] Opção inválida!\n");
        }

    } while (opcao != 3);

    liberarMemoria(mapa);
    return 0;
}
