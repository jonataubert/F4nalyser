//******************************************************************************
// STM32F4 Discovery SDCard + FatFs Test - CLIVE - SOURCER32@GMAIL.COM
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

#include "ff.h"
#include "diskio.h"
//#include "stdarg.h"


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
	//================ Variables locales ===================//

	FRESULT res;		// File function return code (si exception ou ok)	
	FILINFO finf2; 		// Informations du fichier
	FILINFO finf3; 		// Informations du fichier
	FIL fil2;		  	// File object structure
	FIL fil3;		  	// File object structure
//	DIR dir;		   	// Directory object structure
	FATFS fs32;		 	// File system object structure
  	
	UINT BytesWritten;	//Info sur le nombre de bytes écrits dans le fichier
	
	//================= Initialisation pin et interruptions =============================

	GPIO_Configuration();
 	NVIC_Configuration(); /* Interrupt Config SDIO*/
	EXTILine0_Config();	// Interrupt PA0
	Timer2_Config();		//Configure timer2 1[s]
	Init_Uart2(4800);
	Init_Uart3(4800);

	fileUart2 = initialiserFile();
	fileUart3 = initialiserFile();

  	//========================= Montage de la carte SD ==================================
	memset(&fs32,0, sizeof(FATFS));	 		//initialise l'objet fs32 avec des 0
	res = f_mount(0, &fs32);				 	//Monte la carte SD (device 0)

#ifdef DBG
	if (res != FR_OK)
		printf("res = %d f_mount\n", res);
	else
		USART_puts(USART2,"SD mounted\r\n");
