/*
* @folder numeroNome_ExP1
* @brief Gestão da agenda de um técnico de reparações de eletrodomésticos ao domicílio
* @date 31-01-2017
* @author (COLOCAR O NUMERO, NOME E EMAIL)
*/


#include <stdio.h>
#include <stdlib.h>

#include "declaracoes.h"
#include "funcoesAuxiliares.h"
#include "Exame.h"


/** ------------------------ Funções dos menus ----------------------- **/
int menu(int totalClientes, int *quantAgendamentos, int *quantVisitas);
int menuClientes(void);
int menuAgendamentos(void);
int menuEstatisticas(void);
int menuFicheiros(void);


/** ------------------- Funções das Funcionalidades ------------------- **/

void inserirNovoCliente (tipoCliente vetorClientes[MAX], int *quantClientes);
int procurarCliente (tipoCliente vetorClientes[MAX], int quantCliente, int numContribuinte);
void escreverCliente (tipoCliente cliente);
void listarClientes (tipoCliente vetorClientes[MAX], int quantClientes);



void escreverFicheiroBinario(tipoCliente vetorClientes[MAX], int quantClientes, tipoAgendamento *vetorDinAgendamento, int quantAgendamentos);
void lerFicheiroBinario(tipoCliente vetorClientes[MAX], int *quantClientes,tipoAgendamento *vetorDinAgendamentos, int *quantAgendamentos);


/* * ------------------------------ main ------------------------------ * */

int main(void)
{
    tipoCliente vetorClientes[MAX];
    tipoAgendamento *vetorDinAgendamento;
    vetorDinAgendamento = NULL;
    tipoMelhorCliente *vetorDinMelhorCliente;
    vetorDinMelhorCliente = NULL;



    //quantidade das satisfacoes
    int quantFraco = 0; // 1
    int quantBom = 0; // 2
    int quantExcelente = 0; //3
    int comVisitas = 0; // 1-com , 0-sem

    int quantAgendamentos = 0;
    int quantVisitas = 0;
    int quantClientes, opcao, opcao2;

    quantClientes = 0;

    do
    {
        opcao = menu(quantClientes,&quantAgendamentos,&quantVisitas);
        switch (opcao)
        {
        case 1: // menu CLIENTES
            do
            {
                opcao2 = menuClientes();
                switch (opcao2)
                {
                case 1:  // inserir novo cliente
                    inserirNovoCliente(vetorClientes, &quantClientes);
                    break;
                case 2 : // Listar Clientes
                    listarClientes(vetorClientes, quantClientes);
                    break;
                case 3:  // Listar agendamentos do cliente escolhido
                    mostrarAgendamentosContruibCorrespondente(vetorClientes,vetorDinAgendamento,&quantClientes,&quantAgendamentos);
                    break;
                }
            }
            while (opcao2 != 0);
            break;

        case 2: // menu AGENDAMENTOS
            do
            {
                opcao2 = menuAgendamentos();
                switch (opcao2)
                {
                case 1:  //registar agendamento
                    vetorDinAgendamento = registarAgendamento(vetorDinAgendamento,vetorClientes,&quantAgendamentos);
                    break;
                case 2 : //registar visita
                    registarVisita(vetorDinAgendamento,quantAgendamentos, &quantVisitas);
                    break;
                case 3:  //listar agendamentos por data
                    consultarAgendamentoPorData(vetorDinAgendamento,quantAgendamentos);
                    break;
                }
            }
            while (opcao2 != 0);
            break;

        case 3: // menu ESTATISTICA
            do
            {
                opcao2 = menuEstatisticas();
                switch (opcao2)
                {
                case 1:
                    if(quantAgendamentos > 0 && quantClientes > 0)
                    {
                        vetorDinMelhorCliente = guardarQuantAgendamentosPorCliente(vetorClientes,vetorDinAgendamento,quantAgendamentos,vetorDinMelhorCliente,quantClientes);
                        mostrarClienteComMaisAgendamentos(vetorDinMelhorCliente,quantClientes);
                    }
                    else
                    {
                        printf("Erro. Nao ha agendamentos!\n");
                    }
                    break;
                case 2 :
                    if(quantVisitas == 0)
                    {
                        printf("Erro. Nao existe visitas.\n");
                    }
                    else
                    {
                        contarGrauSatisfacao(vetorDinAgendamento,quantAgendamentos,&quantFraco,&quantBom,&quantExcelente);
                        calcularMostrarPercGrauSatisfacao(&quantVisitas,&quantFraco,&quantBom,&quantExcelente);
                    }
                    break;
                }
            }
            while (opcao2 != 0);
            break;

        case 4: // menu FICHEIROS
            do
            {
                opcao2 = menuFicheiros();
                switch (opcao2)
                {
                case 1:  // Guardar no ficheiro
                    escreverFicheiroBinario(vetorClientes,quantClientes,vetorDinAgendamento,quantAgendamentos);
                    break;
                case 2 : // Ler do ficheiro
                    lerFicheiroBinario(vetorClientes,&quantClientes,vetorDinAgendamento,&quantAgendamentos);
                    // nao consigo ler ficheiros, fread devolve 0 nos ultimos dados a carregar!
                    //ou seja, algum dado corrompido...
                    break;
                }
            }
            while (opcao2 != 0);
            break;
        }


    }
    while (opcao != 0);

    free(vetorDinAgendamento);
    free(vetorDinMelhorCliente);  //Libertamento da memoria dos vetores dinamicos!
    vetorDinAgendamento = NULL;
    vetorDinMelhorCliente = NULL;  //Apontar os vetores dinamicos vazios para NULL!
    return 0;
}





