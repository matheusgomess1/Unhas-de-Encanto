#ifndef FUNCIONARIOS_H
#define FUNCIONARIOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição de constantes
#define MAX_FUNCIONARIOS 10 // Máximo de funcionários

// Definição da estrutura de funcionário
typedef struct funcionario Funcionario;

// Variáveis globais para funcionários
extern Funcionario funcionarios[MAX_FUNCIONARIOS];  // Lista de funcionários
extern int totalFuncionarios;                       // Total de funcionários cadastrados

// Prototipação das funções de manipulação de funcionários
void salvarFuncionarios();                           // Salva a lista de funcionários em um arquivo
void carregarFuncionarios();                         // Carrega a lista de funcionários de um arquivo
void cadastrarFuncionario();                         // Cadastra um novo funcionário
void profissionaisDisponiveis();                     // Exibe os funcionários disponíveis

#endif
