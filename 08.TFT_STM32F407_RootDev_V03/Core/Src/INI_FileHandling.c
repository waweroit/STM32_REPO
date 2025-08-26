/*
 * INI_FileHandling.c
 *
 *  Created on: Jan 13, 2025
 *      Author: waweroIT
 */

#include "INI_FileHandling.h"
#include <ctype.h>

FRESULT Load_INI_File(const char *filename, IniFile *ini)
{
    char *fileData = NULL;
    FRESULT res = Read_File(filename, &fileData);
    if (res != FR_OK) {
        return res;
    }

    char *line = strtok(fileData, "\n");
    char currentSection[32] = "";
    ini->entryCount = 0;

    while (line) {
        // Usuń znak \r z końca linii
        char *carriageReturn = strchr(line, '\r');
        if (carriageReturn) {
            *carriageReturn = '\0';
        }

        // Jeśli sekcja
        if (line[0] == '[') {
            sscanf(line, "[%31[^]]]", currentSection);
        } else if (strchr(line, '=')) {
            // Jeśli klucz=wartość
            char key[32], value[64];
            sscanf(line, "%31[^=]=%63[^\n]", key, value);

            // Usuń białe znaki z końca klucza i wartości
//            TrimWhitespace(key);
//            TrimWhitespace(value);

            IniEntry *entry = &ini->entries[ini->entryCount++];
            strncpy(entry->section, currentSection, sizeof(entry->section) - 1);
            strncpy(entry->key, key, sizeof(entry->key) - 1);
            strncpy(entry->value, value, sizeof(entry->value) - 1);
        }

        line = strtok(NULL, "\n");
    }

    free(fileData);
    return FR_OK;
}

//bool Get_INI_Value(const IniFile *ini, const char *section, const char *key, char *out_value, size_t max_len)
//{
//    for (int i = 0; i < ini->entryCount; i++) {
//        if (strcmp(ini->entries[i].section, section) == 0 &&
//            strcmp(ini->entries[i].key, key) == 0) {
//            strncpy(out_value, ini->entries[i].value, max_len);
//            return true;
//        }
//    }
//    return false;
//}

bool Get_INI_Value(const IniFile *ini, const char *section, const char *key, char *out_value, size_t max_len) {
    for (int i = 0; i < ini->entryCount; i++) {
        if (strcmp(ini->entries[i].section, section) == 0 &&
            strcmp(ini->entries[i].key, key) == 0) {
            strncpy(out_value, ini->entries[i].value, max_len - 1);
            out_value[max_len - 1] = '\0'; // Ręczne zakończenie ciągu
            return true;
        }
    }
    return false;
}

void Set_INI_Value(IniFile *ini, const char *section, const char *key, const char *value)
{
    for (int i = 0; i < ini->entryCount; i++) {
        if (strcmp(ini->entries[i].section, section) == 0 &&
            strcmp(ini->entries[i].key, key) == 0) {
            strncpy(ini->entries[i].value, value, sizeof(ini->entries[i].value));
            return;
        }
    }
    // Dodaj nowy wpis, jeśli nie istnieje
    IniEntry *entry = &ini->entries[ini->entryCount++];
    strncpy(entry->section, section, sizeof(entry->section));
    strncpy(entry->key, key, sizeof(entry->key));
    strncpy(entry->value, value, sizeof(entry->value));
}

bool Get_INI_Value_As_Int(const IniFile *ini, const char *section, const char *key, int *out_value) {
    char value_str[64]; // Tymczasowe przechowywanie wartości jako string

    // Pobierz wartość jako string
    if (Get_INI_Value(ini, section, key, value_str, sizeof(value_str)))
    {
//        snprintf(debugData, sizeof(debugData), "Klucz: [%s], Wartosc: [%s]", key, value_str);
//        Debug(debugData);
//        HAL_Delay(5000);

        // Przekonwertuj na int
        if (Convert_INI_Value_To_Int(value_str, out_value))
        {
//            snprintf(debugData, sizeof(debugData), "Przekonwertowana wartosc: %d", *out_value);
//            Debug(debugData);
//            HAL_Delay(5000);
            return true;
        }
        else
        {
            Debug("Bad konwersji wartosci na int.");
        }
    } else
    {
        Debug("Nie znaleziono klucza w sekcji.");

    }

    return false; // Klucz lub konwersja nieprawidłowa
}

void TrimWhitespace(char *str) {
    char *end;

    // Usuń wiodące spacje
    while (isspace((unsigned char)*str)) str++;

    // Jeśli ciąg jest pusty
    if (*str == 0) return;

    // Usuń końcowe spacje
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Zakończ ciąg znakowy
    *(end + 1) = '\0';
}

void TrimString(char *str) {
    char *end;

    // Usuń wiodące spacje
    while (isspace((unsigned char)*str)) str++;

    // Usuń końcowe spacje
    if (*str == 0) return;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
}

/**
 * @brief Konwertuje wartość z INI na typ int
 * @param value Wskaźnik na ciąg znaków zawierający wartość
 * @param out_result Wskaźnik na zmienną, do której zostanie zapisany wynik
 * @return true, jeśli konwersja się powiedzie, false w przypadku błędu
 */
bool Convert_INI_Value_To_Int(const char *value, int *out_result) {
    if (!value || !out_result) {
        Debug("Blad: Wskaznik `value` lub `out_result` jest NULL.");
        return false;
    }

    // Tworzenie kopii ciągu
    char temp_value[64];
    strncpy(temp_value, value, sizeof(temp_value) - 1);
    temp_value[sizeof(temp_value) - 1] = '\0';

    // Usunięcie białych znaków
    TrimString(temp_value);

    // Debugowanie
//	Debug("                                            ");
//    snprintf(debugData, sizeof(debugData), "Wartosc po przycieciu: [%s]", temp_value);
//    Debug(debugData);
//    HAL_Delay(5000);

    // Konwersja na liczbę
    char *end_ptr;
    long result = strtol(temp_value, &end_ptr, 10);

    // Sprawdzenie błędów konwersji
    if (*end_ptr != '\0' || end_ptr == temp_value)
    {
        snprintf(debugData, sizeof(debugData), "Nieprawidlowy ciag znakow: %s", temp_value);
        Debug(debugData);
        return false;
    }

    // Sprawdzenie zakresu dla int
    if (result < INT_MIN || result > INT_MAX)
    {
        Debug("Wynik poza zakresem dla int.");
        return false;
    }

    *out_result = (int)result;
    return true;
}

