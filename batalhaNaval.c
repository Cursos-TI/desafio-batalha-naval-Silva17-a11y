#include <stdio.h>
#include <stdbool.h>

// Constantes do jogo
#define TAMANHO 10
#define TAMANHO_NAVIO 3
#define AGUA 0
#define NAVIO 3
#define HABILIDADE 5

// Tamanho das matrizes de habilidade
#define TAM_HABILIDADE 5

// Tipos de orientação para os navios
typedef enum {
    HORIZONTAL,
    VERTICAL,
    DIAGONAL_CIMA,
    DIAGONAL_BAIXO
} Orientacao;

// Tipos de habilidades
typedef enum {
    CONE,
    CRUZ,
    OCTAEDRO
} TipoHabilidade;

// Função para inicializar o tabuleiro com água
void inicializarTabuleiro(int tabuleiro[TAMANHO][TAMANHO]) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

// Função para verificar se a posição do navio é válida
bool posicaoValida(int linha, int coluna, Orientacao orientacao, int tabuleiro[TAMANHO][TAMANHO]) {
    // Verifica se o navio cabe no tabuleiro
    switch (orientacao) {
        case HORIZONTAL:
            if (coluna + TAMANHO_NAVIO > TAMANHO) return false;
            break;
        case VERTICAL:
            if (linha + TAMANHO_NAVIO > TAMANHO) return false;
            break;
        case DIAGONAL_CIMA:
            if (linha - TAMANHO_NAVIO < -1 || coluna + TAMANHO_NAVIO > TAMANHO) return false;
            break;
        case DIAGONAL_BAIXO:
            if (linha + TAMANHO_NAVIO > TAMANHO || coluna + TAMANHO_NAVIO > TAMANHO) return false;
            break;
    }
    
    // Verifica se as posições estão livres
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        int linha_atual = linha;
        int coluna_atual = coluna;
        
        switch (orientacao) {
            case HORIZONTAL:
                coluna_atual += i;
                break;
            case VERTICAL:
                linha_atual += i;
                break;
            case DIAGONAL_CIMA:
                linha_atual -= i;
                coluna_atual += i;
                break;
            case DIAGONAL_BAIXO:
                linha_atual += i;
                coluna_atual += i;
                break;
        }
        
        if (tabuleiro[linha_atual][coluna_atual] != AGUA) {
            return false;
        }
    }
    
    return true;
}

// Função para posicionar um navio no tabuleiro
void posicionarNavio(int tabuleiro[TAMANHO][TAMANHO], int linha, int coluna, Orientacao orientacao) {
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        int linha_atual = linha;
        int coluna_atual = coluna;
        
        switch (orientacao) {
            case HORIZONTAL:
                coluna_atual += i;
                break;
            case VERTICAL:
                linha_atual += i;
                break;
            case DIAGONAL_CIMA:
                linha_atual -= i;
                coluna_atual += i;
                break;
            case DIAGONAL_BAIXO:
                linha_atual += i;
                coluna_atual += i;
                break;
        }
        
        tabuleiro[linha_atual][coluna_atual] = NAVIO;
    }
}

// Função para criar a matriz de uma habilidade
void criarHabilidade(int habilidade[TAM_HABILIDADE][TAM_HABILIDADE], TipoHabilidade tipo) {
    // Inicializa toda a matriz com 0
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            habilidade[i][j] = 0;
        }
    }
    
    int centro = TAM_HABILIDADE / 2;
    
    switch (tipo) {
        case CONE:
            // Forma de cone apontando para baixo
            for (int i = 0; i < TAM_HABILIDADE; i++) {
                int largura = i + 1;
                int inicio = centro - i/2;
                for (int j = 0; j < largura && inicio + j < TAM_HABILIDADE; j++) {
                    if (inicio + j >= 0) {
                        habilidade[i][inicio + j] = 1;
                    }
                }
            }
            break;
            
        case CRUZ:
            // Forma de cruz
            for (int i = 0; i < TAM_HABILIDADE; i++) {
                habilidade[centro][i] = 1;  // Linha horizontal
                habilidade[i][centro] = 1;  // Linha vertical
            }
            break;
            
        case OCTAEDRO:
            // Forma de losango (octaedro visto de frente)
            for (int i = 0; i < TAM_HABILIDADE; i++) {
                int distancia = i <= centro ? i : TAM_HABILIDADE - 1 - i;
                for (int j = centro - distancia; j <= centro + distancia; j++) {
                    if (j >= 0 && j < TAM_HABILIDADE) {
                        habilidade[i][j] = 1;
                    }
                }
            }
            break;
    }
}

