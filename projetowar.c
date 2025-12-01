#include <stdio.h>
#include <stdlib.h> // Para malloc, free, rand, srand
#include <string.h> // Para strcmp, strcpy, strstr
#include <time.h>   // Para time

// --- Constantes ---
#define MAX_DADO 6       // Valor máximo do dado
#define MAX_NOME 30      // Tamanho máximo do nome do território
#define MAX_COR 10       // Tamanho máximo da cor do exército
#define MAX_MISSAO 100   // Tamanho máximo da string de missão
#define NUM_JOGADORES 2  // Número fixo de jogadores (VERMELHO e AZUL)

// Vetor de strings com as missões pré-definidas
const char* missoesEstrategicas[] = {
    "Dominar 6 territorios e ter pelo menos 2 tropas em cada.",
    "Eliminar todas as tropas da cor AZUL.",        
    "Dominar 10 territorios no total.",              
    "Conquistar 3 territorios seguidos na mesma rodada.",
    "Ter mais da metade dos territorios e todas as tropas inimigas eliminadas."
};
const int TOTAL_MISSOES = 5;

// --- Estruturas ---

// Estrutura para representar um Território
typedef struct {
    char nome[MAX_NOME]; 
    char cor[MAX_COR];   
    int tropas;         
} Territorio;

// Estrutura para representar um Jogador
typedef struct {
    char* missao;   
    char cor[MAX_COR]; 
} Jogador;

// --- Variável Global para o tamanho do array de Territórios ---
int numTerritorios = 0;

// --- Protótipos das Funções ---
Territorio* alocarMemoriaTerritorios();
Jogador* alocarMemoriaJogadores();
void liberarMemoria(Territorio* mapa, Jogador* jogadores);
void cadastrarTerritorios(Territorio* mapa);
int rolarDado();
void atacar(Territorio* atacante, Territorio* defensor);
void simularTurno(Territorio* mapa, Jogador* jogadores);
void exibirMapa(Territorio* mapa);
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
void exibirMissao(const char* missao);
int verificarMissao(const char* missao, Territorio* mapa, const char* corJogador);


// ====================================================================
// --- FUNÇÃO PRINCIPAL (MAIN) ---
// ====================================================================
int main() {
    srand(time(NULL));
    
    Territorio* listaTerritorios = NULL; 
    Jogador* listaJogadores = NULL;
    
    printf("🎮 WAR ESTRUTURADO FINAL 🎮\n");
    printf("------------------------------\n");
    
    // 1. Alocação de Territórios
    listaTerritorios = alocarMemoriaTerritorios();
    if (listaTerritorios == NULL) return 1;
    
    // 2. Alocação dos Jogadores e suas Missões
    listaJogadores = alocarMemoriaJogadores();
    if (listaJogadores == NULL) { free(listaTerritorios); return 1; }

    // Define as cores dos jogadores (Fixas)
    strcpy(listaJogadores[0].cor, "VERMELHO");
    strcpy(listaJogadores[1].cor, "AZUL");

    // 3. Atribuição das Missões
    for (int i = 0; i < NUM_JOGADORES; i++) {
        atribuirMissao(listaJogadores[i].missao, missoesEstrategicas, TOTAL_MISSOES);
        printf("\n--- JOGADOR %d (%s) ---\n", i + 1, listaJogadores[i].cor);
        exibirMissao(listaJogadores[i].missao); 
    }

    // 4. Cadastro Inicial de Territórios
    cadastrarTerritorios(listaTerritorios);
    
    // 5. Simulação de Jogo (Turno de Ataques)
    simularTurno(listaTerritorios, listaJogadores);
    
    // 6. Liberação de Memória
    liberarMemoria(listaTerritorios, listaJogadores);
    
    return 0;
}

// ====================================================================
// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE GERENCIAMENTO DE MEMÓRIA ---
// ====================================================================

/**
 * Solicita o número de territórios e aloca o vetor dinamicamente com calloc.
 */
Territorio* alocarMemoriaTerritorios() {
    printf("Digite o numero total de territorios que deseja cadastrar: ");
    while (scanf("%d", &numTerritorios) != 1 || numTerritorios <= 0) {
        printf("Entrada invalida. Digite um numero positivo: ");
        while (getchar() != '\n'); 
    }
    Territorio* mapa = (Territorio*)calloc(numTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("\nERRO: Falha na alocacao de memoria para territorios.\n");
    }
    return mapa;
}

/**
 * Aloca o vetor de Jogadores e a string de missão para cada um usando malloc.
 */
