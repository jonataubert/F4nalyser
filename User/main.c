//******************************************************************************
// F4nalyser main source code
//******************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f4xx.h"

#include "stm32f4_discovery.h"
#include "stm32f4_discovery_sdio_sd.h"
#include "fonctionsInit.h"
#include "fonctionsUsart.h"
#include "itData.h"
#include "fifo.h"

#include "writeToFile.h"


#define DBG


//******************************************************************************

void NVIC_Configuration(void);



//******************************************************************************



#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof lfn;
#endif


const char head_txt2[] = "\r\n\r\nNouvelle capture UART2\r\n\r\n";
const char head_txt3[] = "\r\n\r\nNouvelle capture UART3\r\n\r\n";
const char file2_path[] = "UART2.TXT";
const char file3_path[] = "UART3.TXT";

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
	
	//================= Initialisation pin et interruptions =============================

	GPIO_Configuration();
 	NVIC_Configuration(); 	/* Interrupt Config SDIO*/
	EXTILine0_Config();		// Interrupt PA0
	Timer2_Config();		//Configure timer2 1[s]
	Init_Uart2(4800);
	Init_Uart3(4800);
	Init_SDCard();			//Mount sd card and use fat32 format

	fileUart2 = initialiserFile();	//Init fifo pile
	fileUart3 = initialiserFile();	//Init fifo pile

	
  	while(1) 	/* Infinite loop */
  	{
		switch(etatBtBleu)
		{
			case START:
				enableUsartIt();
				GPIO_SetBits(GPIOD, GPIO_Pin_12); 			//Turn on green led
				led_gr_blink = 1;							//Activate the blink mod in an interruption
	
				writeHeader(head_txt2[], file2_path);		//header for the usart2 file
				writeHeader(head_txt3[], file3_path);		//header for the usart3 file

				etatBtBleu = RUN;
			break;
	
			case STOP:
				disableUsartIt();							//we will read the fifo pile so it's better to inibit usart interrupt
				led_gr_blink = 0;						
				GPIO_ResetBits(GPIOD, GPIO_Pin_12); 		//green led	off
				GPIO_SetBits(GPIOD, GPIO_Pin_15);			//blue led on
	
				writeDynamicTabData(pullN(fileUart2, nbCaractUart2), nbCaractUart2, file2_path);			//write all the fifo's uart2 data
				writeDynamicTabData(pullN(fileUart3, nbCaractUart3), nbCaractUart3, file3_path);			//write all the fifo's uart3 data
	
				GPIO_ResetBits(GPIOD, GPIO_Pin_15);			//blue led off

				nbCaractUart2 = 0;
				nbCaractUart3 = 0;
				etatBtBleu=WAIT;
	
			break;

			case RUN:
				//=========== write data when we have NB_CARACT_STOCK into the usart2 fifo pile =============
				if(flag_fileUart2Ready)
				{
					flag_fileUart2Ready=0;
					writeDynamicTabData(pullN(fileUart2, NB_CARACT_STOCK), NB_CARACT_STOCK, file2_path);	//write NB_CARACT_STOCK of uart2 data
					GPIO_ResetBits(GPIOD, GPIO_Pin_15);		//blue led off
				}
				
				//=========== write data when we have NB_CARACT_STOCK into the usart3 fifo pile =============
				if(flag_fileUart3Ready)
				{
					flag_fileUart3Ready = 0;
					writeDynamicTabData(pullN(fileUart3, NB_CARACT_STOCK), NB_CARACT_STOCK, file3_path);	//write NB_CARACT_STOCK of uart3 data
					GPIO_ResetBits(GPIOD, GPIO_Pin_15);		//blue led off 
				}
			break;
		}	
  	} 
}

//******************************************************************************

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************************/





//******************************************************************************
// Hosting of stdio functionality through USART2
//******************************************************************************

#include <rt_misc.h>

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f)
{
	static int last;

	if ((ch == (int)'\n') && (last != (int)'\r'))
	{
		last = (int)'\r';

  	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

 	  USART_SendData(USART2, last);
	}
	else
		last = ch;

	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

  USART_SendData(USART2, ch);

  return(ch);
}

int fgetc(FILE *f)
{
	char ch;

	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

	ch = USART_ReceiveData(USART2);

  return((int)ch);
}

int ferror(FILE *f)
{
  /* Your implementation of ferror */
  return EOF;
}

void _ttywrch(int ch)
{
	static int last;

	if ((ch == (int)'\n') && (last != (int)'\r'))
	{
		last = (int)'\r';

  	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

 	  USART_SendData(USART2, last);
	}
	else
		last = ch;

	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

  USART_SendData(USART2, ch);
}

void _sys_exit(int return_code)
{
label:  goto label;  /* endless loop */
}

//******************************************************************************

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while(1); /* Infinite loop */
}
#endif

//******************************************************************************
