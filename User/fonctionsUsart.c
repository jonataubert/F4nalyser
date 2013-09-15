/* === Fonctions utilisant l'USART === */

#include "stm32f4_discovery.h"
#include "fonctionsUsart.h"

// Variables Globales internes
char inibUsart = 0;


void USART_puts(USART_TypeDef* USARTx, char *s){

	while(*s){
		// wait until data register is empty
		while( !(USARTx->SR & 0x00000040) );
		USART_SendData(USARTx, *s);
		*s++;
	}
}

void enableUsartIt(void)
{
	inibUsart = 1;
}

void disableUsartIt(void)
{
	inibUsart = 0;
}

char isUsartOk(void)
{
	return inibUsart;
}
