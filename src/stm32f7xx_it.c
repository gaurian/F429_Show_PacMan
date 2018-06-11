/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx.h"
#include "stm32f7xx_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

	  if(Gui_Touch_Timer_ms!=0) {
	    Gui_Touch_Timer_ms--;
	  }

	  if(Mode_Systic_Timer_ms!=0) {
	    Mode_Systic_Timer_ms--;
	  }

	  if(Player_Systick_Timer_ms!=0) {
	    Player_Systick_Timer_ms--;
	  }

	  if(Blinky_Systic_Timer_ms!=0) {
	    Blinky_Systic_Timer_ms--;
	  }

	  if(Pinky_Systic_Timer_ms!=0) {
	    Pinky_Systic_Timer_ms--;
	  }

	  if(Inky_Systic_Timer_ms!=0) {
	    Inky_Systic_Timer_ms--;
	  }

	  if(Clyde_Systic_Timer_ms!=0) {
	    Clyde_Systic_Timer_ms--;
	  }

}
