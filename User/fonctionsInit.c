/* === Fonctions d'initialisations === */

#include "stm32f4_discovery.h"

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  	GPIOD_InitStructure;
	GPIO_InitTypeDef  	GPIOA_InitStructure;

	/* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	 //OUPUT
	GPIOD_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIOD_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIOD_InitStructure);  			//Application des paramètres



    /* GPIOA Periph clock enable */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  	/* Configure PA0 in input mode */
  	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_IN;	 		//input
  	GPIOA_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIOA_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//Pas de pull/up/DN
  	GPIO_Init(GPIOA, &GPIOA_InitStructure);			//Application des paramètres
}


void Init_Uart2(unsigned long baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStruct; 
	NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 	//Enable clk gpioA
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);		//Enable clk usart2
  	/*
  	*  USART2_TX -> PA2 , USART2_RX ->	PA3
	*/			
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);	 	//TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);		//RX
											 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 				//TX
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_100MHz; 
  	GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  				//RX
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	USART_InitStruct.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
	USART_InitStruct.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
	USART_Init(USART2, &USART_InitStruct);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 		// enable the USART2 receive interrupt
//	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 		//  the USART2 receive interrupt


	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		 		// we want to configure the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	// this sets the priority group of the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 	// this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 	// the USART2 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);							 	// the properties are passed to the NVIC_Init function which takes care of the low level stuff

	// finally this enables the complete USART2 peripheral
	USART_Cmd(USART2, ENABLE);

}

void Init_Uart3(unsigned long baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStruct; 
	NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	 	//Enable clk gpioD
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);		//Enable clk usart3
  	/*
  	* USART3_RX ->	PD9
	*/			
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);		//RX
											 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	        
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  				//RX
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
  	GPIO_Init(GPIOD, &GPIO_InitStructure);




	USART_InitStruct.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
	USART_InitStruct.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
	USART_Init(USART3, &USART_InitStruct);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 		// enable the USART3 receive interrupt
//	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE); 		//  the USART3 receive interrupt


	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		 		// we want to configure the USART3 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	// this sets the priority group of the USART3 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		 	// this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 	// the USART2 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);							 	// the properties are passed to the NVIC_Init function which takes care of the low level stuff

	// finally this enables the complete USART2 peripheral
	USART_Cmd(USART3, ENABLE);

}

void Timer2_Config(void)
{
 	//==========Déclarations locales==============
	//#define APB1PSC  4
	#define TM2_PSC_VAL 8400-1	//valeur du prescaler pour 10Khz

	//#define  TM2_CCR1_VAL = 40961;

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;
	//============================================


  	/* TIM2 clock enable */ //Attention de bien vérifier quelle fonction RCC utiliser selon le periph!
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  	/* Enable the TIM2 gloabal Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
 // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		Laissé par défaut
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 10;  //la valeur du registre l'autoreload	pour 1 ms (peut être codé sur 32 bits)
	TIM_TimeBaseStructure.TIM_Prescaler = TM2_PSC_VAL;
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;	//pas d'autre clk division  et surtout n'a aucun effet???!
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//application des paramètres
		
																

	TIM_SetCounter  (TIM2, 0x00000000);   	//le compteur est à 0

  	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//clear le flag

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 		//interruption lors d'un overflow

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);

}


void EXTILine0_Config(void)
{
  	//==========Déclarations locales==============
	EXTI_InitTypeDef	EXTI1_InitStructure; 
	NVIC_InitTypeDef  NVIC1_InitStructure;
	//============================================

 	//EXTI Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	//Activation du clock SYSCONFIG sur APB2	

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); 	//PA0 est associé à une ligne d'interruption externe

	EXTI1_InitStructure.EXTI_Line = EXTI_Line0;					//Ligne d'interruption n°0 puisque c'est un P0
	EXTI1_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  	//Mode par interruption
	EXTI1_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	//Front (relachement)
   EXTI1_InitStructure.EXTI_LineCmd = ENABLE;		//Activation de la ligne de commande
	EXTI_Init(&EXTI1_InitStructure);  				//Application des paramètres

     /* Enable and set EXTI Line0 Interrupt to the highest priority */
	NVIC1_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  			//IRQ avec ligne EXTI0
	NVIC1_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;	//priorité 5
	NVIC1_InitStructure.NVIC_IRQChannelSubPriority = 0x05; 		//priorité max
	NVIC1_InitStructure.NVIC_IRQChannelCmd = ENABLE; 	//Autorisation de l'interruption
	NVIC_Init(&NVIC1_InitStructure);					//application des paramètres

}

