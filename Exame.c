#include <stdio.h>
#include <stdlib.h>

#include "Exame.h"
#include "declaracoes.h"
#include "funcoesAuxiliares.h"

//funcoes agendamento
int guardarContribuinte(tipoCliente vetorClientes[MAX], int *semContribuinte)
{
    int contribuinte = 0;
    int contribuinteEncontrado = 0;
    int i; // contador

    contribuinte = lerInteiro("Insirir numero de contribuinte: ", CONTRIBUITE_MIN, CONTRIBUITE_MAX);
    for (i=0; i < MAX; i++)
    {
        if(contribuinte == vetorClientes[i].numContribuinte)
        {
            contribuinteEncontrado = -1;
        }
    }
    if(contribuinteEncontrado == 0)
    {
        *semContribuinte = 0;
        printf("Erro. Numero de contribuinte nao existente!\n");
    }
    if(contribuinteEncontrado == -1)
    {
        return contribuinte;
    }
}

void pedirDataHorarios(tipoAgendamento *vetorDinAgendamento, int quantAgendamentos) //verifica disponibilidade e pede horarios
{
    int disponibilidade = 1;// 1-disponivel,0-ocupado
    int i;//contador
    tipoData dataInserida;
    tipoHorario horarioInserido;

    do
    {
        dataInserida = lerData();
        horarioInserido = lerHorario();
        for(i=0; i < quantAgendamentos; i++)
        {
            if(dataInserida.ano == vetorDinAgendamento[i].dataVisita.ano)
            {
                if(dataInserida.mes == vetorDinAgendamento[i].dataVisita.mes)
                {
                    if(dataInserida.dia == vetorDinAgendamento[i].dataVisita.dia)
                    {
                        if(horarioInserido.horas == vetorDinAgendamento[i].horarioVisita.horas)
                        {
                            if(horarioInserido.minutos == vetorDinAgendamento[i].horarioVisita.minutos)
                            {
                                printf("Erro. Sem disponibilidade!\n");
                                disponibilidade = 0;
                            }
                        }
                    }
                }
            }

        }

    }
    while(disponibilidade ==0);
    if(disponibilidade == 1)
    {
        vetorDinAgendamento[quantAgendamentos-1].dataVisita.ano = dataInserida.ano;
        vetorDinAgendamento[quantAgendamentos-1].dataVisita.mes = dataInserida.mes;
        vetorDinAgendamento[quantAgendamentos-1].dataVisita.dia = dataInserida.dia;
        vetorDinAgendamento[quantAgendamentos-1].horarioVisita = horarioInserido;
    }
}

tipoAgendamento* registarAgendamento(tipoAgendamento *vetorDinAgendamento,tipoCliente vetorClientes[MAX], int *quantAgendamento)
{
    tipoAgendamento *pAux;
    pAux = vetorDinAgendamento;
    int semContribuinte = -1; //-1 tem; 0 nao tem
    int posicao = 0;

    (*quantAgendamento)++;
    vetorDinAgendamento = realloc(vetorDinAgendamento,(*quantAgendamento)* sizeof(tipoAgendamento));
    posicao = (*quantAgendamento) - 1;
    //printf("POSICAOOOOOOOOOOO = %d", posicao);

    if(vetorDinAgendamento == NULL)
    {
        printf("Erro. Problema com a memoria.\n");
        vetorDinAgendamento = pAux;
    }
    else
    {
        vetorDinAgendamento[posicao].codigo = *quantAgendamento; //pois a quantAgendamento nao é o primeiro campo do vetor "0"
        vetorDinAgendamento[posicao].numeroContribuinte = guardarContribuinte(vetorClientes,&semContribuinte);
        if(semContribuinte == -1) //se encontrar um contribuinte continua a pedir dados
        {

            pedirDataHorarios(vetorDinAgendamento,*quantAgendamento);
            lerString("Descrever Problema:",vetorDinAgendamento[posicao].descricProblema,TEXTO_LONGO);
            vetorDinAgendamento[posicao].estado = 0;
        }
        else //senao encontrar retira -1 na quant e nao pede mais dados
        {
            (*quantAgendamento)--;
            vetorDinAgendamento[posicao].codigo = -1;
            vetorDinAgendamento[posicao].numeroContribuinte = 00000000;
        }
    }

    return vetorDinAgendamento;
}