Jogador* alocarMemoriaJogadores() {
    Jogador* jogadores = (Jogador*)malloc(NUM_JOGADORES * sizeof(Jogador));
    if (jogadores == NULL) {
        printf("ERRO: Falha na alocacao de memoria para jogadores.\n");
        return NULL;
    }
    
    for (int i = 0; i < NUM_JOGADORES; i++) {
        jogadores[i].missao = (char*)malloc(MAX_MISSAO * sizeof(char));
        if (jogadores[i].missao == NULL) {
            printf("ERRO: Falha na alocacao de memoria para a missao do jogador %d.\n", i + 1);
            for (int j = 0; j < i; j++) free(jogadores[j].missao);
            free(jogadores);
            return NULL;
        }
    }
    return jogadores;
}

/**
 * Libera toda a memória alocada dinamicamente (Territórios e Missões) usando free().
 */
void liberarMemoria(Territorio* mapa, Jogador* jogadores) {
    printf("\n\n--- GERENCIAMENTO DE MEMÓRIA ---\n");
    
    if (jogadores != NULL) {
        // 1. Libera a string de missão de cada jogador
        for (int i = 0; i < NUM_JOGADORES; i++) {
            free(jogadores[i].missao);
        }
        // 2. Libera o vetor de jogadores
        free(jogadores);
    }
    
    // 3. Libera o vetor de territórios
    if (mapa != NULL) {
        free(mapa);
    }
    
    printf("Memoria dos territorios e missoes liberada com sucesso.\n");
}

// ====================================================================
// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE MISSÃO ---
// ====================================================================

/**
 * Sorteia uma missão e copia a string para o ponteiro de destino (Passagem por Referência).
 */
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indiceSorteado]);
}

/**
 * Exibe a missão do jogador (Passagem por Valor - const char*).
 */
void exibirMissao(const char* missao) {
    printf("✅ Missao Secreta: %s\n", missao);
    printf("---------------------------\n");
}

/**
 * Verifica se a missão do jogador foi cumprida.
 */
int verificarMissao(const char* missao, Territorio* mapa, const char* corJogador) {
    int territoriosDoJogador = 0;
    int tropasInimigas = 0;
    char corInimiga[MAX_COR] = "";

    if (strcmp(corJogador, "VERMELHO") == 0) strcpy(corInimiga, "AZUL");
    else if (strcmp(corJogador, "AZUL") == 0) strcpy(corInimiga, "VERMELHO");

    for (int i = 0; i < numTerritorios; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            territoriosDoJogador++;
        } else if (strcmp(mapa[i].cor, corInimiga) == 0) {
            tropasInimigas += mapa[i].tropas;
        }
    }

    // Missão [0]: Dominar 6 territórios e ter pelo menos 2 tropas em cada.
    if (strstr(missao, "Dominar 6 territorios") != NULL) {
        if (territoriosDoJogador >= 6) {
            for (int i = 0; i < numTerritorios; i++) {
                if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas < 2) {
                    return 0;
                }
            }
            return 1; 
        }
    }

    // Missão [1]: Eliminar todas as tropas da cor AZUL.
    if (strstr(missao, "Eliminar todas as tropas da cor AZUL") != NULL && strcmp(corJogador, "VERMELHO") == 0) {
        return tropasInimigas == 0;
    }
    
    // Missão [2]: Dominar 10 territórios no total (Simplificada para > 70% dos territórios).
    if (strstr(missao, "Dominar 10 territorios") != NULL) {
        return territoriosDoJogador >= (numTerritorios * 0.7);
    }
    
    // Missão [4]: Ter mais da metade dos territórios e todas as tropas inimigas eliminadas.
    if (strstr(missao, "Ter mais da metade dos territorios") != NULL) {
        return (territoriosDoJogador > numTerritorios / 2) && (tropasInimigas == 0);
    }

    return 0;
}

// ====================================================================
// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE JOGO (INPUT/OUTPUT E BATALHA) ---
// ====================================================================

/**
 * Preenche o vetor de territórios, alternando o dono (cor) para os 2 jogadores.
 */
void cadastrarTerritorios(Territorio* mapa) {
    printf("\n\n**INÍCIO DO CADASTRO DE TERRITÓRIOS** \n");
    for (int i = 0; i < numTerritorios; i++) {
        Territorio* t = mapa + i; 
        printf("\nCadastro do Territorio %d de %d \n", i + 1, numTerritorios);
        printf("Nome (max. %d, sem espacos): ", MAX_NOME - 1);
        scanf("%29s", t->nome); 
        
        // Atribui a cor de forma alternada para 2 jogadores
        strcpy(t->cor, (i % 2 == 0) ? "VERMELHO" : "AZUL");
        printf("Cor atribuida: %s\n", t->cor);

        printf("Quantidade de Tropas (inteiro >= 1): ");
        while (scanf("%d", &t->tropas) != 1 || t->tropas <= 0) {
            printf("Entrada invalida. Tropas devem ser >= 1: ");
            while (getchar() != '\n');
        }
    }
    printf("\n**CADASTRO CONCLUÍDO!** \n");
}