// Função para aplicar uma habilidade ao tabuleiro
void aplicarHabilidade(int tabuleiro[TAMANHO][TAMANHO], int habilidade[TAM_HABILIDADE][TAM_HABILIDADE], int linha_centro, int coluna_centro) {
    int offset = TAM_HABILIDADE / 2;
    
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            int linha_tab = linha_centro - offset + i;
            int coluna_tab = coluna_centro - offset + j;
            
            // Verifica se está dentro dos limites do tabuleiro
            if (linha_tab >= 0 && linha_tab < TAMANHO && coluna_tab >= 0 && coluna_tab < TAMANHO) {
                // Se a posição na matriz de habilidade for 1, marca no tabuleiro
                if (habilidade[i][j] == 1) {
                    // Não sobrescreve os navios, apenas a água
                    if (tabuleiro[linha_tab][coluna_tab] == AGUA) {
                        tabuleiro[linha_tab][coluna_tab] = HABILIDADE;
                    }
                }
            }
        }
    }
}

// Função para exibir o tabuleiro
void exibirTabuleiro(int tabuleiro[TAMANHO][TAMANHO]) {
    printf("\nTabuleiro de Batalha Naval com Habilidades:\n");
    printf("   ");
    for (int j = 0; j < TAMANHO; j++) {
        printf("%2d ", j); // Índices das colunas
    }
    printf("\n");
    
    for (int i = 0; i < TAMANHO; i++) {
        printf("%2d ", i); // Índice da linha
        for (int j = 0; j < TAMANHO; j++) {
            printf("%2d ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int tabuleiro[TAMANHO][TAMANHO];
    
    // Inicializa o tabuleiro com água
    inicializarTabuleiro(tabuleiro);
    
    // Posicionamento dos navios (coordenadas e orientações)
    // Navio 1 - Horizontal
    posicionarNavio(tabuleiro, 2, 3, HORIZONTAL);
    
    // Navio 2 - Vertical
    posicionarNavio(tabuleiro, 5, 7, VERTICAL);
    
    // Navio 3 - Diagonal para cima
    posicionarNavio(tabuleiro, 7, 1, DIAGONAL_CIMA);
    
    // Navio 4 - Diagonal para baixo
    posicionarNavio(tabuleiro, 1, 2, DIAGONAL_BAIXO);
    
    // Matrizes para as habilidades
    int cone[TAM_HABILIDADE][TAM_HABILIDADE];
    int cruz[TAM_HABILIDADE][TAM_HABILIDADE];
    int octaedro[TAM_HABILIDADE][TAM_HABILIDADE];
    
    // Criar as habilidades
    criarHabilidade(cone, CONE);
    criarHabilidade(cruz, CRUZ);
    criarHabilidade(octaedro, OCTAEDRO);
    
    // Aplicar as habilidades ao tabuleiro
    aplicarHabilidade(tabuleiro, cone, 4, 4);      // Cone no centro
    aplicarHabilidade(tabuleiro, cruz, 2, 7);      // Cruz perto do navio vertical
    aplicarHabilidade(tabuleiro, octaedro, 6, 2);  // Octaedro perto do navio diagonal
    
    // Exibe o tabuleiro com os navios e habilidades
    exibirTabuleiro(tabuleiro);
    
    return 0;
}