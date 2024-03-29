/*
 * --------------------------- Exercício 03 ----------------------------------
 *
 * Egydio Tadeu Gomes Ramos
 *
 *         - Implementações das funcionalidades do kernel
 */

// ----------------------------------------------------------------------------

/*
 * --------------- Inclusão das bibliotecas auxiliares ------------------------
 */

#include "kernel.h"

// ----------------------------------------------------------------------------

/*
 * ----------------------- Definição das Funções Auxiliares -------------------
 */

void osKernelInit()
{
  __disable_irq();
  MILLIS_PRESCALER = BUS_FREQ/1000;
}

void osKernelLaunch(uint32_t quanta)
{
  SysTick->CTRL = 0;
  SysTick->VAL = 0;
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
  SysTick->LOAD = (quanta*MILLIS_PRESCALER) - 1;
  SysTick->CTRL = 0x00000007;
  osSchedulerLaunch();
}


void osKernelAddTasks(void (*task0)(),
                      void (*task1)(),
                      void (*task2)())
{
  // Inicializa o apontador nextPt para formar a lista circular
  // e a pilha de cada tarefa
  for (int i = 0; i < NUMBER_OF_TASKS; i++)
    {
      if (i == 0)
        {
          tcbs[i].nextPt = &tcbs[NUMBER_OF_TASKS-1]; // T1 -> T3
        }
      else
        {
          tcbs[i].nextPt = &tcbs[i-1]; // T2 -> T1, T3 -> T2, ...
        }
      osKernelStackInit(i);
    }

  // Inicializa o PC de cada pilha
  TCBS_STACK[0][STACK_SIZE-2] = (uint32_t) task0;
  TCBS_STACK[1][STACK_SIZE-2] = (uint32_t) task1;
  TCBS_STACK[2][STACK_SIZE-2] = (uint32_t) task2;

  currentPt = &tcbs[0];
}


void osKernelStackInit(uint32_t taskIndex)
{
  // Inicializa o valor de xPSR;
  TCBS_STACK[taskIndex][STACK_SIZE-1] = 1U << 24;
  // Inicializa o apontador de pilha na posição do registrador R4
  tcbs[taskIndex].stackPt = &TCBS_STACK[taskIndex][STACK_SIZE-16];
}

void osSchedulerLaunch()
{
  /*
   * Sequência de instruções:
   *    1) Carregar R0 com currentPt
   *    2) Carregar R2 com o valor do endereço em R0
   *    3) Carregar SP (R13) com o valor do endereço em R2
   *    4) Liberar R3-R11
   *    5) Liberar R0-R3
   *    6) Liberar R12
   *    7) Deslocar SP em 4 bytes
   *    8) Liberar LR (R14)
   *    9) Deslocar SP em 4 bytes
   */
  __asm(
      "LDR R0, =currentPt\n\t"
      "LDR R2, [R0]\n\t"
      "LDR R13, [R2]\n\t"
      "POP {R4-R11}\n\t"
      "POP {R0-R3}\n\t"
      "POP {R12}\n\t"
      "ADD R13, #4\n\t"
      "POP {R14}\n\t"
      "ADD R13, #4"
   );
  __enable_irq();
}

void SysTick_Handler()
{
  /*
   * Sequência de instruções:
   *    1) Colocar R4-R13 na pilha
   *    2) Carregar R0 com currentPt
   *    3) Carregar R1 com o valor do endereço em R0
   *    4) Guargar conteúdo de R13 no endereço apontado por R1
   *    5) Deslocar R1
   *    6) Guardar R1 no endereço apontado por R0
   *    7) Carregar R13 com valor do endereço apontado por R1
   *    8) Liberar R4-R11
   */
  __disable_irq();
  save_context();
  restore_context();
  __enable_irq();
}

void osSemaphoreInit(int32_t *semaphore, int32_t initial_value)
{
  //__disable_irq();
  *semaphore = initial_value;
  //__enable_irq();
}

void osSemaphorePost(int32_t *semaphore)
{
  __disable_irq();
  *semaphore += 1;
  __enable_irq();
}

void osSemaphorePend(int32_t *semaphore)
{
  __disable_irq();
  while((*semaphore) < SEMAPHORE_INIT_VALUE)
    {
      __enable_irq();
      //osTaskYield();
      __disable_irq();
    }
  *semaphore -= 1;
  __enable_irq();
}

void osTaskYield()
{
  SysTick->VAL = 0;
  INITCTRL = 0x04000000;
}

void osGroupEventInit(uint8_t *event_group, uint8_t initial_value)
{
  *event_group = initial_value;
}

void osGroupEventSync(uint8_t *event_group, uint8_t task_bit, uint8_t sync_byte)
{
  __disable_irq();
  *event_group |= task_bit;

  while((*event_group & task_bit) == task_bit)
    {
      __enable_irq();
      if((*event_group & sync_byte) == sync_byte)
        {
          *event_group = 0;
        }
      __disable_irq();
    }

  __enable_irq();
}

void osMailBoxPost(int32_t *mailBox, int32_t *message)
{
  __disable_irq();
  while(mailBox != NULL)
    {
      __enable_irq();
      osTaskYield();
      __disable_irq();
    }

  mailBox = message;
  __enable_irq();
}

void osMailBoxReceive(int32_t *mailBox, int32_t *message)
{
  __disable_irq();
  while(mailBox == NULL)
    {
      __enable_irq();
      osTaskYield();
      __disable_irq();
    }
  message = mailBox;
  mailBox = NULL;
  __enable_irq();
}
// ----------------------------------------------------------------------------