/**
 * Simula a rolagem de um dado de 6 lados (1 a 6).
 */
int rolarDado() {
    return (rand() % MAX_DADO) + 1;
}

/**
 * Exibe o estado atual de todos os territórios.
 */
void exibirMapa(Territorio* mapa) {
    printf("\n--- MAPA ATUAL DOS TERRITÓRIOS ---\n");
    for (int i = 0; i < numTerritorios; i++) {
        printf("#%d. Nome: %s | Cor: %s | Tropas: %d\n", 
            i + 1, 
            (mapa + i)->nome, 
            (mapa + i)->cor, 
            (mapa + i)->tropas
        );
    }
    printf("--------------------------------\n");
}

/**
 * Simula um ataque com rolagem de dados e atualiza as tropas/dono.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante->tropas <= 1) {
        printf("\n🛑 ATENÇÃO: %s tem apenas 1 tropa e nao pode atacar.\n", atacante->nome);
        return;
    }
    
    int dadoAtacante = rolarDado();
    int dadoDefensor = rolarDado();
    
    printf("\n--- BATALHA: %s vs %s ---\n", atacante->nome, defensor->nome);
    printf("Dado do Atacante: %d | Dado do Defensor: %d\n", dadoAtacante, dadoDefensor);
    
    if (dadoAtacante > dadoDefensor) {
        printf("🎉 ATACANTE VENCEU! %s conquista %s.\n", atacante->nome, defensor->nome);
        
        // Conquista: Muda a cor e transfere metade das tropas
        strcpy(defensor->cor, atacante->cor);
        int tropasTransferidas = atacante->tropas / 2;
        atacante->tropas -= tropasTransferidas;
        defensor->tropas = tropasTransferidas; 
        
    } else { 
        printf("🛡️ DEFENSOR RESISTIU! %s perde 1 tropa.\n", atacante->nome);
        atacante->tropas--;
    }
}

/**
 * Gerencia a interatividade do turno (escolhas de ataque) e verifica a condição de vitória.
 */
void simularTurno(Territorio* mapa, Jogador* jogadores) {
    int idxAtacante, idxDefensor;
    char escolha = 's';
    int vencedor = -1;

    do {
        printf("\n\n=============== TURNO DE ATAQUES ==============\n");
        exibirMapa(mapa);
        
        printf("Escolha o TERRITÓRIO ATACANTE (numero #, 0 para encerrar): ");
        if (scanf("%d", &idxAtacante) != 1 || idxAtacante == 0) {
            break;
        }
        
        printf("Escolha o TERRITÓRIO DEFENSOR (numero #): ");
        if (scanf("%d", &idxDefensor) != 1) continue; 
        
        idxAtacante--; // Converte para índice 0-based
        idxDefensor--;
        
        // Validações
        if (idxAtacante < 0 || idxAtacante >= numTerritorios || 
            idxDefensor < 0 || idxDefensor >= numTerritorios || idxAtacante == idxDefensor) {
            printf("\n⚠️ ALERTA: Selecao de territorio invalida. Tente novamente.\n");
        } else if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
            printf("\n⚠️ ALERTA: Nao e possivel atacar um territorio da sua propria cor (%s).\n", mapa[idxAtacante].cor);
        } else {
            // Executa a batalha
            Territorio* atacante = mapa + idxAtacante;
            Territorio* defensor = mapa + idxDefensor;
            atacar(atacante, defensor);
            
            // ⭐️ VERIFICAÇÃO DA MISSÃO APÓS O ATAQUE ⭐️
            for (int i = 0; i < NUM_JOGADORES; i++) {
                if (verificarMissao(jogadores[i].missao, mapa, jogadores[i].cor)) {
                    vencedor = i;
                    goto fim_jogo;
                }
            }
        }
        
        printf("\nDeseja realizar outro ataque neste turno? (s/n): ");
        while (scanf(" %c", &escolha) != 1 || (escolha != 's' && escolha != 'n')) {
            printf("Escolha invalida. Digite 's' ou 'n': ");
            while (getchar() != '\n');
        }
        
    } while (escolha == 's');

fim_jogo:
    if (vencedor != -1) {
        printf("\n\n=======================================================\n");
        printf("!!! 👑 VITÓRIA POR MISSÃO! 👑 !!!\n");
        printf("O JOGADOR %d (%s) CUMPRIU SUA MISSÃO SECRETA:\n '%s'\n", 
            vencedor + 1, jogadores[vencedor].cor, jogadores[vencedor].missao);
        printf("=======================================================\n");
    } else {
        printf("\nFim da simulação. Nenhum jogador cumpriu a missao de vitoria neste turno.\n");
    }
}