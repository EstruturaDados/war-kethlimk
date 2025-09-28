#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/*
    WAR Missões Estratégicas
    Funcionalidades:
    - Cadastro de territórios 
    - Ataque entre territórios 
    - Sorteio de missões estratégicas para cada jogador
    - Verificação automática se um jogador venceu
    - Modularização e uso de ponteiros
*/

/* 
STRUCT TERRITÓRIO 
 */
typedef struct {
    char nome[30];
    char cor[30];
    int tropas;
} Territorio;

/*
FUNÇÕES AUXILIARES 
 */

// Remove os espaços extras no início/fim
void trim_whitespace(char *s) {
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1);

    char *end = s + strlen(s) - 1;
    while (end >= s && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
}

// Converte string para minúsculas
void to_lowercase(char *s) {
    for (size_t i = 0; s[i]; i++)
        s[i] = (char)tolower((unsigned char)s[i]);
}

// Normaliza string: trim + lowercase
void normalize(char *s) {
    trim_whitespace(s);
    to_lowercase(s);
}

// Leitura segura de texto
void read_line(const char *prompt, char *buffer, size_t size) {
    do {
        printf("%s", prompt);
        if (fgets(buffer, (int)size, stdin) == NULL) {
            buffer[0] = '\0';
        } else {
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n')
                buffer[len - 1] = '\0';
            trim_whitespace(buffer);
        }
    } while (buffer[0] == '\0'); // não permite vazio
}

// Leitura segura de inteiro
int read_int(const char *prompt) {
    char line[64];
    char *endptr;
    long val;
    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL)
            continue;
        trim_whitespace(line);
        if (line[0] == '\0')
            continue;
        val = strtol(line, &endptr, 10);
        if (*endptr == '\0')
            return (int)val;
        printf("Entrada inválida. Digite um número inteiro.\n");
    }
}

/* 
CADASTRO DE TERRITÓRIOS 
 */
void cadastrarTerritorios(Territorio *mapa, int qtd) {
    printf("\n=== Cadastro de Territórios ===\n");
    for (int i = 0; i < qtd; ++i) {
        printf("\nTerritório %d:\n", i + 1);
        read_line("  Nome do território: ", mapa[i].nome, sizeof(mapa[i].nome));
        read_line("  Cor do exército: ", mapa[i].cor, sizeof(mapa[i].cor));
        normalize(mapa[i].cor); // salva a cor normalizada
        mapa[i].tropas = read_int("  Quantidade de tropas: ");
    }
}

/* 
EXIBIR OS TERRITÓRIOS
 */
