#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ------------------------------------------------------
// Estrutura que representa uma peça (tipo + identificador)
// ------------------------------------------------------
typedef struct {
    char nome; // Tipo: 'I', 'O', 'T', 'L'
    int id;    // ID único incremental
} Peca;

// ------------------------------------------------------
// Estrutura da Fila Circular
// ------------------------------------------------------
typedef struct {
    Peca pecas[TAM_FILA];
    int inicio;
    int fim;
    int qtd;
} Fila;

// ------------------------------------------------------
// Estrutura da Pilha (linear)
// ------------------------------------------------------
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// ------------------------------------------------------
// Variáveis globais de controle
// ------------------------------------------------------
char tipos[] = {'I', 'O', 'T', 'L'};
int contadorID = 0;

// ------------------------------------------------------
// Função: gerarPeca()
// Gera uma nova peça aleatória com ID único
// ------------------------------------------------------
Peca gerarPeca() {
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorID++;
    return nova;
}

// ------------------------------------------------------
// Inicializa a Fila Circular
// ------------------------------------------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        f->pecas[i] = gerarPeca();
        f->fim = (f->fim + 1) % TAM_FILA;
        f->qtd++;
    }
}

// ------------------------------------------------------
// Inicializa a Pilha
// ------------------------------------------------------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// ------------------------------------------------------
// Funções utilitárias de verificação
// ------------------------------------------------------
int filaVazia(Fila *f) { return f->qtd == 0; }
int filaCheia(Fila *f) { return f->qtd == TAM_FILA; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }

// ------------------------------------------------------
// Funções de manipulação da Fila e Pilha
// ------------------------------------------------------
void enfileirar(Fila *f, Peca p) {
    if (!filaCheia(f)) {
        f->pecas[f->fim] = p;
        f->fim = (f->fim + 1) % TAM_FILA;
        f->qtd++;
    }
}

Peca desenfileirar(Fila *f) {
    Peca removida = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return removida;
}

void empilhar(Pilha *p, Peca nova) {
    if (!pilhaCheia(p))
        p->pecas[++p->topo] = nova;
}

Peca desempilhar(Pilha *p) {
    return p->pecas[p->topo--];
}

// ------------------------------------------------------
// Exibição do estado atual da fila e pilha
// ------------------------------------------------------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n================= ESTADO ATUAL =================\n");
    printf("Fila de peças\t");
    int i = f->inicio;
    for (int c = 0; c < f->qtd; c++) {
        printf("[%c %d] ", f->pecas[i].nome, f->pecas[i].id);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");

    printf("Pilha de reserva\t(Topo -> base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
    printf("\n================================================\n");
}

// ------------------------------------------------------
// Operações principais do jogo
// ------------------------------------------------------
void jogarPeca(Fila *f) {
    if (filaVazia(f)) {
        printf("❌ Fila vazia!\n");
        return;
    }
    Peca jogada = desenfileirar(f);
    printf("✅ Peça [%c %d] jogada!\n", jogada.nome, jogada.id);

    // Mantém a fila sempre cheia
    enfileirar(f, gerarPeca());
}

void reservarPeca(Fila *f, Pilha *p) {
    if (pilhaCheia(p)) {
        printf("⚠️ Pilha cheia! Não é possível reservar.\n");
        return;
    }
    Peca movida = desenfileirar(f);
    empilhar(p, movida);
    printf("🔒 Peça [%c %d] movida para reserva.\n", movida.nome, movida.id);

    // Mantém a fila cheia
    enfileirar(f, gerarPeca());
}

void usarReservada(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("⚠️ Nenhuma peça reservada!\n");
        return;
    }
    Peca usada = desempilhar(p);
    printf("🎮 Peça reservada [%c %d] usada!\n", usada.nome, usada.id);
}

void trocarTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("⚠️ Não há peças suficientes para troca!\n");
        return;
    }

    Peca *frenteFila = &f->pecas[f->inicio];
    Peca *topoPilha = &p->pecas[p->topo];

    Peca temp = *frenteFila;
    *frenteFila = *topoPilha;
    *topoPilha = temp;

    printf("🔁 Troca realizada entre a frente da fila e o topo da pilha!\n");
}

void trocaMultipla(Fila *f, Pilha *p) {
    if (p->topo != 2 || f->qtd < 3) {
        printf("⚠️ São necessárias exatamente 3 peças na pilha e ao menos 3 na fila!\n");
        return;
    }

    int idxFila = f->inicio;
    for (int i = 0; i < 3; i++) {
        Peca temp = f->pecas[idxFila];
        f->pecas[idxFila] = p->pecas[p->topo - i];
        p->pecas[p->topo - i] = temp;
        idxFila = (idxFila + 1) % TAM_FILA;
    }

    printf("🔄 Troca múltipla entre as 3 primeiras peças da fila e as 3 da pilha!\n");
}

// ------------------------------------------------------
// Função principal (menu interativo)
// ------------------------------------------------------
int main() {
    srand(time(NULL));
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;
    do {
        exibirEstado(&fila, &pilha);
        printf("\n====== MENU DE AÇÕES ======\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para reserva\n");
        printf("3 - Usar peça da reserva\n");
        printf("4 - Trocar peça da frente com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com os 3 da pilha\n");
        printf("0 - Sair\n");
        printf("===========================\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: jogarPeca(&fila); break;
            case 2: reservarPeca(&fila, &pilha); break;
            case 3: usarReservada(&pilha); break;
            case 4: trocarTopo(&fila, &pilha); break;
            case 5: trocaMultipla(&fila, &pilha); break;
            case 0: printf("Encerrando o programa...\n"); break;
            default: printf("❌ Opção inválida!\n");
        }

    } while (opcao != 0);

    // 🔄 Nível Mestre: Integração Estratégica entre Fila e Pilha
    //
    // - Implemente interações avançadas entre as estruturas:
    //      4 - Trocar a peça da frente da fila com o topo da pilha
    //      5 - Trocar os 3 primeiros da fila com as 3 peças da pilha
    // - Para a opção 4:
    //      Verifique se a fila não está vazia e a pilha tem ao menos 1 peça.
    //      Troque os elementos diretamente nos arrays.
    // - Para a opção 5:
    //      Verifique se a pilha tem exatamente 3 peças e a fila ao menos 3.
    //      Use a lógica de índice circular para acessar os primeiros da fila.
    // - Sempre valide as condições antes da troca e informe mensagens claras ao usuário.
    // - Use funções auxiliares, se quiser, para modularizar a lógica de troca.
    // - O menu deve ficar assim:
    //      4 - Trocar peça da frente com topo da pilha
    //      5 - Trocar 3 primeiros da fila com os 3 da pilha

    return 0;
}
