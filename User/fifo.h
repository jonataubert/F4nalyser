/* ######################################## *
	     Fifo.h
 * ######################################## */
#ifndef	DEF_FIFO
#define	DEF_FIFO

typedef struct Element Element;
struct Element
{
    char caractere;
    Element *suivant;
};
 
typedef struct Fifo Fifo;
struct Fifo
{
    Element *premier;
};

Fifo *initialiserFile(void);

/*******************************************************************************
* Function Name  	: push
* Description    	: push a value to the fifo pile
* Input          	: la file FIFO, le caractere a stocker
* Output         	: rajoute un element a la file
* Return         	: None
* Attention	 		: None
*******************************************************************************/
void push(Fifo *file, char nvNombre);

/*******************************************************************************
* Function Name 	: pull
* Description		: pull a value from the fifo pile
* Input				: la file FIFO
* Output				: enleve un element de la file
* Return				: un caractere de la file
* Attention			: None
*******************************************************************************/
char pull(Fifo *file);

/*******************************************************************************
* Function Name	: listFile
* Description     : list values from the fifo pile
* Input           : la file FIFO
* Output          : None.
* Return          : une chaine de caractere contenant toute la pile
* Attention		   : Ne pas oublier d'utiliser free() !!!!
*******************************************************************************/
char* listFile(Fifo *file);

/*******************************************************************************
* Function Name   : pullN
* Description     : fait n fois la fonction pull
* Input           : la file FIFO et n
* Output          : enleve n elements de la file
* Return          : un tableau de caractere contenant n elements de la pile
* Attention		   : Ne pas oublier d'utiliser free() !!!!
*******************************************************************************/
char* pullN(Fifo *file, unsigned int nb);

#endif
