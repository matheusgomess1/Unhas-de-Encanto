//#include "/home/neto/Área de trabalho/Unhas-de-Encanto/include/TAD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_FUNCIONARIOS 10 // Máximo de funcionários

// Definição de estruturas
typedef struct funcionario {
    char nome[50];
    char cargo[50];
} Funcionario;

typedef struct horario {
    int diaSemana;
    int horario;
    int altura;
    struct horario* esquerda;
    struct horario* direita;
} Horario;

Horario* adicionarHorario(Horario* node, int dia, int hora); // Protótipo da função adicionarHorario
void salvarHorarios(Horario* root, FILE* arquivo);
void salvarFuncionarios();
Horario* carregarHorarios(Horario* root);
void carregarFuncionarios();
int altura(Horario* N);
int max(int a, int b);
Horario* novoHorario(int dia, int hora);
Horario* rotacaoDireita(Horario* y);
Horario* rotacaoEsquerda(Horario* x);
int getBalance(Horario* N);
int verificarHorario(Horario* node, int dia, int hora);
void mostrarHorarios(Horario* root);
void listarServicos();
void marcarHorario(Horario** root);
void cadastrarFuncionario();
void profissionaisDisponiveis();


// Variáveis globais
Funcionario funcionarios[MAX_FUNCIONARIOS];
int totalFuncionarios = 0;

// Função para salvar os horários agendados em um arquivo
void salvarHorarios(Horario* root, FILE* arquivo) {
    if (root != NULL) {
        salvarHorarios(root->esquerda, arquivo);
        fprintf(arquivo, "%d %d\n", root->diaSemana, root->horario); // Salva dia e horário no arquivo
        salvarHorarios(root->direita, arquivo);
    }
}

// Função para salvar os funcionários em um arquivo
void salvarFuncionarios() {
    FILE* arquivo = fopen("funcionarios.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os funcionários!\n");
        return;
    }

    for (int i = 0; i < totalFuncionarios; i++) {
        fprintf(arquivo, "%s %s\n", funcionarios[i].nome, funcionarios[i].cargo);
    }

    fclose(arquivo);
    printf("Funcionários salvos com sucesso!\n");
}

// Função para carregar os horários de um arquivo para a árvore AVL
Horario* carregarHorarios(Horario* root) {
    FILE* arquivo = fopen("horarios.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum horário salvo encontrado.\n");
        return root;
    }

    int dia, hora;
    while (fscanf(arquivo, "%d %d", &dia, &hora) != EOF) {
        root = adicionarHorario(root, dia, hora); // Adiciona cada horário na árvore AVL
    }

    fclose(arquivo);
    printf("Horários carregados com sucesso!\n");
    return root;
}

// Função para carregar os funcionários de um arquivo
void carregarFuncionarios() {
    FILE* arquivo = fopen("funcionarios.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum funcionário salvo encontrado.\n");
        return;
    }

    totalFuncionarios = 0;
    while (fscanf(arquivo, "%s %s", funcionarios[totalFuncionarios].nome, funcionarios[totalFuncionarios].cargo) != EOF) {
        totalFuncionarios++;
        if (totalFuncionarios >= MAX_FUNCIONARIOS)
            break;
    }

    fclose(arquivo);
    printf("Funcionários carregados com sucesso!\n");
}

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
    if (node == NULL)
        return(novoHorario(dia, hora));

    if (dia < node->diaSemana || (dia == node->diaSemana && hora < node->horario))
        node->esquerda = adicionarHorario(node->esquerda, dia, hora);
    else if (dia > node->diaSemana || (dia == node->diaSemana && hora > node->horario))
        node->direita = adicionarHorario(node->direita, dia, hora);
    else // Se o horário já existir, não faz nada
        return node;

    // Atualizar a altura deste nó ancestral
    node->altura = 1 + max(altura(node->esquerda), altura(node->direita));

    // Obter o fator de balanceamento deste nó ancestral
    int balance = getBalance(node);

    // Realizar rotações de balanceamento se necessário
    if (balance > 1 && dia < node->esquerda->diaSemana)
        return rotacaoDireita(node);
    if (balance < -1 && dia > node->direita->diaSemana)
        return rotacaoEsquerda(node);
    if (balance > 1 && dia > node->esquerda->diaSemana) {
        node->esquerda = rotacaoEsquerda(node->esquerda);
        return rotacaoDireita(node);
    }
    if (balance < -1 && dia < node->direita->diaSemana) {
        node->direita = rotacaoDireita(node->direita);
        return rotacaoEsquerda(node);
    }

    return node;
}

// Restante do código principal...


