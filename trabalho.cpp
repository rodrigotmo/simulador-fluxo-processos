#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

//========= Estrutura do processo ============
struct Processo {
    int pid;
    int estado;
    int numeroCiclosPermitidosNoProcessador;
    int numeroCiclos;
    int numeroCiclosTotal;
    int numeroCiclosBloqueado;
    int numeroCiclosBloqueadoTotal;
    int numeroCiclosNaFilaAptos;
    bool passouPorCriacao;
    bool passouPorApto;
    bool passouPorExecucao;
    bool passouPorBloqueado;
    bool passouPorDestruido;
};

//============================================

//================= Variáveis Globais ==========================================

int criacao = 1, apto = 2, execucao = 3, bloqueado = 4, destruicao = 5;
int HD = 1, video = 2, impressora = 3;
int contadorDeProcessos = 0;
int numeroTotalDeCiclos = 0;
int numeroCiclosRodadoBloqueado = 0;
int contadorEstadoCriacao = 0;
int contadorEstadoApto = 0;
int contadorEstadoExecucao = 0;
int contadorEstadoBloqueado = 0;
int contadorEstadoDestruido = 0;
int contadorDevolvidosParaAptos = 0;
int contadorTempoEsperaFilaAptos = 0;
int contadorTempoExecucaoProcesso = 0;

vector<Processo> processador;
vector<Processo> filaAptos;
vector<Processo> filaHD;
vector<Processo> filaVideo;
vector<Processo> filaImpressora;
vector<Processo> listaDestruidos;

//=============================================================================

void criaProcesso() {
    contadorDeProcessos++;

    Processo processoCriado;

    processoCriado.pid = contadorDeProcessos;
    processoCriado.numeroCiclosTotal = 100 + (rand() % 200);
    processoCriado.numeroCiclos = processoCriado.numeroCiclosTotal;
    processoCriado.numeroCiclosBloqueadoTotal = 0;
    processoCriado.numeroCiclosBloqueado = 0;
    processoCriado.numeroCiclosNaFilaAptos = 0;
    processoCriado.estado = criacao;
    processoCriado.numeroCiclos = processoCriado.numeroCiclos - 1; //Primeiro ciclo é sempre de CPU
    processoCriado.passouPorCriacao = true;

    processoCriado.estado = apto;
    filaAptos.push_back(processoCriado);
}