/** ------------------- Funções dos CLIENTES ------------------- **/

// Função que permite inserir um novo cliente no vetorClientes
void inserirNovoCliente (tipoCliente vetorClientes[MAX], int *quantClientes)
{
    int posicao, numContribuinte;

    if (*quantClientes < MAX)
    {
        printf("\n\n*********** NOVO CLIENTE ***********\n\n");
        numContribuinte = lerInteiro("\nNumero de contribuinte: ", CONTRIBUITE_MIN, CONTRIBUITE_MAX);
        posicao = procurarCliente(vetorClientes, *quantClientes, numContribuinte);

        if (posicao > -1)
        {
            printf("\n\nATENCAO: o nr. contribuinte inserido ja existe.\n");
            escreverCliente(vetorClientes[posicao]);
        }
        else
        {
            vetorClientes[*quantClientes].numContribuinte = numContribuinte;
            lerString("\nNome: ", vetorClientes[*quantClientes].nome, TEXTO_BREVE);
            lerString("\nMorada: ", vetorClientes[*quantClientes].morada, TEXTO_LONGO);
            vetorClientes[*quantClientes].telefone = lerInteiro("\nTelefone: ", TELEFONE_MIN, TELEFONE_MAX);
            (*quantClientes)++;
        }
    }
    else
    {
        printf("\n\nATENCAO: o limite maximo de clientes ja foi atingido\n\n");
    }
}


// Função que procura um cliente pelo nº de contribuinte. A função devolve -1 se não encontrar o cliente ou devolve a posição do cliente no vetor
int procurarCliente (tipoCliente vetorClientes[MAX], int quantCliente, int numContribuinte)
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


// Função que lista no ecra os dados de todos os clientes inseridos no vetor
void listarClientes (tipoCliente vetorClientes[MAX], int quantClientes)
{
    int i;

    if (quantClientes > 0)
    {
        printf("\n\n*********** LISTAGEM dos CLIENTES ***********\n\n");
        for (i=0; i < quantClientes; i++)
        {
            escreverCliente(vetorClientes[i]);
        }
    }
    else
    {
        printf("\n\nATENCAO: Nao existem clientes inseridos.\n\n");
    }
}


