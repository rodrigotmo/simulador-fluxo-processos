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
vector<Processo> filaAptos;
vector<Processo> filaBloqueados;

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
    filaAptos[contadorDeProcessos-1] = processoCriado;
}

void simulador(int numeroMaximoProcessos) {
    do {
        int chanceNovoProcesso = rand() % 100;
        if (((chanceNovoProcesso > 0) && (chanceNovoProcesso <= 20)) && (contadorDeProcessos != numeroMaximoProcessos)) {
            criaProcesso();
        }
        if (arrayProcesso[0].numeroCiclos != 0) {
            numeroCiclosRodado = 0;
            numeroCiclosRodadoBloqueado = 0;
            for (int i = 0; i < 50; i++) {
                if (arrayProcesso[0].numeroCiclos == 0 ) {
                    arrayProcesso[0].estado = destruicao;
                    processosDestruidos++;
                    printf("\ndestruido!\n");
                    break;
                } else {
                    if (arrayProcesso[0].estado == bloqueado) {
                        if (arrayProcesso[0].numeroCiclosBloqueado == 0) {
                            arrayProcesso[0].estado = apto;
                            printf("Agora esta apto!\n");
                        } else {
                            printf("PID do processo: %i - ", arrayProcesso[0].pid);
                            printf("Estado do processo: %i - ", arrayProcesso[0].estado);
                            printf("Processador livre! Numero total de ciclos: %i ", numeroTotalDeCiclos);
                            printf("Processo Bloqueado por %i ciclos! \n", arrayProcesso[0].numeroCiclosBloqueado);
                            arrayProcesso[0].numeroCiclosBloqueado--;
                            numeroTotalDeCiclos++;
                            Sleep(50);
                        }
                    } else {
                        if (processos[0].estado == apto) {
                            arrayProcesso[0].estado = execucao;
                            printf("PID do processo: %i - ", arrayProcesso[0].pid);
                            printf("Estado do processo: %i - ", arrayProcesso[0].estado);
                            printf("Numero ciclos: %i ", arrayProcesso[0].numeroCiclos);
                            printf("Numero total de ciclos: %i \n", numeroTotalDeCiclos);
                            int chanceES = rand() % 100;
                            int dispositivo = rand() % 3;
                            if (chanceES == 1) {
                                printf("chamou ES\n");
                                arrayProcesso[0].estado = bloqueado;
                                if (dispositivo == 1) {
                                    arrayProcesso[0].numeroCiclosBloqueado = 20;
                                } else {
                                    if (dispositivo == 2) {
                                        arrayProcesso[0].numeroCiclosBloqueado = 10;
                                    } else {
                                        arrayProcesso[0].numeroCiclosBloqueado = 60;
                                    }
                                }
                            }
                            arrayProcesso[0].numeroCiclos--;
                            numeroTotalDeCiclos++;
                            Sleep(50);
                        }
                    }
                }
            }
            //printf("50 ciclos!\n");
            if (processos[0].estado != bloqueado) {
                //printf("Agora esta APTO! \n");
                processos[0].estado = apto;
            }
//            if (numeroCiclosRodado != 0) {
//                arrayProcesso[0].numeroCiclos = arrayProcesso[0].numeroCiclos - numeroCiclosRodado;
//                arrayProcesso[0].numeroCiclosBloqueado = arrayProcesso[0].numeroCiclosBloqueado - numeroCiclosRodadoBloqueado;
//            } else {
//                arrayProcesso[0].numeroCiclosBloqueado = arrayProcesso[0].numeroCiclosBloqueado - numeroCiclosRodadoBloqueado;
//            }
        }
    } while(processosDestruidos != numeroMaximoProcessos);
    system("cls");
    printf("Numero total de processos criados: %i\n", contadorDeProcessos);
    printf("Numero total de ciclos: %i\n", numeroTotalDeCiclos);
    printf("Numero ciclos medio por ciclo: %i\n", numeroTotalDeCiclos/contadorDeProcessos);
    for (int i = 0; i < 5; i++) {
        int contador = 0;
        if (filaAptos[i].processo) {
            contador++;
        }
        printf("Tamanho filaAptos: %i\n", contador);
    }
}

int main(void){
    int numero;
    scanf("%i", &numero);
    //scanf("%i", &tempo);
    getchar();
//    Processo processoCriado;
//    srand((unsigned)time(NULL));
//    int n = 100 + (rand() % 200);
//    processoCriado.numeroCiclos = n;
//    processoCriado.pid = 1;
//    processos[0] = processoCriado;

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
