/**
  ******************************************************************************
  * @file    IO_Toggle/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stm32f4_discovery_sdio_sd.h"
#include "fonctionsUsart.h"
#include "itData.h"
#include "fifo.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup IO_Toggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/





/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */		   




	// this is the interrupt request handler (IRQ) for ALL USART2 interrupts
void USART2_IRQHandler(void)
{
	// check if the USART2 receive interrupt flag was set
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		if(isUsartOk())
		{
			char caract_rec = USART2->DR; // the character from the USART2 data register is saved in caract_rec
			GPIO_ToggleBits(GPIOD, GPIO_Pin_13);	//led activity
			push(fileUart2, caract_rec); //le caractere recu est stocké dans la file

			//on compte le nb de caracteres, qd egale au nombre voulu on met un flag à 1
			nbCaractUart2++;
			nbCaractUart2%=NB_CARACT_STOCK;	  
			if(nbCaractUart2 == 0)
				flag_fileUart2Ready = 1;
		}
		
		USART_ClearITPendingBit(USART2,  USART_IT_RXNE);
	}
}

void USART3_IRQHandler(void)
{
	// check if the USART3 receive interrupt flag was set
	if(USART_GetITStatus(USART3, USART_IT_RXNE))
	{
		if(isUsartOk())
		{
			char caract_rec = USART3->DR;
			GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
			push(fileUart3, caract_rec); //le caractere recu est stocké dans la file

			//on compte le nb de caracteres, qd egale au nombre voulu on met un flag à 1
			nbCaractUart3++;
			nbCaractUart3%=NB_CARACT_STOCK;	  
			if(nbCaractUart3 == 0)
				flag_fileUart3Ready = 1;	
		}		
		USART_ClearITPendingBit(USART3,  USART_IT_RXNE);
	}
}


//--------------------------------Timers----------------------------------------

void TIM2_IRQHandler(void)		  	//interruption timer2 (Attention elle est appelée au démarrage du programme, je ne sais pas encore pourquoi...
{
	static unsigned int time_led = 1;
	static unsigned int time_debounce_btn = 1;
	EXTI_InitTypeDef	EXTI1_InitStructure; 

	if(TIM_GetITStatus (TIM2, TIM_IT_Update) != RESET) 	//Est-ce une interruption update (car autoreload) 
	{
		if(time_led >= 1000) //each seconds
		{	
			time_led	= 1;
			if(led_gr_blink)
			{
 				GPIO_ToggleBits(GPIOD, GPIO_Pin_12); 	//Toggle la led verte

			}
		}	

		if(debounce_btn_en)	//Debounce int	
		{
			if(time_debounce_btn >= 20)	//20 ms	
			{

				EXTI_ClearFlag(EXTI_Line0);	//Clear flags 
				EXTI1_InitStructure.EXTI_Line = EXTI_Line0;					//Ligne d'interruption n°0 puisque c'est un P0
				EXTI1_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  	//Mode par interruption
				EXTI1_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	//Front (relachement)
			  EXTI1_InitStructure.EXTI_LineCmd = ENABLE;		//Activation de la ligne de commande
				EXTI_Init(&EXTI1_InitStructure);  				//Application des paramètres
				debounce_btn_en = 0;
				time_debounce_btn = 1;	
			}
			time_debounce_btn++;
		}
		 	

		time_led++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//clear le flag
	}
}


//--------------------------------Interruption externe---------------------------
void EXTI0_IRQHandler(void)
{
  	if(EXTI_GetITStatus(EXTI_Line0) != RESET)	   //Est-ce que bp relaché?
	{
		EXTI_InitTypeDef	EXTI1_InitStructure; 

		EXTI1_InitStructure.EXTI_Line = EXTI_Line0;					//Ligne d'interruption n°0 puisque c'est un P0
		EXTI1_InitStructure.EXTI_LineCmd = DISABLE;	//Désactivation de la ligne de commande (on ne veut plus d'interrupt
		EXTI_Init(&EXTI1_InitStructure);  				//Application des paramètres

		etatBtBleu++;
		etatBtBleu%=2;
		debounce_btn_en = 1;
		
    	/* Clear the EXTI line 0 pending bit */
    	EXTI_ClearITPendingBit(EXTI_Line0);
  	}


}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
