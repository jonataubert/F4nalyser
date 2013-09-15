/* === variables gobales générales === */
#ifndef	DEF_ITDATA
#define	DEF_ITDATA


//Nombre de caracteres à stocker avant de les ecrire sur la carte SD
#define NB_CARACT_STOCK	1000

//Etats bouton bleu
#define	STOP        0
#define	START       1
#define WAIT        2
#define	RUN         3

extern unsigned char etatBtBleu;
extern unsigned char led_gr_blink;
extern unsigned char debounce_btn_en;

extern struct Fifo *fileUart2;
extern struct Fifo *fileUart3;

extern unsigned char flag_fileUart2Ready;
extern unsigned char flag_fileUart3Ready;

extern unsigned int nbCaractUart2;
extern unsigned int nbCaractUart3;

#endif
