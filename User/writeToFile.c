/* === SD card write to file functions depending on ff.h and diskio.h === */

#include "ff.h"
#include "diskio.h"

#define DBG //Debug/developping mode


void writeHeader(char header[], char pathToFile[])
{
    FRESULT res;        // File function return code (si exception ou ok)   
    FILINFO fInfo;      // File informations
    FIL myFile;         // File object structure
//  DIR dir;            // Directory object structure
    FATFS fs32;         // File system object structure
    UINT BytesWritten;  // Informations about the number of byte written into the file


    memset(&myFile, 0, sizeof(FIL));                                            //Init myFile
    memset(&fInfo, 0, sizeof(FILINFO));                                         //Init fInfo

    res = f_open(&myFile, pathToFile, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);     //open/create the file at pathToFile

    #ifdef DBG
        if (res != FR_OK)
            printf("res = %d f_open %s\n", res, pathToFile);
        else
            USART_puts(USART2,"File %s created/opened\r\n", pathToFile);
    #endif
    
    res = f_stat(pathToFile, &fInfo);                                           //get file's info

    #ifdef DBG
        printf("end of file %d \n", myFile.fsize);
    #endif

    res = f_lseek(&myFile, fInfo.fsize);                                        //go to the end of the file
    if (res == FR_OK)
        res = f_write (&myFile, header, strlen(header), &BytesWritten);         //write the header

    #ifdef DBG
        if (res != FR_OK)
            printf("res = %d File write problem %s\n", res, pathToFile);
        else
            USART_puts(USART2,"File %s written\r\n", pathToFile);
    #endif
    
    res = f_close(&myFile);                                                     //close the file
}

unsigned char writeDynamicTabData(char* dynamicTab, int nbOfElements, char pathToFile[])
{
    FRESULT res;        // File function return code (si exception ou ok)   
    FILINFO fInfo;      // File informations
    FIL myFile;         // File object structure
//  DIR dir;            // Directory object structure
    FATFS fs32;         // File system object structure
    UINT BytesWritten;  // Informations about the number of byte written into the file

    unsigned char error = 0;

    memset(&myFile, 0, sizeof(FIL));                                            //Init myFile
    memset(&fInfo, 0, sizeof(FILINFO));                                         //Init fInfo

    res = f_open(&myFile, pathToFile, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);     //open/create the file at pathToFile
    res = f_stat(pathToFile, &fInfo);                                           //get file's info
    res = f_lseek(&myFile, fInfo.fsize);                                        //go to the end of the file

    if (res == FR_OK)
    {
        res = f_write (&myFile, dynamicTab, nbOfElements, &BytesWritten);       //write the dynamic tab

        error=0;
        #ifdef DBG              
            USART_puts(USART2,"File %s written ok\r\n", pathToFile);
        #endif
    }
    else
    {
        error=1;
        #ifdef DBG
            USART_puts(USART2,"File %s write problem\r\n", pathToFile);
        #endif
    }

    free(dynamicTab);
    res = f_close(&myFile);                                                     //close the file 

    return error;
}

