/*
 * --------------------------- Exercício 03 ----------------------------------
 *
 * Egydio Tadeu Gomes Ramos
 *
 *         - Definições das funcionalidades do kernel
 */

// ----------------------------------------------------------------------------

#ifndef INCLUDE_KERNEL_H_
#define INCLUDE_KERNEL_H_

/*
 * --------------- Inclusão das bibliotecas auxiliares ------------------------
 */

#include "stm32f4xx.h"

// ----------------------------------------------------------------------------

/*
 * ------------- Definição de constantes e variáveis globais ------------------
 */

#define BUS_FREQ SystemCoreClock
#define NUMBER_OF_TASKS 3
#define STACK_SIZE 100
#define SEMAPHORE_INIT_VALUE 0

// Criação do bloco de controle de tarefas

struct Tcb
{
  int32_t *stackPt;
  struct Tcb *nextPt;
};

typedef struct Tcb tcbType;
tcbType tcbs[NUMBER_OF_TASKS];
tcbType *currentPt;
int32_t TCBS_STACK[NUMBER_OF_TASKS][STACK_SIZE];

// Variável para escala de tempo

uint32_t MILLIS_PRESCALER;

// Semáforo spinlock

int32_t semaphore;

// ----------------------------------------------------------------------------

/*
 * ---------------- Protótipos das funções auxiliares ------------------------
 */

void osKernelInit();
void osKernelLaunch(uint32_t quanta);
void osKernelAddTasks(void (*task0)(),
                      void (*task1)(),
                      void (*task2)());



// Funções de manipulação das pilhas de tarefas

/*
 * Inicializa a pilha TCBS_STACK[taskIndex] e o apontador do TCB para a pilha
 */
void osKernelStackInit(uint32_t taskIndex);

void osSchedulerLaunch();

// Funções de manipulação do semáforo

/*
 * Atribui um valor inicial ao semáforo
 */
void osSemaphoreInit(int32_t *semaphore, int32_t initial_value);

/*
 * Disponibiliza o semáforo
 */
void osSemaphorePost(int32_t *semaphore);

/*
 * Espera disponibilidade do semáforo
 */
void osSemaphorePend(int32_t *semaphore);

// Para utilizar kernel cooperativo
void osTaskYield();

// ----------------------------------------------------------------------------

#endif /* INCLUDE_KERNEL_H_ */
