#include "TAD.h"

#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do nó AVL
typedef struct Horario {
    int diaSemana;
    int horario;
    int altura;
    struct Horario* esquerda;
    struct Horario* direita;
} Horario;

// Função para obter a altura de um nó
int altura(Horario* N) {
    if (N == NULL)
        return 0;
    return N->altura;
}

// Função que retorna o maior de dois números
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para criar um novo nó AVL
Horario* novoHorario(int dia, int hora) {
    Horario* horario = (Horario*)malloc(sizeof(Horario));
    horario->diaSemana = dia;
    horario->horario = hora;
    horario->esquerda = NULL;
    horario->direita = NULL;
    horario->altura = 1; // Novo nó será inicialmente adicionado na folha
    return(horario);
}

// Função para fazer uma rotação simples à direita
Horario* rotacaoDireita(Horario* y) {
    Horario* x = y->esquerda;
    Horario* T2 = x->direita;

    // Realizando a rotação
    x->direita = y;
    y->esquerda = T2;

    // Atualizando as alturas
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

    // Retornando nova raiz
    return x;
}

// Função para fazer uma rotação simples à esquerda
Horario* rotacaoEsquerda(Horario* x) {
    Horario* y = x->direita;
    Horario* T2 = y->esquerda;

    // Realizando a rotação
    y->esquerda = x;
    x->direita = T2;

    // Atualizando as alturas
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

    // Retornando nova raiz
    return y;
}

// Função para obter o fator de balanceamento de um nó
int getBalance(Horario* N) {
    if (N == NULL)
        return 0;
    return altura(N->esquerda) - altura(N->direita);
}

// Função para inserir um novo horário na árvore AVL
Horario* adicionarHorario(Horario* node, int dia, int hora) {
    // Passo 1: Realizar a inserção normal de uma BST
    if (node == NULL)
        return(novoHorario(dia, hora));

    if (dia < node->diaSemana || (dia == node->diaSemana && hora < node->horario))
        node->esquerda = adicionarHorario(node->esquerda, dia, hora);
    else if (dia > node->diaSemana || (dia == node->diaSemana && hora > node->horario))
        node->direita = adicionarHorario(node->direita, dia, hora);
    else // Se o horário já existir, não faz nada
        return node;

    // Passo 2: Atualizar a altura deste nó ancestral
    node->altura = 1 + max(altura(node->esquerda), altura(node->direita));

    // Passo 3: Obter o fator de balanceamento deste nó ancestral
    int balance = getBalance(node);

    // Passo 4: Se o nó se tornar desbalanceado, há 4 casos

    // Caso Esquerda-Esquerda
    if (balance > 1 && dia < node->esquerda->diaSemana)
        return rotacaoDireita(node);

    // Caso Direita-Direita
    if (balance < -1 && dia > node->direita->diaSemana)
        return rotacaoEsquerda(node);

    // Caso Esquerda-Direita
    if (balance > 1 && dia > node->esquerda->diaSemana) {
        node->esquerda = rotacaoEsquerda(node->esquerda);
        return rotacaoDireita(node);
    }

    // Caso Direita-Esquerda
    if (balance < -1 && dia < node->direita->diaSemana) {
        node->direita = rotacaoDireita(node->direita);
        return rotacaoEsquerda(node);
    }

    // Retornar o nó (inalterado)
    return node;
}

// Função para verificar colisão de horário
int verificarHorario(Horario* node, int dia, int hora) {
    if (node == NULL)
        return 0;  // Não há colisão

    if (dia == node->diaSemana && hora == node->horario)
        return 1;  // Colisão de horário

    if (dia < node->diaSemana || (dia == node->diaSemana && hora < node->horario))
        return verificarHorario(node->esquerda, dia, hora);

    return verificarHorario(node->direita, dia, hora);
}

// Função para mostrar os horários agendados (em ordem crescente)
void mostrarHorarios(Horario* root) {
    if (root != NULL) {
        mostrarHorarios(root->esquerda);
        printf("Dia %d, Horario: %d:00\n", root->diaSemana, root->horario);
        mostrarHorarios(root->direita);
    }
}

void marcarHorario(Horario** root) {
    int horario, dia;

    printf("\nVoce selecionou 'Marcar o horario'.\n");

    // Solicitar o dia da semana
    printf("Insira o dia da semana (1 = Segunda-feira, ..., 7 = Domingo): ");
    scanf("%d", &dia);

    // Validar o dia da semana
    while (dia < 1 || dia > 7) {
        printf("Dia invalido. Insira um valor entre 1 (Segunda) e 7 (Domingo): ");
        scanf("%d", &dia);
    }

    // Solicitar o horário
    printf("Insira o horario desejado (apenas horarios fechados ex: 14 para 14hrs): ");
    scanf("%d", &horario);

    // Verificar se o horário é válido e se já está ocupado
    while (horario < 8 || horario > 18 || verificarHorario(*root, dia, horario)) {
        if (horario < 8 || horario > 18) {
            printf("Horario invalido. Funcionamos das 8hrs as 18hrs.\n");
        } else {
            printf("Horario ja marcado para esse dia. Escolha outro horario.\n");
        }

        // Solicitar um novo horário
        printf("Insira o horario desejado (apenas horarios fechados ex: 14 para 14hrs): ");
        scanf("%d", &horario);
    }

    // Se o horário estiver livre e válido, adiciona na árvore
    *root = adicionarHorario(*root, dia, horario);
    printf("Horario marcado para %d:00 no dia %d.\n", horario, dia);
}

