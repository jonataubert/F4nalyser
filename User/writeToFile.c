/* === SD card write to file functions depending on ff.h and diskio.h === */

#include "ff.h"
#include "diskio.h"
#include "stm32f4xx.h"

#include <stdlib.h>
#include <string.h>

#define DBG //Debug/developping mode

#ifdef DBG
		#include <stdio.h>
#endif

void Init_SDCard(void)
{
    FRESULT res;        // File function return code (if everything is ok or error code)  
    FATFS fs32;         // File system object structure

    memset(&fs32,0, sizeof(FATFS));             //initialise l'objet fs32 avec des 0
    res = f_mount(0, &fs32);                    //Monte la carte SD (device 0)

    #ifdef DBG
        if (res != FR_OK)
            printf("res = %d f_mount\n", res);
        else
            printf("SD mounted\r\n");
    #endif
}

void writeHeader(char header[], char pathToFile[])
{
    FRESULT res;        // File function return code (if everything is ok or error code)   
    FILINFO fInfo;      // File informations
    FIL myFile;         // File object structure
//  DIR dir;            // Directory object structure
//  FATFS fs32;         // File system object structure
    UINT BytesWritten;  // Informations about the number of byte written into the file


    memset(&myFile, 0, sizeof(FIL));                                            //Init myFile
    memset(&fInfo, 0, sizeof(FILINFO));                                         //Init fInfo

    res = f_open(&myFile, pathToFile, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);     //open/create the file at pathToFile

    #ifdef DBG
        if (res != FR_OK)
            printf("res = %d f_open %s\n", res, pathToFile);
        else
            printf("File %s created/opened\r\n", pathToFile);
    #endif
    
    res = f_stat(pathToFile, &fInfo);                                           //get file's info

    #ifdef DBG
        printf("end of file %d \n", (unsigned int)myFile.fsize);
    #endif

    res = f_lseek(&myFile, fInfo.fsize);                                        //go to the end of the file
    if (res == FR_OK)
        res = f_write (&myFile, header, strlen(header), &BytesWritten);         //write the header

    #ifdef DBG
        if (res != FR_OK)
            printf("res = %d File write problem %s\n", res, pathToFile);
        else
            printf("File %s written\r\n", pathToFile);
    #endif
    
    res = f_close(&myFile);                                                     //close the file
}

unsigned char writeDynamicTabData(char* dynamicTab, int nbOfElements, char pathToFile[])
{
    FRESULT res;        // File function return code (if everything is ok or error code)   
    FILINFO fInfo;      // File informations
    FIL myFile;         // File object structure
//  DIR dir;            // Directory object structure
//  FATFS fs32;         // File system object structure
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
            printf("File %s written ok\r\n", pathToFile);
        #endif
    }
    else
    {
        error=1;
        #ifdef DBG
            printf("File %s write problem\r\n", pathToFile);
        #endif
    }

    free(dynamicTab);
    res = f_close(&myFile);                                                     //close the file 

    return error;
}

