#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

struct Processo {
    int pid;
    int estado;
    int numeroCiclosRodados;
    int numeroCiclos;
    int numeroCiclosBloqueado;
};

//=================Variáveis Globais==========================================

int criacao = 1, apto = 2, execucao = 3, bloqueado = 4, destruicao = 5;
int HD = 1, video = 2, impressora = 3;
int contadorDeProcessos = 0;
int processosDestruidos = 0;
int numeroTotalDeCiclos = 0;
int numeroCiclosRodadoBloqueado = 0;
int numeroCiclosPermitidoRodar = 50;
vector<Processo> executando;
vector<Processo> filaAptos;
vector<Processo> filaHD;
vector<Processo> filaVideo;
vector<Processo> filaImpressora;
vector<Processo> listaDestruidos;

//=============================================================================

void criaProcesso() {
    contadorDeProcessos++;
    Processo processoCriado;
    processoCriado.numeroCiclosRodados = 0;
    processoCriado.numeroCiclos = 100 + (rand() % 200);
    processoCriado.numeroCiclosBloqueado = 0;
    processoCriado.pid = contadorDeProcessos;
    processoCriado.estado = criacao;
    numeroTotalDeCiclos++;
    processoCriado.numeroCiclos = processoCriado.numeroCiclos - 1;
    processoCriado.estado = apto;
    filaAptos.push_back(processoCriado);
}

void simulador(int numeroMaximoProcessos) {
    while (1) {
        int chanceNovoProcesso = rand() % 100;
        if (((chanceNovoProcesso > 0) && (chanceNovoProcesso <= 20)) && (contadorDeProcessos < numeroMaximoProcessos)) {
            criaProcesso();
        }

        if (filaAptos.size() > 0) {
            if (executando.size() == 0) {
                cout << "Foi enviado para o processador" << endl;
                executando.push_back(filaAptos.front());
                filaAptos.erase(filaAptos.begin());
                executando[0].estado = execucao;
            }
        }

        if (filaVideo.size() > 0) {
            if (filaVideo[0].numeroCiclosBloqueado == 0) {
                filaAptos.push_back(filaVideo.front());
                filaVideo.erase(filaVideo.begin());
            } else {
                cout << "Bloqueado por " << filaVideo[0].numeroCiclosBloqueado << endl;
                filaVideo[0].numeroCiclosBloqueado--;
            }
        }

        if (filaImpressora.size() > 0) {
            if (filaImpressora[0].numeroCiclosBloqueado == 0) {
                filaAptos.push_back(filaImpressora.front());
                filaImpressora.erase(filaImpressora.begin());
            } else {
                cout << "Bloqueado por " << filaImpressora[0].numeroCiclosBloqueado << endl;
                filaImpressora[0].numeroCiclosBloqueado--;
            }
        }

        if (filaHD.size() > 0) {
            if (filaHD[0].numeroCiclosBloqueado == 0) {
                filaAptos.push_back(filaHD.front());
                filaHD.erase(filaHD.begin());
            } else {
                cout << "Bloqueado por " << filaHD[0].numeroCiclosBloqueado << endl;
                filaHD[0].numeroCiclosBloqueado--;
            }
        }

        if ((executando.size() > 0) && (executando[0].numeroCiclos == 0)) {
            executando[0].estado = destruicao;
            processosDestruidos++;
            listaDestruidos.push_back(executando.front());
            executando.erase(executando.begin());
            cout << "Destruido" << endl;
            break;
        }

        if (executando.size() > 0) {
            cout << "PID do processo: " <<  executando[0].pid << " - ";
            cout << "Estado do processo: " << executando[0].estado << " - ";
            cout << "Numero ciclos restantes: " << executando[0].numeroCiclos << " - ";
            cout << "Numero total de ciclos: " << numeroTotalDeCiclos << endl;

            int chanceES = rand() % 100;
            int dispositivo = rand() % 3;

            if (chanceES == 1) {
                executando[0].estado = bloqueado;
                if (dispositivo == HD) {
                    executando[0].numeroCiclosBloqueado = 20;
                    cout << "Requisitou HD" << endl;
                    filaHD.push_back(executando.front());
                    executando.erase(executando.begin());
                } else {
                    if (dispositivo == video) {
                        executando[0].numeroCiclosBloqueado = 10;
                        cout << "Requisitou Video" << endl;
                        filaVideo.push_back(executando.front());
                        executando.erase(executando.begin());
                    } else {
                        executando[0].numeroCiclosBloqueado = 30;
                        cout << "Requisitou Impressora" << endl;
                        filaImpressora.push_back(executando.front());
                        executando.erase(executando.begin());
                    }
                }
            }

            executando[0].numeroCiclos--;
            executando[0].numeroCiclosRodados++;
            Sleep(50);
        }

        if ((executando.size() > 0) && (executando[0].numeroCiclosRodados == 50)) {
            executando[0].estado = apto;
            executando[0].numeroCiclosRodados = 0;
            cout << "Foi enviado para aptos" << endl;
            filaAptos.push_back(executando.front());
            executando.erase(executando.begin());
        }
        numeroTotalDeCiclos++;
    }

    //system("cls");
    cout << "Numero total de processos criados: " << contadorDeProcessos << endl;;
    cout << "Numero total de ciclos: " << numeroTotalDeCiclos << endl;
    cout << "Numero ciclos medio por processo: " << numeroTotalDeCiclos/contadorDeProcessos << endl;
}

int main(void){
    int numero;
    cin >> numero;
    srand((unsigned)time(NULL));
    simulador(numero);
}
