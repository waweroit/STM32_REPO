/*
 * File_Handling_RTOS.h
 *
 *  Created on: 14-May-2020
 *      Author: Controllerstech
 */
#pragma once
#ifndef FILE_HANDLING_RTOS_H_
#define FILE_HANDLING_RTOS_H_

#include "fatfs.h"
#include "string.h"
#include "stdio.h"
#include "fatfs.h"
#include "ffconf.h"
#include "ff.h"
#include "stdbool.h"
//#define fs SDFatFS

extern volatile bool CardDetectedInSlot;

//void Send_ToLCDDisplay (char *string);

// wymagane do static void MX_SDIO_SD_Init(void) w USER
void InitializeSDCard();

/* mounts the sd card*/
FRESULT Mount_SD (const TCHAR* path);

/* unmounts the sd card*/
FRESULT Unmount_SD (const TCHAR* path);

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_SD (char* pat);

/* Only supports removing files from home directory. Directory remover to be added soon */
FRESULT Format_SD (void);

/* write the data to the file
 * @ name : is the path to the file*/
FRESULT Write_File (char *name, char *data);

/* read data from the file
 * @ name : is the path to the file*/
FRESULT Read_File(const char *name, char **data);

FRESULT Read_And_Display_JPEG(const char *filename, uint16_t startDrawX, uint16_t startDrawY, uint16_t ImageWidth, uint16_t ImageHeight, uint16_t chunkRows);

/* creates the file, if it does not exists
 * @ name : is the path to the file*/
FRESULT Create_File (char *name);

/* Removes the file from the sd card
 * @ name : is the path to the file*/
FRESULT Remove_File (char *name);

/* creates a directory
 * @ name: is the path to the directory
 */
FRESULT Create_Dir (char *name);

/* checks the free space in the sd card*/
void Check_SD_Space (void);

/* updates the file. write pointer is set to the end of the file
 * @ name : is the path to the file
 */
FRESULT Update_File (char *name, char *data);


#endif /* FILE_HANDLING_RTOS_H_ */
