/* ######################################## *
	    Fifo.c
 * ######################################## */

#include <stdlib.h>
#include <stdio.h>
#include "fifo.h"
#include "stm32f4_discovery.h"




Fifo* initialiserFile()
{
    Fifo *file = malloc(sizeof(*file));
    file->premier = NULL;

    return file;
}

void push(Fifo *file, char nvNombre)
{
	Element *nouveau = malloc(sizeof(*nouveau));
	Element *elementActuel = NULL;

    if (file == NULL || nouveau == NULL)							//NETTOYER CETTE MERDE
    {
        printf("CACA !!!");
    }
 
    nouveau->caractere = nvNombre;
    nouveau->suivant = NULL;
 	 
    if (file->premier != NULL) // La file n'est pas vide 
    {
        // On se positionne à la fin de la file 
        elementActuel = file->premier;
        while (elementActuel->suivant != NULL)
        {
//		  		printf("%d, %d \n\r",elementActuel, elementActuel->suivant);
            elementActuel = elementActuel->suivant;		
        }
        elementActuel->suivant = nouveau;		//	*/
		  
    }
    else // La file est vide, notre élément est le premier 
    {
        file->premier = nouveau;
    }			//	*/
//	 printf("\n\r%c \n\n\r------------\n\r", nouveau->caractere);
//	 printf(listFile(file));
//	 printf("\n\r-----------------------\n\n\r");
}

char pull(Fifo *file)
{
    char nombreDefile = '\0';

 	 if (file == NULL)
    {
        printf("CACA !!!");
    }		 
 

// 		while(busy);
//		busy = 1;
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    /* On vérifie s'il y a quelque chose à défiler */
    if (file->premier != NULL)
    {
        Element *elementDefile = file->premier;
 
        nombreDefile = elementDefile->caractere;
        file->premier = elementDefile->suivant;
        free(elementDefile);
    }
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//		busy = 0;
    return nombreDefile;
}

char* listFile(Fifo *file)
{
	char *string = NULL;
	unsigned int nbElements = 1;
	unsigned int i=0;
	Element *elementActuel = file->premier;

	if (file == NULL)
   {
       printf("CACA !!!");
   }	

	if(elementActuel!=NULL)
	{
		//On compte le nombre d'elements
		while (elementActuel->suivant != NULL)
	   {
	      elementActuel = elementActuel->suivant;
			nbElements++;
	   }

		string = malloc((nbElements + 1) * sizeof(char));
		if (string == NULL)
		{
			printf("CACA !!!");
		}	
	
		elementActuel = file->premier;  //on repointe sur le premier element
	
		//on lit tout les elements de la file
		for(i=0;i<nbElements;i++)
		{	
			string[i]= elementActuel->caractere;
			elementActuel = elementActuel->suivant;
		}
		string[i] = '\0';
	}

	return string;
}

char* pullN(Fifo *file, unsigned int nb)
{
	unsigned int i=0;
	char *tab;
	tab = malloc(nb*sizeof(char));

	if ((file == NULL)||(tab==NULL))
   {
      printf("CACA !!!");
   } 

	//on appelle pull n fois
	for(i=0;i<nb;i++)
		tab[i]= pull(file);	


	return tab;
}


