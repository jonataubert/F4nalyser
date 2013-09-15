/* === Header fonctions USART === */
#ifndef DEF_FONCTIONSUSART
#define DEF_FONCTIONSUSART

/* define*/
#define MAX_STRLEN 1000 // this is the maximum string length of our string in characters

// Variables Globales
extern char received_string_usart2[MAX_STRLEN+1]; // this will hold the recieved string
extern char received_string_usart3[MAX_STRLEN+1]; // this will hold the recieved string
//extern unsigned char flag_rx2_full;	//when string_usart2 is full = 1
//extern unsigned char flag_rx3_full;	//when string_usart3 is full = 1
/* This function is used to transmit a string of characters via
 * the USART specified in USARTx.
 *
 * It takes two arguments: USARTx --> can be any of the USARTs e.g. USART1, USART2 etc.
 * 						   (volatile) char *s is the string you want to send
 *
 * Note: The string has to be passed to the function as a pointer because
 * 		 the compiler doesn't know the 'string' data type. In standard
 * 		 C a string is just an array of characters
 *
 * Note 2: At the moment it takes a volatile char because the received_string variable
 * 		   declared as volatile char --> otherwise the compiler will spit out warnings
 * */
void USART_puts(USART_TypeDef* USARTx, char *s);

void enableUsartIt(void);
void disableUsartIt(void);
char isUsartOk(void);

#endif