void registarVisita(tipoAgendamento *vetorDinAgendamento, int quantAgendamentos, int *quantVisitas)
{
    int codigoUnico;
    int i; // contador
    int codigoExiste = 0; // 1-Sim , 0- Nao

    printf("\nInserir codigo unico: ");
    scanf("%d", &codigoUnico);                // nao utilizei a funcao, pois nao existe valor maximo de agendamentos
    limparBuffer();

    for(i=0; i < quantAgendamentos; i++)
    {
        if(codigoUnico == vetorDinAgendamento[i].codigo)
        {
            if(vetorDinAgendamento[i].estado == 1)
            {
                printf("Erro. Agendamento ja realizado.\n");
            }
            else
            {
                codigoExiste = 1;
                (*quantVisitas)++;
                vetorDinAgendamento[i].custo = lerFloat("Inserir custo: ",0,9999999);
                vetorDinAgendamento[i].satisfacao = lerInteiro("Inserir avaliacao do cliente (1-Fraco,2-Bom,3-Excelente):",1,3);
                vetorDinAgendamento[i].estado = 1; // Fica realizado
            }
        }

    }
    if (codigoExiste == 0)
    {
        printf("Erro. O codigo inserido nao existe.\n");
    }

}


void consultarAgendamentoPorData(tipoAgendamento *vetorDinAgendamentos, int quantAgendamentos)
{
    tipoData data;
    int i;
    int existe = 0; // 0-Nao existe, 1-Existe

    printf("\nInserir data:\n");
    data = lerData();

    for(i=0; i < quantAgendamentos; i++)
    {
        if(vetorDinAgendamentos[i].dataVisita.ano == data.ano)
        {
            if(vetorDinAgendamentos[i].dataVisita.mes == data.mes)
            {
                if(vetorDinAgendamentos[i].dataVisita.dia == data.dia)
                {
                    existe = 1;
                    printf("\nCodigo: %d\t\t\tContribuinte: %9d\n",
                           vetorDinAgendamentos[i].codigo,vetorDinAgendamentos[i].numeroContribuinte);
                    printf("Horario: %dh:%dmin\t\tEstado (0-Nao realizado, 1-Realizado): %d\n",
                           vetorDinAgendamentos[i].horarioVisita.horas,vetorDinAgendamentos[i].horarioVisita.minutos,
                           vetorDinAgendamentos[i].estado);
                    printf("Descricao: %s \n", vetorDinAgendamentos[i].descricProblema);
                    if(vetorDinAgendamentos[i].estado == 1)
                    {
                        printf("Custo:%.2f$\n", vetorDinAgendamentos[i].custo);
                        printf("Grau de satisfacao (1-Fraco , 2-Bom, 3-Excelente): %d \n\n\n\n",
                               vetorDinAgendamentos[i].satisfacao);
                    }

                }
            }
        }

    }
    if(existe == 0)
    {
        printf("Erro.A data escolhida nao tem agendamentos.\n");
    }


}

//funcoes cliente
int procurarClienteExame (tipoCliente vetorClientes[MAX], int quantCliente, int numContribuinte)
{
    int posicao = -1, i;

    for (i=0; i < quantCliente;  i++)
    {
        if (vetorClientes[i].numContribuinte == numContribuinte)
        {
            posicao = i;
            i = quantCliente;
        }
    }

    return posicao;
}

void mostrarAgendamentosContruibCorrespondente(tipoCliente vetorClientes[MAX],
        tipoAgendamento *vetorDinAgendamento, int *quantClientes, int *quantAgendamento)
{
    int contribuinte, posicao;
    int i; //contador
    int comCorrespondencia = 0; //0-sem , 1-com

    contribuinte = lerInteiro("Inserir contribuinte: ", CONTRIBUITE_MIN, CONTRIBUITE_MAX);
    posicao = procurarClienteExame(vetorClientes,*quantClientes,contribuinte);
    if(posicao == -1)
    {
        printf("Erro. Numero de contribuinte nao encontrado.\n");
    }
    else
    {
        for(i=0; i < (*quantAgendamento); i++)
        {
            if(vetorDinAgendamento[i].numeroContribuinte == contribuinte)
            {
                comCorrespondencia = 1;
                printf("\n\nCodigo: %d ", vetorDinAgendamento[i].codigo);
                printf("\nData: %d/%d/%d", vetorDinAgendamento[i].dataVisita.ano,
                       vetorDinAgendamento[i].dataVisita.mes, vetorDinAgendamento[i].dataVisita.dia);
                printf("\nHorario: %dh:%dmin", vetorDinAgendamento[i].horarioVisita.horas,
                       vetorDinAgendamento[i].horarioVisita.minutos);
                printf("\nDescricao: %s", vetorDinAgendamento[i].descricProblema);
                printf("\nEstado (0-Nao realizado, 1-Realizado): %d", vetorDinAgendamento[i].estado);
                if( vetorDinAgendamento[i].estado == 1)
                {
                    printf("\nGrau de satisfacao (1-Fraco , 2-Bom, 3-Excelente): %d", vetorDinAgendamento[i].satisfacao);
                    printf("\nCusto:%.2f$\n\n\n\n", vetorDinAgendamento[i].custo);
                }

            }
        }
        if(comCorrespondencia == 0)
        {
            printf("Erro. Cliente sem agendamentos.\n");
        }

    }



}

