/*
 * --------------------------- Exercício 02 ----------------------------------
 *
 * Egydio Tadeu Gomes Ramos
 *
 * Stack Frame.
 *
 *  - Implementa duas tasks blink_orange e blink_green
 *  - São criadas as stack frames para cada task
 *  - Na interrupção do systick, o contexto é mudado manualmente alterando
 *    o valor do apontador de pilha (sp)
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
  osKernelInit();
  osKernelAddTasks(count1_task, count2_task, count3_task);
  osKernelLaunch(10);
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
  uint32_t c1 = 0;

  while(1)
    c1 += 1;
}

void count2()
{
  uint32_t c2 = 0;

  while(1)
    c2 += 2;
}

void count3()
{
  uint32_t c3 = 0;

  while(1)
    c3 += 3;
}

// ----------------------------------------------------------------------------

/*
 * ----------------------- Definição das Funções Auxiliares -------------------
 */


// ----------------------------------------------------------------------------

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