// Função que escreve no ecra os dados de um cliente
void escreverCliente (tipoCliente cliente)
{
    printf("\n\nContribuinte: %9d\t\tNome: %s\n", cliente.numContribuinte, cliente.nome);
    printf("Telefone: %13d\t\tMorada: %s\n", cliente.telefone, cliente.morada);
}



/** ------------------- Funções dos FICHEIROS ------------------- **/
void escreverFicheiroBinario(tipoCliente vetorClientes[MAX], int quantClientes, tipoAgendamento *vetorDinAgendamento, int quantAgendamentos)
{
    FILE *ficheiro;
    int quantValoresEscritos;

    ficheiro = fopen("dados.dat", "wb");

    if (ficheiro == NULL)
    {
        printf("\n\nERRO: falha na abertura do ficheiro.\n\n");
    }
    else
    {
        // CODIGO PARA GUARDAR QUANTIDADE DE CLIENTES E DADOS DOS CLIENTES
        quantValoresEscritos = fwrite(&quantClientes, sizeof(int), 1, ficheiro);
        if (quantValoresEscritos != 1)
        {
            printf("\n\nERRO: falha na escrita da quantidade de clientes\n\n");
        }
        else
        {
            printf("Quantidade de clientes guardada com sucesso!\n");
            quantValoresEscritos = fwrite(vetorClientes, sizeof(tipoCliente), quantClientes, ficheiro);
            if (quantValoresEscritos != quantClientes)
            {
                printf("\n\nERRO: falha na escrita dos dados dos clientes\n\n");
            }
            else
            {
                printf("Dados dos clientes guardados com sucesso!\n");
                //CODIGO PARA GUARDAR QUANTIDADE AGENDAMENTOS E DADOS DE AGENDAMENTOS
                quantValoresEscritos = fwrite(&quantAgendamentos, sizeof(int), 1, ficheiro);
                if (quantValoresEscritos != 1)
                {
                    printf("\n\nERRO: falha na escrita da quantidade de agendamentos\n\n");
                }
                else
                {
                    printf("Quantidade de agendamentos guardada com sucesso!\n");
                    quantValoresEscritos = fwrite(vetorDinAgendamento, sizeof(tipoAgendamento), quantAgendamentos, ficheiro);
                    if (quantValoresEscritos != quantAgendamentos)
                    {
                        printf("\n\nERRO: falha na escrita dos dados dos agendamentos\n\n");
                    }
                    else
                    {
                        printf("Dados de agendamentos guardados com sucesso!\n");

                    }
                }
            }
        }


        fclose(ficheiro);
    }
}

