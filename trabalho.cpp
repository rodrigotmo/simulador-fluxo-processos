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
    int numeroCiclos;
    int numeroCiclosBloqueado;
};

//=================Variáveis Globais==========================================

int criacao = 1, apto = 2, execucao = 3, bloqueado = 4, destruicao = 5;
int contadorDeProcessos = 0;
int processosDestruidos = 0;
int numeroTotalDeCiclos = 0;
int numeroCiclosRodado = 0;
int numeroCiclosRodadoBloqueado = 0;
vector<Processo> executando;
vector<Processo> filaAptos;
vector<Processo> filaBloqueados;
vector<Processo> listaDestruidos;

//=============================================================================

void criaProcesso() {
    contadorDeProcessos++;
    Processo processoCriado;
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
    do {
        int chanceNovoProcesso = rand() % 100;
        if (((chanceNovoProcesso > 0) && (chanceNovoProcesso <= 20)) && (contadorDeProcessos < numeroMaximoProcessos)) {
            criaProcesso();
        }
        if ((filaAptos.size() > 0) && (filaAptos[0].numeroCiclos > 0)) {
            executando.insert(executando.begin(), filaAptos.front());
            filaAptos.erase(filaAptos.begin());
            numeroCiclosRodado = 0;
            numeroCiclosRodadoBloqueado = 0;
            for (int i = 0; i < 50; i++) {
                if (executando[0].numeroCiclos == 0 ) {
                    executando[0].estado = destruicao;
                    processosDestruidos++;
                    listaDestruidos.insert(listaDestruidos.begin(), executando.front());
                    executando.erase(executando.begin());
                    cout << "Destruido" << endl;
                    break;
                } else {
//                        if (executando[0].estado == bloqueado) {
//                            if (executando[0].numeroCiclosBloqueado == 0) {
//                                executando[0].estado = apto;
//                                printf("Agora esta apto!\n");
//                            } else {
//                                printf("PID do processo: %i - ", executando[0].pid);
//                                printf("Estado do processo: %i - ", executando[0].estado);
//                                printf("Processador livre! Numero total de ciclos: %i ", numeroTotalDeCiclos);
//                                printf("Processo Bloqueado por %i ciclos! \n", executando[0].numeroCiclosBloqueado);
//                                executando[0].numeroCiclosBloqueado--;
//                                numeroTotalDeCiclos++;
//                                Sleep(50);
//                            }
//                        } else {
                        if (executando[0].estado == apto) {
                            executando[0].estado = execucao;
                            cout << "PID do processo: " <<  executando[0].pid << " - ";
                            cout << "Estado do processo: " << executando[0].estado << " - ";
                            cout << "Numero ciclos: " << executando[0].numeroCiclos << " - ";
                            cout << "Numero total de ciclos: " << numeroTotalDeCiclos << endl;
//                                int chanceES = rand() % 100;
//                                int dispositivo = rand() % 3;
//                                if (chanceES == 1) {
//                                    printf("chamou ES\n");
//                                    executando[0].estado = bloqueado;
//                                    if (dispositivo == 1) {
//                                        executando[0].numeroCiclosBloqueado = 20;
//                                    } else {
//                                        if (dispositivo == 2) {
//                                            executando[0].numeroCiclosBloqueado = 10;
//                                        } else {
//                                            executando[0].numeroCiclosBloqueado = 60;
//                                        }
//                                    }
//                                }
                            executando[0].numeroCiclos--;
                            numeroTotalDeCiclos++;
                            Sleep(50);
                        }
//                        }
                }
            }
            if ((executando.size() > 0) && (executando[0].estado != bloqueado)) {
                executando[0].estado = apto;
                filaAptos.push_back(executando[0]);
                executando.erase(executando.begin());
            }
        }
    } while(processosDestruidos < numeroMaximoProcessos);
    cout << "Numero total de processos criados: " << contadorDeProcessos << endl;
    cout << "Numero total de ciclos: " << numeroTotalDeCiclos << endl;
    cout << "Numero ciclos medio por ciclo: " << numeroTotalDeCiclos/contadorDeProcessos << endl;
}

int main(void){
    int numero;
    cin >> numero;
    srand((unsigned)time(NULL));
    simulador(numero);
}



































//int processador(Processo processoAtual) {
//    int i;
//    numeroCiclosRodado = 0;
//    for (i = 0; i < 50; i++) {
//        if (processoAtual.numeroCiclos == 0 ) {
//            processoAtual.estado = destruicao;
//            processosDestruidos++;
//            printf("\ndestruido!\n");
//            break;
//        } else {
//            if (processoAtual.estado == bloqueado) {
//                if (processoAtual.numeroCiclosBloqueado == 0) {
//                    processoAtual.estado == apto;
//                    numeroTotalDeCiclos++;
//                    break;
//                } else {
//                    printf("Processo Bloqueado por %i ciclos! ", processoAtual.numeroCiclosBloqueado);
//                    printf("Processador livre!\n");
//                    processoAtual.numeroCiclosBloqueado--;
//                    numeroTotalDeCiclos++;
//                    numeroCiclosRodadoBloqueado++;
//                }
//            } else {
//                processoAtual.estado = execucao;
//                printf("PID do processo: %i - ", processoAtual.pid);
//                printf("Estado do processo: %i - ", processoAtual.estado);
//                printf("Numero ciclos: %i ", processoAtual.numeroCiclos);
//                printf("Numero total de ciclos: %i\n", numeroTotalDeCiclos);
//                int chanceES = rand() % 100;
//                int dispositivo = rand() % 3;
//                if (chanceES == 1) {
//                    printf("chamou ES\n");
//                    processoAtual.estado = bloqueado;
//                    if (dispositivo == 1) {
//                        processoAtual.numeroCiclosBloqueado = 20;
//                    } else {
//                        if (dispositivo == 2) {
//                            processoAtual.numeroCiclosBloqueado = 10;
//                        } else {
//                            processoAtual.numeroCiclosBloqueado = 60;
//                        }
//                    }
//                }
//                processoAtual.numeroCiclos--;
//                numeroCiclosRodado++;
//                numeroTotalDeCiclos++;
//                Sleep(50);
//            }
//        }
//    }
//    printf("          50 ciclos\n");
//    if (numeroCiclosRodado != 0) {
//        return numeroCiclosRodado;
//    } else {
//        return numeroCiclosRodadoBloqueado;
//    }
//}