//funcoes estatistica
void contarGrauSatisfacao(tipoAgendamento *vetorDinAgendamento, int quantAgendamentos, int *quantFraco,
                          int *quantBom, int *quantExcelente)
{
    int i;//contador
    *quantFraco = 0;
    *quantBom = 0;
    *quantExcelente = 0;

    for(i=0; i < quantAgendamentos; i++)
    {
        if(vetorDinAgendamento[i].satisfacao == 1)
        {
            (*quantFraco)++;
        }
        if(vetorDinAgendamento[i].satisfacao == 2)
        {
            (*quantBom)++;
        }
        if(vetorDinAgendamento[i].satisfacao == 3)
        {
            (*quantExcelente)++;
        }
    }


}

void calcularMostrarPercGrauSatisfacao(int *quantVisitas, int *quantFraco, int *quantBom, int *quantExcelente)
{
    float auxFraco, auxBom, auxExcelente;
    float percFraco, percBom, percExcelente;

    auxFraco = (float) *quantFraco / *quantVisitas;
    auxBom = (float)*quantBom / *quantVisitas;
    auxExcelente = (float)*quantExcelente / *quantVisitas;
    percFraco = auxFraco*100;
    percBom = auxBom*100;
    percExcelente = auxExcelente*100;

    printf("\nPERCENTAGENS DE SATISFACAO:\n\n");
    printf("Excelente (3): %.2f%% \n", percExcelente);
    printf("Bom (2): %.2f%% \n", percBom);
    printf("Fraco (1): %.2f%% \n", percFraco);
}


tipoMelhorCliente* guardarQuantAgendamentosPorCliente(tipoCliente vetorClientes[MAX],tipoAgendamento *vetorDinAgendamento,
                                        int quantAgendamentos, tipoMelhorCliente *vetorDinMelhorCliente, int quantClientes)
{
    int i, j; //contadores
    int quant;
    vetorDinMelhorCliente = realloc(vetorDinMelhorCliente,quantClientes*sizeof(tipoMelhorCliente));


    for(i=0; i < quantClientes; i++)
    {   quant = 0; // para resetar cada vez que troque de contribuinte e nao acumular
        for(j=0; j < quantAgendamentos; j++)
        {
            if(vetorClientes[i].numContribuinte == vetorDinAgendamento[j].numeroContribuinte)
            {
               // printf("I=%d --------- J=%d",i,j);
                quant++;
                vetorDinMelhorCliente[i].numeroContribuinte = vetorDinAgendamento[j].numeroContribuinte;
                vetorDinMelhorCliente[i].quantAgendamentos = quant;
            }


        }
    }

    //printf("vetorDinMelhorCliente[0].numeroContribuinte = %d AAAAAA",vetorDinMelhorCliente[0].numeroContribuinte);
    return vetorDinMelhorCliente;

}

void mostrarClienteComMaisAgendamentos(tipoMelhorCliente *vetorDinMelhorCliente, int quantClientes)
{
    int i;//contador
    int contribuinte = -1; // contribuinte com maior numero de agendamentos
    int quantAgendamentosMaior = -1; // maior numero de agendamentos

    if(quantClientes != 1)
    {
        for(i=0; i < quantClientes; i++)
        {
            if(i != (quantClientes-1))
            {
                if(vetorDinMelhorCliente[i].quantAgendamentos > vetorDinMelhorCliente[i+1].quantAgendamentos)
                {
                    contribuinte = vetorDinMelhorCliente[i].numeroContribuinte;
                    quantAgendamentosMaior = vetorDinMelhorCliente[i].quantAgendamentos;
                }
            }
            else //pois senao estaria a comparar o ultimo elemento com nada = crashar
            {
                if(quantAgendamentosMaior < vetorDinMelhorCliente[i].quantAgendamentos) // i = quantClientes - 1
                {
                    contribuinte = vetorDinMelhorCliente[i].numeroContribuinte;
                    quantAgendamentosMaior = vetorDinMelhorCliente[i].quantAgendamentos;
                }

            }

        }

    }
    else //quando quantClientes = 1
    {
        //printf("vetorDinMelhorCliente[0].numeroContribuinte = %d",vetorDinMelhorCliente[0].numeroContribuinte);
        contribuinte = vetorDinMelhorCliente[0].numeroContribuinte;
        quantAgendamentosMaior = vetorDinMelhorCliente[0].quantAgendamentos;
    }

    printf("\nMELHOR CLIENTE (CLIENTE COM MAIS AGENDAMENTOS):\n\n");
    printf("Contribuinte: %d", contribuinte);
    printf("\nQuantidade de agendamentos: %d \n\n", quantAgendamentosMaior);
}





