void exibirTerritorios(Territorio *mapa, int qtd) {
    printf("\n=== Situação Atual dos Territórios ===\n");
    for (int i = 0; i < qtd; ++i) {
        printf(" %2d) %s | cor: %s | tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("\n");
}

/* 
ATAQUE ENTRE OS TERRITÓRIOS 
 */
void atacar(Territorio *atacante, Territorio *defensor) {
    printf("\nTentativa de ataque: %s (%s) -> %s (%s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    // Validações
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("[ERRO] Não é possível atacar um território da mesma cor!\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("[ERRO] O atacante precisa ter pelo menos 2 tropas para atacar!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    printf("Rolagens -> Atacante: %d | Defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor); // troca a cor do território
        int tropasMovidas = atacante->tropas / 2;
        if (tropasMovidas < 1) tropasMovidas = 1;
        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas;
        printf("%s agora pertence ao exército %s!\n", defensor->nome, defensor->cor);
    } else {
        printf("Defensor resistiu. Atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

/* 
Atribui missão aleatória a um jogador
*/


void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Exibe a missão do jogador
void exibirMissao(const char *nomeJogador, const char *missao) {
    printf("\nMissão do jogador %s:\n", nomeJogador);
    printf("  %s\n", missao);
}

/*
    Lógica simples para verificar missão:
    - "Conquistar 3 territórios seguidos": verifica se o jogador controla 3 territórios consecutivos
    - "Eliminar todas as tropas da cor vermelha": verifica se não existe nenhum território vermelho
*/
int verificarMissao(const char *missao, Territorio *mapa, int qtd, const char *corJogador) {
    if (strstr(missao, "Conquistar 3 territórios") != NULL) {
        int cont = 0;
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                cont++;
                if (cont == 3) return 1; // missão cumprida
            } else {
                cont = 0;
            }
        }
        return 0;
    }

    if (strstr(missao, "Eliminar todas as tropas da cor vermelha") != NULL) {
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0 && mapa[i].tropas > 0)
                return 0; // ainda existe vermelho
        }
        return 1; // missão cumprida
    }

    return 0; // missão não reconhecida ou não cumprida
}

/* 
LIBERAR MEMÓRIA 
*/
void liberarMemoria(Territorio *mapa, char *missao1, char *missao2) {
    free(mapa);
    free(missao1);
    free(missao2);
    printf("Memória liberada.\n");
}

/* 
PROGRAMA PRINCIPAL 
*/
int main(void) {
    srand((unsigned) time(NULL));

    printf("=== Sistema WAR com Missões ===\n");

    // Cadastro de territórios
    int qtd = read_int("Informe a quantidade de territórios: ");
    if (qtd <= 0) {
        printf("Quantidade inválida.\n");
        return 1;
    }

    Territorio *mapa = (Territorio *) calloc((size_t)qtd, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória para territórios.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, qtd);

    // Missões pré-definidas
    char *missoes[] = {
        "Conquistar 3 territórios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar metade do mapa",
        "Dominar todos os territórios verdes",
        "Ter ao menos 20 tropas em um único território"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // Atribuir missões para dois jogadores
    char *missaoJogador1 = (char *) malloc(100 * sizeof(char));
    char *missaoJogador2 = (char *) malloc(100 * sizeof(char));

    if (!missaoJogador1 || !missaoJogador2) {
        printf("Erro ao alocar memória para missões.\n");
        free(mapa);
        return 1;
    }

    atribuirMissao(missaoJogador1, missoes, totalMissoes);
    atribuirMissao(missaoJogador2, missoes, totalMissoes);

    printf("\nMissões foram sorteadas!\n");
    exibirMissao("Jogador 1", missaoJogador1);
    exibirMissao("Jogador 2", missaoJogador2);

    // Loop principal
    int turno = 1;
    int opcao, venceu = 0;
    char corJogador1[30], corJogador2[30];

    // Definir cores dos jogadores
    read_line("\nDigite a cor do exército do Jogador 1: ", corJogador1, sizeof(corJogador1));
    normalize(corJogador1);

    read_line("Digite a cor do exército do Jogador 2: ", corJogador2, sizeof(corJogador2));
    normalize(corJogador2);

    do {
        printf("\n=== Turno %d ===\n", turno);
        exibirTerritorios(mapa, qtd);

        printf("1) Atacar\n2) Passar turno\n3) Sair do jogo\n");
        opcao = read_int("Escolha uma opção: ");

        if (opcao == 1) {
            int a = read_int("Território atacante: ");
            int d = read_int("Território defensor: ");

            if (a < 1 || a > qtd || d < 1 || d > qtd || a == d) {
                printf("[ERRO] Índices inválidos.\n");
            } else {
                atacar(&mapa[a-1], &mapa[d-1]);
            }
        }

        // Verificação de missões
        if (verificarMissao(missaoJogador1, mapa, qtd, corJogador1)) {
            printf("\n*** Jogador 1 venceu a partida! ***\n");
            venceu = 1;
        } else if (verificarMissao(missaoJogador2, mapa, qtd, corJogador2)) {
            printf("\n*** Jogador 2 venceu a partida! ***\n");
            venceu = 1;
        }

        turno++;
    } while (opcao != 3 && venceu == 0);

    liberarMemoria(mapa, missaoJogador1, missaoJogador2);
    return 0;
}