void entrarServicos() {
    int opcaoServico;

    do {
        listarServicos(); // Exibe a lista de serviços
        printf("Selecione um serviço para mais detalhes: ");
        scanf("%d", &opcaoServico);

        switch (opcaoServico) {
            case 1:
                printf("Manicure: Tratamento completo para as unhas das mãos.\n");
                break;
            case 2:
                printf("Pedicure: Tratamento completo para as unhas dos pés.\n");
                break;
            case 3:
                printf("Spa das mãos: Tratamento de esfoliação e hidratação das mãos.\n");
                break;
            case 4:
                printf("Alongamento de unhas: Técnica de alongamento com gel ou acrílico.\n");
                break;
            case 5:
                printf("Nail Art: Decoração artística nas unhas.\n");
                break;
            case 6:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcaoServico != 6); // Continua no submenu até selecionar "Voltar"
}

void profissionaisDisponiveis() {
    printf("Você selecionou 'Profissionais disponíveis'.\n");
    if (totalFuncionarios == 0) {
        printf("Nenhum funcionário cadastrado.\n");
    } else {
        for (int i = 0; i < totalFuncionarios; i++) {
            printf("%d. %s - %s\n", i + 1, funcionarios[i].nome, funcionarios[i].cargo);
        }
    }
}

    if (totalFuncionarios < MAX_FUNCIONARIOS) {
        printf("Você selecionou 'Cadastrar novo funcionário'.\n");
        printf("Digite o nome do novo funcionário: ");
        scanf("%s", funcionarios[totalFuncionarios].nome);
        printf("Digite o cargo do novo funcionário: ");
        scanf("%s", funcionarios[totalFuncionarios].cargo);
        totalFuncionarios++;
        printf("Funcionário cadastrado com sucesso.\n");
    } else {
        printf("Número máximo de funcionários atingido.\n");
    }
}

void cadastrarFuncionario() {
    if (totalFuncionarios < MAX_FUNCIONARIOS) {
        printf("Você selecionou 'Cadastrar novo funcionário'.\n");
        printf("Digite o nome do novo funcionário: ");
        scanf("%s", funcionarios[totalFuncionarios].nome);
        printf("Digite o cargo do novo funcionário: ");
        scanf("%s", funcionarios[totalFuncionarios].cargo);
        totalFuncionarios++;
        printf("Funcionário cadastrado com sucesso.\n");
    } else {
        printf("Número máximo de funcionários atingido.\n");
    }
}

void atualizarFuncionario() {
    if (totalFuncionarios == 0) {
        printf("Nenhum funcionário cadastrado.\n");
        return;
    }
    
    printf("Você selecionou 'Atualizar funcionário'.\n");
    profissionaisDisponiveis(); // Exibe a lista de funcionários
    
    int index;
    printf("Digite o número do funcionário que deseja atualizar: ");
    scanf("%d", &index);
    
    if (index < 1 || index > totalFuncionarios) {
        printf("Funcionário inválido.\n");
        return;
    }

    index--; // Ajusta o índice para o array

    // Exibe opções para atualização
    int opcaoAtualizar;
    printf("O que deseja atualizar?\n");
    printf("1 - Nome\n");
    printf("2 - Cargo\n");
    printf("3 - Ambos\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcaoAtualizar);

    switch (opcaoAtualizar) {
        case 1:
            printf("Digite o novo nome: ");
            scanf("%s", funcionarios[index].nome);
            printf("Nome atualizado com sucesso.\n");
            break;
        case 2:
            printf("Digite o novo cargo: ");
            scanf("%s", funcionarios[index].cargo);
            printf("Cargo atualizado com sucesso.\n");
            break;
        case 3:
            printf("Digite o novo nome: ");
            scanf("%s", funcionarios[index].nome);
            printf("Digite o novo cargo: ");
            scanf("%s", funcionarios[index].cargo);
            printf("Nome e cargo atualizados com sucesso.\n");
            break;
        default:
            printf("Opção inválida.\n");
    }
}

void maisInformacoes() {
    printf("\n===== Mais informações =====\n");
    printf("Informações sobre o sistema:\n");
    printf("Versão: 1.0.0\n");
    printf("Desenvolvido por: @Unhas de Encanto 2024\n");
    printf("Contato: unhasdencanto@gmail.com\n");
    printf("Horário de funcionamento: 8h - 18h (Segunda a Sábado)\n");
    printf("Endereço: Rua das Flores, 123, Centro\n");
}

int main() {
    Horario* arvoreHorarios = NULL;
    int opcao;

    do {
        printf("\n===== MENU =====\n");
        printf("1 - Marcar o horario\n");
        printf("2 - Mostrar horarios\n");
        printf("3 - Servicos\n");
        printf("4 - Profissionais disponiveis\n");
        printf("5 - Cadastrar novo funcionario\n");
        printf("6 - Atualizar funcionario\n");
        printf("7 - Mais informacoes\n");
        printf("8 - Sair\n");
        printf("================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                marcarHorario(&arvoreHorarios);
                break;
            case 2:
                mostrarHorarios(arvoreHorarios);
                break;
            case 3:
                entrarServicos();
                break;
            case 4:
                profissionaisDisponiveis();
                break;
            case 5:
                cadastrarFuncionario();
                break;
            case 6:
                atualizarFuncionario();
                break;
            case 7:
                maisInformacoes();
                break;
            case 8:
                printf("Volte sempre <3...\n");
                break;
            default:
                printf("Opcao invalida, tente novamente.\n");
        }
    } while (opcao != 8);

    return 0;
}
