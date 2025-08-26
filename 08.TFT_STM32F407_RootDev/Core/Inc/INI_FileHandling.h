/*
 * INI_FileHandling.h
 *
 *  Created on: Jan 13, 2025
 *      Author: waweroIT
 */
#pragma once
#ifndef INC_INI_FILEHANDLING_H_
#define INC_INI_FILEHANDLING_H_

#include "File_Handling.h"
#include "stm32f4xx_hal.h"
#include "ili9341.h"
#include "limits.h"

typedef struct {
    char section[32];
    char key[32];
    char value[64];
} IniEntry;

typedef struct {
    IniEntry entries[50]; // Maksymalnie 100 wpisów
    int entryCount;
} IniFile;

FRESULT Load_INI_File(const char *filename, IniFile *ini);
bool Get_INI_Value(const IniFile *ini, const char *section, const char *key, char *out_value, size_t max_len);
bool Get_INI_Value_As_Int(const IniFile *ini, const char *section, const char *key, int *out_value);
bool Convert_INI_Value_To_Int(const char *value, int *out_result);
void Set_INI_Value(IniFile *ini, const char *section, const char *key, const char *value);

#endif /* INC_INI_FILEHANDLING_H_ */