// Função para verificar colisão de horário
int verificarHorario(Horario* node, int dia, int hora) {
    if (node == NULL)
        return 0;

    if (dia == node->diaSemana && hora == node->horario)
        return 1;

    if (dia < node->diaSemana || (dia == node->diaSemana && hora < node->horario))
        return verificarHorario(node->esquerda, dia, hora);

    return verificarHorario(node->direita, dia, hora);
}

// Função para mostrar os horários agendados
void mostrarHorarios(Horario* root) {
    if (root != NULL) {
        mostrarHorarios(root->esquerda);
        printf("Dia %d, Horario: %d:00\n", root->diaSemana, root->horario);
        mostrarHorarios(root->direita);
    }
}


// Função para listar os serviços disponíveis
void listarServicos() {
    printf("\n===== LISTA DE SERVIÇOS =====\n");
    printf("1 - Manicure\n");
    printf("2 - Pedicure\n");
    printf("3 - Spa das Mãos\n");
    printf("4 - Alongamento de Unhas\n");
    printf("5 - Nail Art\n");
    printf("6 - Voltar ao Menu\n");
    printf("==============================\n");
}

// Função para agendar um horário
void marcarHorario(Horario** root) {
    int horario, dia;

    printf("\nVocê selecionou 'Marcar o horário'.\n");

    // Solicitar o dia da semana
    printf("Insira o dia da semana (1 = Segunda-feira, ..., 7 = Domingo): ");
    scanf("%d", &dia);

    // Validar o dia da semana
    while (dia < 1 || dia > 7) {
        printf("Dia inválido. Insira um valor entre 1 (Segunda) e 7 (Domingo): ");
        scanf("%d", &dia);
    }

    // Solicitar o horário
    printf("Insira o horário desejado (8 - 18 horas): ");
    scanf("%d", &horario);

    // Verificar se o horário é válido e se já está ocupado
    while (horario < 8 || horario > 18 || verificarHorario(*root, dia, horario)) {
        if (horario < 8 || horario > 18) {
            printf("Horário inválido. Funcionamos das 8hrs às 18hrs.\n");
        } else {
            printf("Horário já marcado para esse dia. Escolha outro horário.\n");
        }
        printf("Insira o horário desejado (8 - 18 horas): ");
        scanf("%d", &horario);
    }

    // Adicionar o horário na árvore
    *root = adicionarHorario(*root, dia, horario);
    printf("Horário marcado para %d:00 no dia %d.\n", horario, dia);
}

// Função para cadastrar um novo funcionário
void cadastrarFuncionario() {
    if (totalFuncionarios < MAX_FUNCIONARIOS) {
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

// Função para exibir os profissionais disponíveis
void profissionaisDisponiveis() {
    printf("Profissionais disponíveis:\n");
    if (totalFuncionarios == 0) {
        printf("Nenhum funcionário cadastrado.\n");
    } else {
        for (int i = 0; i < totalFuncionarios; i++) {
            printf("%d. %s - %s\n", i + 1, funcionarios[i].nome, funcionarios[i].cargo);
        }
    }
}

// Função principal
int main() {
    Horario* arvoreHorarios = NULL;
    int opcao;

    // Carrega os dados salvos anteriormente
    carregarFuncionarios();
    arvoreHorarios = carregarHorarios(arvoreHorarios);

    do {
        printf("\n===== MENU =====\n");
        printf("1 - Marcar horário\n");
        printf("2 - Mostrar horários\n");
        printf("3 - Serviços\n");
        printf("4 - Profissionais disponíveis\n");
        printf("5 - Cadastrar novo funcionário\n");
        printf("6 - Salvar dados\n");
        printf("7 - Sair\n");
        printf("================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                marcarHorario(&arvoreHorarios);
                break;
            case 2:
                mostrarHorarios(arvoreHorarios);
                break;
            case 3:
                listarServicos();
                break;
            case 4:
                profissionaisDisponiveis();
                break;
            case 5:
                cadastrarFuncionario();
                break;
            case 6:
                {
                    FILE* arquivoHorarios = fopen("horarios.txt", "w");
                    if (arquivoHorarios == NULL) {
                        printf("Erro ao abrir o arquivo para salvar os horários!\n");
                    } else {
                        salvarHorarios(arvoreHorarios, arquivoHorarios);
                        fclose(arquivoHorarios);
                        printf("Horários salvos com sucesso!\n");
                    }
                    salvarFuncionarios();
                }
                break;
            case 7:
                printf("Volte sempre!\n");
                break;
            default:
                printf("Opção inválida, tente novamente.\n");
        }
    } while (opcao != 7);

    return 0;
}