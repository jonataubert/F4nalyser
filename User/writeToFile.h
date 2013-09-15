/* === SD card write file functions header === */

#ifndef DEF_SDWRITE
#define DEF_SDWRITE

/*******************************************************************************
* Function Name  : InitSDCard
* Description    : mount the sd card and set the use of fat32 format
* Input          : none
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void Init_SDCard(void);

/*******************************************************************************
* Function Name  : writeHeader
* Description    : write a header string into a file
* Input          : header string, the path to the file
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void writeHeader(const char header[], const char pathToFile[]);

/*******************************************************************************
* Function Name  : writeDynamicTabDate
* Description    : write an dynamically allocated tab into a file
* Input          : tab, the number of elements of the tab, the path to the file
* Output         : None
* Return         : write error (1=error,0=no error)
* Attention      : None
*******************************************************************************/
unsigned char writeDynamicTabData(char* dynamicTab, int nbOfElements, const char pathToFile[]);

#endif
