/*
 * --------------------------- Exercício 03 ----------------------------------
 *
 * Egydio Tadeu Gomes Ramos
 *
 * Kernel.
 *
 *  - Implementação de um bloco de controle de tarefas básicos
 *  - Algoritmo de escalonamento de tarefas usando lista circular
 *  - Teste realizado utilizando três tasks de contadores
 */

// ----------------------------------------------------------------------------


/*
 * --------------- Inclusão das bibliotecas auxiliares ------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "kernel.h"


// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

// ----------------------------------------------------------------------------

/*
 * ------------- Definição de constantes e variáveis globais ------------------
 */

// Declaração dos contadores para as tasks
uint32_t c1 = 0, c2 = 0, c3 = 0;

// Declaração dos semáforos

int32_t semaphore1, semaphore2;

//Criação do grupo de eventos
uint8_t event_bits;


// ----------------------------------------------------------------------------

/*
 * ---------------- Protótipos das funções auxiliares ------------------------
 */

// ----------------------------------------------------------------------------

/*
 * ---------------- Protótipos das funções de Tasks ---------------------------
 */

void count1();
void count2();
void count3();

// Apontadores para cada tarefa

void (*count1_task)() = count1;
void (*count2_task)() = count2;
void (*count3_task)() = count3;

// ----------------------------------------------------------------------------

/*
 * -------------------------- Função Main -------------------------------------
 */

int
main(int argc, char* argv[])
{
  __disable_irq();
  osGroupEventInit(&event_bits, EVENT_GROUP_INIT);
  //osSemaphoreInit(&semaphore1, SEMAPHORE_INIT_VALUE);
  //osSemaphoreInit(&semaphore2, SEMAPHORE_INIT_VALUE);
  osKernelInit();
  osKernelAddTasks(count1_task, count2_task, count3_task);
  osKernelLaunch(1);
  while (1)
    {

    }

  return 0;
}

// ----------------------------------------------------------------------------

/*
 * ----------------------- Definição das Tasks --------------------------------
 */

void count1()
{
  while(1)
    {
      c1 += 1;

      //osSemaphorePend(&semaphore1);
      //osSemaphorePost(&semaphore2);
      osGroupEventSync(&event_bits, TASK0_BIT, ALL_SYNC_BITS);
    }
}

void count2()
{
  while(1)
    {
      c2 += 1;

      //osSemaphorePend(&semaphore2);
      //osSemaphorePost(&semaphore1);
      osGroupEventSync(&event_bits, TASK1_BIT, ALL_SYNC_BITS);

    }
}

void count3()
{
  while(1)
    {
      c3 += 1;

      osGroupEventSync(&event_bits, TASK2_BIT, ALL_SYNC_BITS);

    }
}

// ----------------------------------------------------------------------------

/*
 * ----------------------- Definição das Funções Auxiliares -------------------
 */


// ----------------------------------------------------------------------------

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
