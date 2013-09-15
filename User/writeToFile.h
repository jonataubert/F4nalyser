/* === SD card write file functions header === */


/*******************************************************************************
* Function Name  : writeHeader
* Description    : write a header string into a file
* Input          : header string, the path to the file
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void writeHeader(char header[], char pathToFile[]);

/*******************************************************************************
* Function Name  : writeDynamicTabDate
* Description    : write an dynamically allocated tab into a file
* Input          : tab, the number of elements of the tab, the path to the file
* Output         : None
* Return         : write error (1=error,0=no error)
* Attention      : None
*******************************************************************************/
unsigned char writeDynamicTabData(char* dynamicTab, int nbOfElements, char pathToFile[]);
