/* === variables globales et données liée au interruptions === */

#include "itData.h"
#include <stdio.h>

unsigned char etatBtBleu = 2;				//initialise a pour commencer par START quand on appuie sur le btt
unsigned char led_gr_blink = 0;
unsigned char debounce_btn_en = 0;
struct Fifo *fileUart2;
struct Fifo *fileUart3;

unsigned char flag_fileUart2Ready = 0;
unsigned char flag_fileUart3Ready = 0;

unsigned int nbCaractUart2 = 0;
unsigned int nbCaractUart3 = 0;
