#ifndef EXAME_H_INCLUDED
#define EXAME_H_INCLUDED

#include "declaracoes.h"

//estrutura de argumentos
typedef struct
{
    int codigo; // codigo unico gerado automaticamente
    int numeroContribuinte;
    tipoData dataVisita;
    tipoHorario horarioVisita;
    char descricProblema[TEXTO_LONGO]; //descricao do porblema do eletrodoméstico
    float custo; //custo da visita
    int satisfacao; // 1-Fraco , 2-Bom, 3-Excelente
    int estado; // 0-Nao realizado, 1-Realizado

} tipoAgendamento;

typedef struct
{
    int numeroContribuinte;  //estrutura criada para criar um vetor para guardar a quantidade de
    int quantAgendamentos;   //agendamentos por cliente
} tipoMelhorCliente;

//funcoes agendamento
int guardarContribuinte(tipoCliente vetorClientes[MAX], int *semContribuinte);
void pedirDataHorarios(tipoAgendamento *vetorDinAgendamento, int quantAgendamentos);
tipoAgendamento* registarAgendamento(tipoAgendamento *vetorDinAgendamento,tipoCliente vetorClientes[MAX], int *quantAgendamento);
void registarVisita(tipoAgendamento *vetorDinAgendamento, int quantAgendamentos, int *quantVisitas);
void consultarAgendamentoPorData(tipoAgendamento *vetorDinAgendamentos, int quantAgendamentos);

//funcoes clientes
int procurarClienteExame (tipoCliente vetorClientes[MAX], int quantCliente, int numContribuinte);
void mostrarAgendamentosContruibCorrespondente(tipoCliente vetorClientes[MAX],
        tipoAgendamento *vetorDinAgendamento, int *quantClientes, int *quantAgendamento);

//funcoes estatistica
void contarGrauSatisfacao(tipoAgendamento *vetorDinAgendamento, int quantAgendamentos, int *quantFraco,
                               int *quantBom, int *quantExcelente);
 //-------------------------------------------------------------------------------------------------------------
void calcularMostrarPercGrauSatisfacao(int *quantVisitas, int *quantFraco, int *quantBom, int *quantExcelente);
//-------------------------------------------------------------------------------------------------------------
tipoMelhorCliente* guardarQuantAgendamentosPorCliente(tipoCliente vetorClientes[MAX],tipoAgendamento *vetorDinAgendamento,
        int quantAgendamentos, tipoMelhorCliente *vetorDinMelhorCliente, int quantClientes);
//-------------------------------------------------------------------------------------------------------------
void mostrarClienteComMaisAgendamentos(tipoMelhorCliente *vetorDinMelhorCliente, int quantClientes);
#endif // EXAME_H_INCLUDED
