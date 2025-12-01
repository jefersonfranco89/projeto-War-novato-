#include <stdio.h>
#include <string.h>

// Constante que define o número de territórios a serem cadastrados.
// Garante manutenibilidade, pois o número pode ser alterado facilmente.
#define NUM_TERRITORIOS 5

// 1. Definição da struct: Territorio
// Estrutura composta para agrupar as informações de um único território.
// (Requisito Funcional: Criação da struct)
typedef struct {
    char nome[30];  // Nome do território (máximo 29 caracteres + '\0')
    char cor[10];   // Cor do exército
    int tropas;     // Quantidade de tropas
} Territorio;

int main() {
    // 2. Declaração de vetor de structs
    // Cria um array capaz de armazenar 5 estruturas do tipo Territorio.
    Territorio listaTerritorios[NUM_TERRITORIOS];
    
    int i; 

    printf("**CADASTRO DE TERRITÓRIOS** \n");
    printf("------------------------------------\n");

    // 3. Entrada dos dados (Cadastro dos territórios)
    // O laço 'for' itera NUM_TERRITORIOS vezes para preencher o vetor.
    // (Requisito Funcional: Cadastro)
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\nCadastro do Territorio %d de %d \n", i + 1, NUM_TERRITORIOS);
        
        // --- Entrada do NOME ---
        // Usando scanf com limitação (%29s) para garantir a segurança e evitar buffer overflow.
        printf("Nome do Territorio (máx. 29 caracteres, sem espaços): ");
        scanf("%29s", listaTerritorios[i].nome);
        
        // --- Entrada da COR ---
        printf("Cor do Exercito (ex: Vermelho): ");
        scanf("%9s", listaTerritorios[i].cor); // Usando %9s para string de cor

        // --- Entrada da QUANTIDADE DE TROPAS ---
        printf("Quantidade de Tropas (número inteiro): ");
        scanf("%d", &listaTerritorios[i].tropas);
        
        // Observação: O uso exclusivo de scanf para todas as entradas evita a maioria dos problemas de buffer
        // que ocorreriam se misturasse scanf e fgets, garantindo Usabilidade simples.
    }
    
    printf("\n\n CADASTRO CONCLUÍDO! EXIBINDO DADOS... \n");
    printf("--------------------------------------------------\n");

    // 4. Exibição dos dados
    // O laço 'for' percorre o vetor e exibe os dados de cada Territorio,
    // acessando os membros da struct com o operador ponto (.).
    // (Requisito Funcional: Exibição dos dados)
    // (Requisito Não Funcional: Desempenho - Exibição imediata após o cadastro)
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("--- TERRITÓRIO #%d ---\n", i + 1);
        printf("Nome: %s\n", listaTerritorios[i].nome);
        printf("Cor: %s\n", listaTerritorios[i].cor);
        printf("Tropas: %d\n", listaTerritorios[i].tropas);
    }

    printf("\n**Fim da listagem de territórios.**\n");

    return 0;
}