void simulador(int numeroMaximoProcessos, int tempoEspera) {
    while (true) {

        int chanceNovoProcesso = rand() % 100;
        if (((chanceNovoProcesso > 0) && (chanceNovoProcesso <= 20)) && (contadorDeProcessos < numeroMaximoProcessos)) {
            criaProcesso();
        }

        //===== Contador de ciclos na fila de Aptos ==========================
        if (filaAptos.size() > 0) {
            for (int i = 0; i < filaAptos.size(); i++) {
                filaAptos[i].numeroCiclosNaFilaAptos++;
            }
        }

        //===== Finalização da execução do simulador ==========================
        if (listaDestruidos.size() == numeroMaximoProcessos) {
            cout << "Processos destruidos: " << listaDestruidos.size() << endl;
            break;
        }

        //===== Insere o primeiro da fila de Aptos para o processador =======================
        if ((filaAptos.size() > 0) && (processador.size() == 0)) {

            cout << "Foi enviado para o processador o processo: " << filaAptos[0].pid << endl;
            filaAptos[0].passouPorApto = true;
            processador.push_back(filaAptos.front());
            filaAptos.erase(filaAptos.begin());
            processador[0].estado = execucao;
            processador[0].numeroCiclosPermitidosNoProcessador = 50;
            processador[0].passouPorExecucao = true;

        }

        //===== Destruicao do processo caso ele tenha chegado ao seu fim ====
        if ((processador.size() > 0) && (processador[0].numeroCiclos == 0)) {

            processador[0].estado = destruicao;
            processador[0].passouPorDestruido = true;
            listaDestruidos.push_back(processador.front());
            processador.erase(processador.begin());
            cout << "Processo " << processador[0].pid << " destruido" << endl;
        }

        //===== Processador ======================================================================================================================
        if (processador.size() > 0) {
            cout << "PID do processo: " <<  processador[0].pid << " - ";
            cout << "Estado do processo: " << processador[0].estado << " - ";
            cout << "Numero ciclos restantes: " << processador[0].numeroCiclos << " - ";
            cout << "Numero total de ciclos: " << numeroTotalDeCiclos << endl;

            int chanceES = rand() % 100;
            int dispositivo = rand() % 3;

            if (chanceES == 1) {
                processador[0].estado = bloqueado;
                if (dispositivo == HD) {

                    processador[0].numeroCiclosBloqueado = 200 + (rand() % 100);
                    processador[0].numeroCiclosBloqueadoTotal = processador[0].numeroCiclosBloqueadoTotal + processador[0].numeroCiclosBloqueado;
                    cout << "Processo " << processador[0].pid <<  " requisitou HD" << endl;
                    filaHD.push_back(processador.front());
                    processador.erase(processador.begin());

                } else {
                    if (dispositivo == video) {

                        processador[0].numeroCiclosBloqueado = 100 + (rand() % 100);
                        processador[0].numeroCiclosBloqueadoTotal = processador[0].numeroCiclosBloqueadoTotal + processador[0].numeroCiclosBloqueado;
                        cout << "Processo " << processador[0].pid <<  " requisitou Video" << endl;
                        filaVideo.push_back(processador.front());
                        processador.erase(processador.begin());

                    } else {

                        processador[0].numeroCiclosBloqueado = 500 + (rand() % 100);
                        processador[0].numeroCiclosBloqueadoTotal = processador[0].numeroCiclosBloqueadoTotal + processador[0].numeroCiclosBloqueado;
                        cout << "Processo " << processador[0].pid <<  " requisitou Impressora" << endl;
                        filaImpressora.push_back(processador.front());
                        processador.erase(processador.begin());
                    }
                }
            }

            processador[0].numeroCiclos--;
            processador[0].numeroCiclosPermitidosNoProcessador--;
        }

        //===== Fila da requisição do Video ==============================================================================
        if (filaVideo.size() > 0) {
            if (filaVideo[0].numeroCiclosBloqueado == 0) {

                cout << "Foi enviado para fila aptos o processo: " << filaVideo[0].pid << endl;
                filaVideo[0].passouPorBloqueado = true;
                filaAptos.push_back(filaVideo.front());
                filaVideo.erase(filaVideo.begin());

            } else {

                cout << "Processo " << filaVideo[0].pid << " bloqueado por " << filaVideo[0].numeroCiclosBloqueado << endl;
                filaVideo[0].numeroCiclosBloqueado--;
            }
        }

        //===== Fila da requisição da Impressora ===================================================================================
        if (filaImpressora.size() > 0) {
            if (filaImpressora[0].numeroCiclosBloqueado == 0) {

                cout << "Foi enviado para fila aptos o processo: " << filaImpressora[0].pid << endl;
                filaImpressora[0].passouPorBloqueado = true;
                filaAptos.push_back(filaImpressora.front());
                filaImpressora.erase(filaImpressora.begin());

            } else {

                cout << "Processo " << filaImpressora[0].pid << " bloqueado por " << filaImpressora[0].numeroCiclosBloqueado << endl;
                filaImpressora[0].numeroCiclosBloqueado--;
            }
        }

        //===== Fila da requisição do HD ============================================================================
        if (filaHD.size() > 0) {
            if (filaHD[0].numeroCiclosBloqueado == 0) {

                cout << "Foi enviado para fila aptos o processo: " << filaHD[0].pid << endl;
                filaHD[0].passouPorBloqueado = true;
                filaAptos.push_back(filaHD.front());
                filaHD.erase(filaHD.begin());

            } else {

                cout << "Processo " << filaHD[0].pid << " bloqueado por " << filaHD[0].numeroCiclosBloqueado << endl;
                filaHD[0].numeroCiclosBloqueado--;
            }
        }

        //===== Processador devolve para fila de Aptos caso o processo tenha utilizado seus 50 ciclos sem interrupções =====
        if ((processador.size() > 0) && (processador[0].numeroCiclosPermitidosNoProcessador == 0)) {

            processador[0].estado = apto;
            contadorDevolvidosParaAptos++;
            cout << "Processo " << processador[0].pid << " foi enviado para aptos" << endl;
            filaAptos.push_back(processador.front());
            processador.erase(processador.begin());
            processador.clear();
        }

        numeroTotalDeCiclos++;
        Sleep(50);
        //Sleep(tempoEspera * 100);
    } //======= Final do laço do simulador ===================================================================================


    //========= Laço para pegar informações para o relatório final =============================================================================================

    for (int i = 0; i < listaDestruidos.size(); i++) {
        if (listaDestruidos[i].passouPorCriacao == true) {
            contadorEstadoCriacao++;
        }
        if (listaDestruidos[i].passouPorApto == true) {
            contadorEstadoApto++;
        }
        if (listaDestruidos[i].passouPorExecucao == true) {
            contadorEstadoExecucao++;
        }
        if (listaDestruidos[i].passouPorBloqueado == true) {
            contadorEstadoBloqueado++;
        }
        if (listaDestruidos[i].passouPorDestruido == true) {
            contadorEstadoDestruido++;
        }
        contadorTempoEsperaFilaAptos = contadorTempoEsperaFilaAptos + listaDestruidos[i].numeroCiclosNaFilaAptos;
        contadorTempoExecucaoProcesso = contadorTempoExecucaoProcesso + (listaDestruidos[i].numeroCiclosTotal + listaDestruidos[i].numeroCiclosBloqueadoTotal);
    }

    system("cls");
    //======================= Relatório final ===================================================================================================
    cout << "Numero total de processos criados: " << contadorDeProcessos << endl;;
    cout << "Numero total de ciclos para atender todos os processos: " << numeroTotalDeCiclos << endl;
    cout << "Numero de ciclos medio por processo: " << numeroTotalDeCiclos/contadorDeProcessos << endl;
    cout << "Numero de processos que passaram por Criacao: " << contadorEstadoCriacao << endl;
    cout << "Numero de processos que passaram por Apto: " << contadorEstadoApto << endl;
    cout << "Numero de processos que passaram por Execucao: " << contadorEstadoExecucao << endl;
    cout << "Numero de processos que passaram por Bloqueado: " << contadorEstadoBloqueado << endl;
    cout << "Numero de processos que passaram por Destruicao: " << contadorEstadoDestruido << endl;
    cout << "Tempo medio de espera na fila de aptos: " << contadorTempoEsperaFilaAptos/contadorDeProcessos << endl;
    cout << "Numero de vezes que os processos foram retirados do processador e devolvidos para aptos: " << contadorDevolvidosParaAptos << endl;
}

int main(void){
    int numero, tempoEspera;
    cin >> numero;
    cin >> tempoEspera;
    srand((unsigned)time(NULL));
    simulador(numero, tempoEspera);
}