#endif



	
  	while(1) 	/* Infinite loop */
  	{
	  

		switch(etatBtBleu)
		{
			case START:
				enableUsartIt();

				GPIO_SetBits(GPIOD, GPIO_Pin_12); 	//on la led verte 
				led_gr_blink = 1;
	
				//============== En-tête fichier 2 ==========================
				memset(&fil2, 0, sizeof(FIL));			//Initialise l'objet fichier2 
				memset(&finf2, 0, sizeof(FILINFO));
	
				res = f_open(&fil2, file2_path, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);	//ouvre /crée fichier txt à la racine
				#ifdef DBG
					if (res != FR_OK)
						printf("res = %d f_open UART2.TXT\n", res);
					else
						USART_puts(USART2,"File UART2.TXT created/opened\r\n");
				#endif
				
				res = f_stat(file2_path, &finf2); 	//get file's info
				#ifdef DBG
					printf("end of file %d \n", finf2.fsize);
				#endif
				
				res = f_lseek(&fil2, finf2.fsize);		//Aller à la fin du fichier
				if (res == FR_OK)
	 				res = f_write (&fil2, head_txt2, strlen(head_txt2), &BytesWritten);	 //Ecriture en-tête du fichier txt2
				#ifdef DBG
					if (res != FR_OK)
						printf("res = %d Problème écriture fichier UART2\n", res);
					else
						USART_puts(USART2,"File UART2.TXT written\r\n");
				#endif
				
				res = f_close(&fil2); // ferme le fichier 
	
				//============= En-tête fichier 3 =========================
				memset(&fil3, 0, sizeof(FIL));			//Initialise l'objet fichier3 
				memset(&finf3, 0, sizeof(FILINFO));
	
				res = f_open(&fil3, file3_path, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);	//ouvre /crée fichier txt à la racine
				#ifdef DBG
					if (res != FR_OK)
						printf("res = %d f_open UART3.TXT\n", res);
					else
						USART_puts(USART2,"File UART3.TXT created/opened\r\n");
				#endif
				
				res = f_stat(file3_path, &finf3);	//Obtaining file's infos
				#ifdef DBG
					printf("end of file %d \n", finf3.fsize);
				#endif
	
				res = f_lseek(&fil3, finf3.fsize);		//Aller à la fin du fichier
				if (res == FR_OK)
	  				res = f_write (&fil3,head_txt3, strlen(head_txt3), &BytesWritten);	 //Ecriture en-tête du fichier txt3
				#ifdef DBG
					if (res != FR_OK)
						printf("res = %d Problème écriture fichier UART3\n", res);
					else
						USART_puts(USART2,"File UART3.TXT written\r\n");
				#endif
	
				res = f_close(&fil3); // ferme le fichier3 
				etatBtBleu = RUN;
			break;
	
			case STOP:
				disableUsartIt();	
				led_gr_blink = 0;
				GPIO_ResetBits(GPIOD, GPIO_Pin_12); 	//green led	off

				//============= Vidage des piles dans la carte SD ===========================================================================
				GPIO_SetBits(GPIOD, GPIO_Pin_15);		//blue led on
	
				//============= Enregistre fichier 2 avec le contenu de la file ==============================
				res = f_open(&fil2, file2_path, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);	//ouvre /crée fichier2 txt à la racine
				res = f_stat(file2_path, &finf2); 		//get file's info
				res = f_lseek(&fil2, finf2.fsize);		//Aller à la fin du fichier
				if (res == FR_OK)
				{
					char *tabUart2;	  //tavleau des elements de la file uart2
//					int i=0;
					tabUart2 = pullN(fileUart2, nbCaractUart2);

//					printf("\n\r -------\n\r");
//					for(i=0;i<nbCaractUart2;i++)
//						printf("%c", tabUart2[i]);
					
					res = f_write (&fil2, tabUart2, nbCaractUart2, &BytesWritten);	//Ecriture du stack dans le fichier 2				
				 	USART_puts(USART2,"File UART2.TXT written ok\r\n");
					free(tabUart2);

				}
				else
				{
				  	USART_puts(USART2,"File UART2.TXT problème wr\r\n");
				}
				res = f_close(&fil2); 	//ferme le fichier 

				//============= Enregistre fichier 3 avec le contenu de la file ==============================
				
		  	res = f_open(&fil3, file3_path, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);	//ouvre /crée fichier3 txt à la racine
				res = f_stat(file3_path, &finf3); 	//get file's info
				res = f_lseek(&fil3, finf3.fsize);		//Aller à la fin du fichier
				if (res == FR_OK)
				{
					char *tabUart3;	  //tavleau des elements de la file uart3
					tabUart3 = pullN(fileUart3, nbCaractUart3);
	 				res = f_write (&fil3, tabUart3, nbCaractUart3, &BytesWritten);	//Ecriture du stack dans le fichier 3				
				  ///	USART_puts(USART2,"File UART3.TXT written ok\r\n");
					free(tabUart3);
				}
				else
				{
				 // 	USART_puts(USART2,"File UART3.TXT problème wr\r\n");
				}
				res = f_close(&fil3); 	//ferme le fichier 
	
				GPIO_ResetBits(GPIOD, GPIO_Pin_15);		//blue led off

				nbCaractUart2 = 0;
				nbCaractUart3 = 0;
				etatBtBleu=2;
	
			break;

			case RUN:
				//=========== Ecriture fichier Uart2 carte SD ===============================================
				if(flag_fileUart2Ready)
				{
					flag_fileUart2Ready=0;
					

					GPIO_SetBits(GPIOD, GPIO_Pin_15);		//blue led on
					//============= Enregistre fichier 2 avec le contenu de la file =========================
		  			res = f_open(&fil2, file2_path, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);	//ouvre /crée fichier2 txt à la racine
					res = f_stat(file2_path, &finf2); 		//get file's info
					res = f_lseek(&fil2, finf2.fsize);		//Aller à la fin du fichier
					if (res == FR_OK)
					{
						char *tabUart2;	  //tavleau des elements de la file uart2
						tabUart2 = pullN(fileUart2, NB_CARACT_STOCK);
						res = f_write (&fil2, tabUart2, NB_CARACT_STOCK, &BytesWritten);	//Ecriture du stack dans le fichier 2				
					  	USART_puts(USART2,"File UART2.TXT written ok\r\n");
						free(tabUart2);
					}
					else
					  	USART_puts(USART2,"File UART2.TXT probleme wr\r\n");

					res = f_close(&fil2); 	//ferme le fichier 
		  	
					GPIO_ResetBits(GPIOD, GPIO_Pin_15);		//blue led off
				}
				
				//=========== Ecriture fichier Uart3 carte SD ===============================================
				if(flag_fileUart3Ready)
				{
					flag_fileUart3Ready = 0;
					GPIO_SetBits(GPIOD, GPIO_Pin_15);		//blue led on
					//============= Enregistre fichier 3 avec le contenu de la file ==========================
		  			res = f_open(&fil3, file3_path, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);	//ouvre /crée fichier3 txt à la racine
					res = f_stat(file3_path, &finf3); 	//get file's info
					res = f_lseek(&fil3, finf3.fsize);		//Aller à la fin du fichier
					if (res == FR_OK)
					{
						char *tabUart3;	  //tavleau des elements de la file uart3
						tabUart3 = pullN(fileUart3, NB_CARACT_STOCK);
	 					res = f_write (&fil3, tabUart3, NB_CARACT_STOCK, &BytesWritten);	//Ecriture du stack dans le fichier 3				
					  	USART_puts(USART2,"File UART3.TXT written ok\r\n");
						free(tabUart3);
					}
					else
					  	USART_puts(USART2,"File UART3.TXT probleme wr\r\n");
					res = f_close(&fil3); 	//ferme le fichier
					GPIO_ResetBits(GPIOD, GPIO_Pin_15);		//blue led off 

				}
			break;
		}	// */



		
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
