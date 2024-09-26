#include "/home/neto/Área de trabalho/Unhas-de-Encanto/include/funcionario.h"

// Definição das variáveis globais
Funcionario funcionarios[MAX_FUNCIONARIOS];
int totalFuncionarios = 0;

// Implementação das funções de funcionários
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
