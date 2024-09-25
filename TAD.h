#include <stdio.h>
#include <string.h>

// Definição da estrutura do nó AVL
typedef struct Horario {
    int diaSemana;
    int horario;
    int altura;
    struct Horario* esquerda;
    struct Horario* direita;
} Horario;

/*
* Função para obter a altura de um nó
*/
int altura(Horario* N);

/*
* Função que retorna o maior de dois números
*/
int max(int a, int b);

/*
* Função para criar um novo nó AVL
*/
Horario* novoHorario(int dia, int hora);

/*
* Função para fazer uma rotação simples à direita
*/
Horario* rotacaoDireita(Horario* y);

/*
* Função para fazer uma rotação simples à esquerda
*/
Horario* rotacaoEsquerda(Horario* x);

/*
* Função para obter o fator de balanceamento de um nó
*/
int getBalance(Horario* N);

/*
* Função para inserir um novo horário na árvore AVL
*/
Horario* adicionarHorario(Horario* node, int dia, int hora);

/*
* Função para verificar colisão de horário
*/
int verificarHorario(Horario* node, int dia, int hora);

/*
* Função para mostrar os horários agendados (em ordem crescente)
*/
void mostrarHorarios(Horario* root);

/*
*
*/
void marcarHorario(Horario** root);

/*
*
*/
void entrarServicos();

/*
*
*/
void profissionaisDisponiveis();

/*
*
*/
void cadastrarFuncionario();

/*
*
*/
void atualizarFuncionario();

/*
*
*/
void maisInformacoes();

/*
*
*/
int main();
