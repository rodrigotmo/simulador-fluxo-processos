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

void criarProcesso() {
    contadorDeProcessos++;

    Processo processoCriado;

    processoCriado.pid = contadorDeProcessos;
    processoCriado.numeroCiclosTotal = 100 + (rand() % 200);
    processoCriado.numeroCiclos = processoCriado.numeroCiclosTotal;
    processoCriado.numeroCiclosBloqueadoTotal = 0;
    processoCriado.numeroCiclosBloqueado = 0;
    processoCriado.numeroCiclosNaFilaAptos = 0;
    processoCriado.estado = criacao;
    processoCriado.passouPorCriacao = true;

    processoCriado.estado = apto;
    filaAptos.push_back(processoCriado);
}

void montarInformacoesRelatorio() {
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
        contadorTempoExecucaoProcesso = contadorTempoExecucaoProcesso + (listaDestruidos[i].numeroCiclosTotal + listaDestruidos[i].numeroCiclosBloqueadoTotal + listaDestruidos[i].numeroCiclosNaFilaAptos);
    }
}

void imprimirRelatorioFinal() {
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

void simulador(int numeroMaximoProcessos, int tempoEspera) {
    while (true) {

        int chanceNovoProcesso = rand() % 100;
        if (((chanceNovoProcesso > 0) && (chanceNovoProcesso <= 20)) && (contadorDeProcessos < numeroMaximoProcessos)) {
            criarProcesso();
        }

        //===== Contador de ciclos na fila de Aptos ==========================
        if (filaAptos.size() > 0) {
            for (int i = 0; i < filaAptos.size(); i++) {
                filaAptos[i].numeroCiclosNaFilaAptos++;
            }
        }

        //===== Finalização da execução do simulador ==========================
        if (listaDestruidos.size() == numeroMaximoProcessos) {
            break;
        }

        //===== Insere o primeiro da fila de Aptos para o processador =======================
        if ((filaAptos.size() > 0) && (processador.size() == 0)) {
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
        }

        //===== Processador ======================================================================================================================
        if (processador.size() > 0) {
            if (processador[0].numeroCiclos == processador[0].numeroCiclosTotal) {
                processador[0].numeroCiclos--;
                processador[0].numeroCiclosPermitidosNoProcessador--;
            } else {
                int chanceES = rand() % 100;
                int dispositivo = rand() % 3;

                if (chanceES == 1) {
                    processador[0].estado = bloqueado;
                    if (dispositivo == HD) {
                        processador[0].numeroCiclosBloqueado = 200 + (rand() % 100);
                        processador[0].numeroCiclosBloqueadoTotal = processador[0].numeroCiclosBloqueadoTotal + processador[0].numeroCiclosBloqueado;
                        filaHD.push_back(processador.front());
                        processador.erase(processador.begin());
                    } else {
                        if (dispositivo == video) {
                            processador[0].numeroCiclosBloqueado = 100 + (rand() % 100);
                            processador[0].numeroCiclosBloqueadoTotal = processador[0].numeroCiclosBloqueadoTotal + processador[0].numeroCiclosBloqueado;
                            filaVideo.push_back(processador.front());
                            processador.erase(processador.begin());
                        } else {
                            processador[0].numeroCiclosBloqueado = 500 + (rand() % 100);
                            processador[0].numeroCiclosBloqueadoTotal = processador[0].numeroCiclosBloqueadoTotal + processador[0].numeroCiclosBloqueado;
                            filaImpressora.push_back(processador.front());
                            processador.erase(processador.begin());
                        }
                    }
                }

                processador[0].numeroCiclos--;
                processador[0].numeroCiclosPermitidosNoProcessador--;
            }
        }

        //===== Fila da requisição do Video ==============================================================================
        if (filaVideo.size() > 0) {
            if (filaVideo[0].numeroCiclosBloqueado == 0) {
                filaVideo[0].passouPorBloqueado = true;
                filaAptos.push_back(filaVideo.front());
                filaVideo.erase(filaVideo.begin());
            } else {
                filaVideo[0].numeroCiclosBloqueado--;
            }
        }

        //===== Fila da requisição da Impressora ===================================================================================
        if (filaImpressora.size() > 0) {
            if (filaImpressora[0].numeroCiclosBloqueado == 0) {
                filaImpressora[0].passouPorBloqueado = true;
                filaAptos.push_back(filaImpressora.front());
                filaImpressora.erase(filaImpressora.begin());
            } else {
                filaImpressora[0].numeroCiclosBloqueado--;
            }
        }

        //===== Fila da requisição do HD ============================================================================
        if (filaHD.size() > 0) {
            if (filaHD[0].numeroCiclosBloqueado == 0) {
                filaHD[0].passouPorBloqueado = true;
                filaAptos.push_back(filaHD.front());
                filaHD.erase(filaHD.begin());
            } else {
                filaHD[0].numeroCiclosBloqueado--;
            }
        }

        //===== Processador devolve para fila de Aptos caso o processo tenha utilizado seus 50 ciclos sem interrupções =====
        if ((processador.size() > 0) && (processador[0].numeroCiclosPermitidosNoProcessador == 0)) {
            processador[0].estado = apto;
            contadorDevolvidosParaAptos++;
            filaAptos.push_back(processador.front());
            processador.erase(processador.begin());
            processador.clear();
        }

        //===== Interface gráfica com os estados de cada processo ============================================
        if (filaAptos.size() > 0) {
            for (int i = 0; i < filaAptos.size(); i++) {
                cout << "Processo " << filaAptos[i].pid << " estado APTO na posicao " << i+1 << endl;
            }
        }
        if (processador.size() > 0) {
            cout << "Processo " << processador[0].pid << " estado EXECUTANDO" << endl;
        }
        if (filaHD.size() > 0) {
            for (int i = 0; i < filaHD.size(); i++) {
                if (i == 0) {
                    cout << "Processo " << filaHD[i].pid << " estado BLOQUEADO usando HD" << endl;
                } else {
                    cout << "Processo " << filaHD[i].pid << " estado BLOQUEADO esperando na fila de HD na posicao " << i+1 << endl;
                }
            }
        }
        if (filaVideo.size() > 0) {
            for (int i = 0; i < filaVideo.size(); i++) {
                if (i == 0) {
                    cout << "Processo " << filaVideo[i].pid << " estado BLOQUEADO usando Video" << endl;
                } else {
                    cout << "Processo " << filaVideo[i].pid << " estado BLOQUEADO esperando na fila de Video na posicao " << i+1 << endl;
                }
            }
        }
        if (filaImpressora.size() > 0) {
            for (int i = 0; i < filaImpressora.size(); i++) {
                if (i == 0) {
                    cout << "Processo " << filaImpressora[i].pid << " estado BLOQUEADO usando Impressora" << endl;
                } else {
                    cout << "Processo " << filaImpressora[i].pid << " estado BLOQUEADO esperando na fila de Impressora na posicao " << i+1 << endl;
                }
            }
        }
        if (listaDestruidos.size() > 0) {
            for (int i = 0; i < listaDestruidos.size(); i++) {
                cout << "Processo " << listaDestruidos[i].pid << " estado DESTRUIDO" << endl;
            }
        }

        numeroTotalDeCiclos++;
        Sleep(tempoEspera * 100);
        system("cls");
    } //======= Final do laço do simulador ===================================================================================

    montarInformacoesRelatorio();

    system("cls");

    imprimirRelatorioFinal();
}

int main(int argc, char *argv[]){
    string num, temp;
    int numero, tempoEspera;
    if (argc != 3) {
        cout << "Inicializacao errada! Lembre-se: sfp -nx -tx" << endl;
        return 0;
    }
    num = argv[1];
    temp = argv[2];
    numero = atoi(num.substr(2, num.length()).c_str());
    tempoEspera = atoi(temp.substr(2, temp.length()).c_str());
    if (numero > 999) {
        cout << "Nao e permitido criar mais de 999 processos!" << endl;
        return 0;
    }
    srand((unsigned)time(NULL));
    simulador(numero, tempoEspera);
    cout << "Tempo total: " << finalSimulador << " segundos" << endl;
}
