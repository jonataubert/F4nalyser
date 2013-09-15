/* === Header fonctions d'initialisations === */
#ifndef DEF_FONCTIONINIT
#define DEF_FONCTIONINIT



/* -- Prototype de fonctions -- */ 

//============Configuration du timer2===================
//Attention, un seul timer peut déclencher plusieurs 
//interruptions temporelles (4 CCR pour timer 2)
//attention le timer 2 est en mode 32 bit!
//	 AHB Prescaler = 1
//   APB1 Prescaler  = 4 donc le clk arrive à 42Mhz
//		donc le clk arrive à (168/4)*2 = 84Mhz
//		*2 car voir datasheet diagram clock
//======================================================
void Timer2_Config(void);

/*******************************************************************************
* Function Name  : Init_Uart2
* Description    : Configure USART2 , 8n1	TX:PA2 RX:PA3
* Input          : baudrate value
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Init_Uart2(unsigned long baudrate);

/*******************************************************************************
* Function Name  : Init_Uart3
* Description    : Configure USART3 , 8n1	RX:PD9
* Input          : baudrate value
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Init_Uart3(unsigned long baudrate);

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure ports (general)
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void GPIO_Configuration(void);


/*******************************************************************************
* Function Name  : EXTILine0_Config
* Description    : Configure interrupt Push-Button
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void EXTILine0_Config(void);



#endif