void lerFicheiroBinario(tipoCliente vetorClientes[MAX], int *quantClientes,tipoAgendamento *vetorDinAgendamentos, int *quantAgendamentos)
{
    FILE *ficheiro;
    int quantValoresLidos, erro = 0;

    ficheiro = fopen("dados.dat", "rb");

    if (ficheiro == NULL)
    {
        printf("\n\nERRO: falha na abertura do ficheiro.\n\n");
    }
    else
    {
        // CODIGO PARA GUARDAR QUANTIDADE DE CLIENTES E DADOS DOS CLIENTES
        quantValoresLidos = fread(quantClientes, sizeof(int), 1, ficheiro);
        if (quantValoresLidos != 1)
        {
            printf("\n\nERRO: falha na leitura da quantidade de clientes\n\n");
            erro = 1;
        }
        else
        {
            printf("Quantidade de clientes carregada com sucesso!\n");
            quantValoresLidos = fread(vetorClientes, sizeof(tipoCliente), *quantClientes, ficheiro);
            if (quantValoresLidos != *quantClientes)
            {
                printf("\n\nERRO: falha na leitura dos dados dos clientes\n\n");
                erro = 1;
            }
            else
            {
                printf("Dados dos clientes carregados com sucesso!\n");
                //CODIGO PARA GUARDAR QUANTIDADE AGENDAMENTOS E DADOS DE AGENDAMENTOS
                quantValoresLidos = fread(quantAgendamentos, sizeof(int),1,ficheiro);
                if (quantValoresLidos != 1)
                {
                    printf("\n\nERRO: falha na leitura da quantidade de argumentos\n\n");
                    erro = 1;
                }
                else
                {
                    //printf("Quantidade de agendamentos: %d\n", *quantAgendamentos);  ------->    - para testar -  mas esta correto
                    printf("Quantidade de agendamentos carregada com sucesso!\n");
                    quantValoresLidos = fread(vetorDinAgendamentos, sizeof(tipoAgendamento), *quantAgendamentos, ficheiro);
                    //printf("Valores devolvidos dos dados dos agendamentos: %d\n", quantValoresLidos);  ------> - para testar - devolve 0
                    if(quantValoresLidos != *quantAgendamentos)
                    {
                        printf("\n\nERRO: falha na leitura dos dados dos agendamentos\n\n");
                        //perror("fread"); ---> para tentar resolver o erro. "Erro: Invalid Argument"
                        erro = 1;
                    }
                    else
                    {
                        printf("Dados de agendamentos carregados com sucesso!\n");
                    }
                }
            }

        }

        if (erro == 1)
        {
            *quantClientes = 0;
            *quantAgendamentos = 0;
        }

        fclose(ficheiro);
    }
}

/** ----------------------- Funções dos MENUS ------------------------- **/

int menu(int totalClientes, int *quantAgendamentos, int *quantVisitas)
{
    int opcao;

    printf("\n\n---------------- TECNICO DE REPARACOES - AGENDA ----------------\n\n");
    printf("Total de Clientes: %d\n", totalClientes);
    printf("Total de Agendamentos: %d\n", *quantAgendamentos);
    printf("Total de Visitas: %d\n\n", *quantVisitas);
    printf("\t1 - Clientes\n\t2 - Agendamentos\n\t3 - Estatisticas\n\t4 - Ficheiros\n\n\t0 - Sair\n\n");

    opcao = lerInteiro("\t\tIndique a opcao:", 0, 4);

    return opcao;
}

int menuClientes(void)
{
    int opcao;

    printf("\n\n--------------------- MENU CLIENTES --------------------------\n\n");
    printf("\t1 - Adicionar novo cliente\n\t2 - Listar Clientes\n\t3 - Listar Clientes e respetivos agendamentos\n\n\t0 - Sair\n\n");

    opcao = lerInteiro("\t\tIndique a opcao:", 0, 3);

    return opcao;
}


int menuAgendamentos(void)
{
    int opcao;

    printf("\n\n------------------- MENU AGENDAMENTOS ------------------------\n\n");
    printf("\t1 - Adicionar novo agendamento\n\t2 - Registar Visita\n\t3 - Consultar Agendamentos (por data)\n\n\t0 - Sair\n\n");

    opcao = lerInteiro("\t\tIndique a opcao:", 0, 3);

    return opcao;
}


int menuEstatisticas(void)
{
    int opcao;

    printf("\n\n------------------- MENU ESTATISTICAS ------------------------\n\n");
    printf("\t1 - Melhor(es) Cliente(s)\n\t2 - Percentagens por grau de satisfacao dos clientes\n\n\t0 - Sair\n\n");

    opcao = lerInteiro("\t\tIndique a opcao:", 0, 2);

    return opcao;
}


int menuFicheiros(void)
{
    int opcao;

    printf("\n\n------------------- MENU FICHEIROS ------------------------\n\n");
    printf("\t1 - Guardar no ficheiro\n\t2 - Ler do ficheiro\n\n\t0 - Sair\n\n");

    opcao = lerInteiro("\t\tIndique a opcao:", 0, 2);

    return opcao;
}
