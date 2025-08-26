/*
 * File_Handling_RTOS.c
 *
 *  Created on: 14-May-2020
 *      Author: Controllerstech
 */

#include "File_Handling.h"
#include "stm32f4xx_hal.h"
#include "ili9341.h"
#include "limits.h"

//extern UART_HandleTypeDef huart1;
//#define UART &huart1

extern SD_HandleTypeDef hsd;
volatile bool CardDetectedInSlot = false;

/* =============================>>>>>>>> NO CHANGES AFTER THIS LINE =====================================>>>>>>> */

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;


void InitializeSDCard()
{
	// 1. Wyrejestruj system plików, jeśli jest zamontowany
	if (f_mount(NULL, "", 0) != FR_OK)
	{
		Debug("Nie udalo sie odmontowac systemu plikow.");
	}
	HAL_SD_DeInit(&hsd);
	__HAL_RCC_SDIO_FORCE_RESET();
	HAL_Delay(1000);
	__HAL_RCC_SDIO_RELEASE_RESET();


	// skopiuj inicjalizację z funkji static void MX_SDIO_SD_Init(void)

	hsd.Instance = SDIO;
	hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
	hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	//hsd.Init.BusWide = SDIO_BUS_WIDE_4B;
	hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;

	// bardzo istotny parametr nie moze przekraczac 12 MHZ Aktualnie jest ustawiona na 24 wiec potrzeba podzielic przez 2
	hsd.Init.ClockDiv = 2;

	hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
	if(HAL_SD_Init(&hsd)!= HAL_OK)
	{
		//Error_Handler();
	}

	if(HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B)!= HAL_OK)
	{
		//Error_Handler();
	}
}

FRESULT Mount_SD (const TCHAR* path)
{
	fresult = f_mount(&fs, path, 1);
	if (fresult != FR_OK)
	{
		Debug ("ERROR!!! in mounting SD CARD...");
	}
	else
	{
		//Send_ToLCDDisplay("SD CARD mounted successfully...\n");
	}
	return fresult;
}

FRESULT Unmount_SD (const TCHAR* path)
{
	fresult = f_mount(NULL, path, 0);
	if (fresult != FR_OK) Debug("ERROR!!! in UNMOUNTING SD CARD");//Send_ToLCDDisplay ("SD CARD UNMOUNTED successfully...");
//	else Send_ToLCDDisplay("ERROR!!! in UNMOUNTING SD CARD");

	return fresult;
}

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_SD (char* pat)
{
    DIR dir;
    UINT i;
    char *path = malloc(20*sizeof (char));
    sprintf (path, "%s",pat);

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (fresult != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", fno.fname))) continue;

            	char *buf;
            	size_t bufSize;

            	// Oblicz wymagany rozmiar bufora dla "Dir: %s\r\n"
            	bufSize = snprintf(NULL, 0, "Dir: %s\r\n", fno.fname) + 1;
            	buf = malloc(bufSize);

            	if (buf != NULL)
            	{
            	    snprintf(buf, bufSize, "Dir: %s\r\n", fno.fname);
            	    Send_ToLCDDisplay(buf);
            	    free(buf);
            	}

            	// Upewnij się, że dodawany ciąg mieści się w `path`
            	i = strlen(path);
            	snprintf(&path[i], PATH_MAX - i, "/%s", fno.fname);
                fresult = Scan_SD(path);                     /* Enter the directory */
                if (fresult != FR_OK) break;
                path[i] = 0;
            }
            else
            {
            	char *buf;
            	size_t bufSize;

            	/* It is a file. */
           	   //char *buf = malloc(30*sizeof(char));
            	bufSize = snprintf(NULL, 0, "File: %s/%s\n", path, fno.fname) + 1;
				buf = malloc(bufSize);
            	if (buf != NULL)
            	{
            	    snprintf(buf, bufSize, "File: %s/%s\n", path, fno.fname);
            	    Send_ToLCDDisplay(buf);
            	    free(buf);
            	}

               Send_ToLCDDisplay(buf);
               free(buf);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}

/* Only supports removing files from home directory */
FRESULT Format_SD (void)
{
    DIR dir;
    char *path = malloc(20*sizeof (char));
    sprintf (path, "%s","/");

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (fresult != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", fno.fname))) continue;
            	fresult = f_unlink(fno.fname);
            	if (fresult == FR_DENIED) continue;
            }
            else
            {   /* It is a file. */
               fresult = f_unlink(fno.fname);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}


FRESULT Write_File (char *name, char *data)
{

	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_ToLCDDisplay (buf);
	    free(buf);
	    return fresult;
	}

	else
	{
	    /* Create a file with read write access and open it */
	    fresult = f_open(&fil, name, FA_OPEN_EXISTING | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
	    	Send_ToLCDDisplay(buf);
	        free(buf);
	        return fresult;
	    }

	    else
	    {
	    	fresult = f_write(&fil, data, strlen(data), &bw);
	    	if (fresult != FR_OK)
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "ERROR!!! No. %d while writing to the FILE *%s*\n\n", fresult, name);
	    		Send_ToLCDDisplay(buf);
	    		free(buf);
	    	}

	    	/* Close file */
	    	fresult = f_close(&fil);
	    	if (fresult != FR_OK)
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "ERROR!!! No. %d in closing file *%s* after writing it\n\n", fresult, name);
	    		Send_ToLCDDisplay(buf);
	    		free(buf);
	    	}
	    	else
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "File *%s* is WRITTEN and CLOSED successfully\n", name);
	    		Send_ToLCDDisplay(buf);
	    		free(buf);
	    	}
	    }
	    return fresult;
	}
}

FRESULT Read_File(const char *name, char **data)
{
    fresult = f_stat(name, &fno);
    if (fresult != FR_OK)
    {
        char buf[100];
        sprintf(buf, "ERROR!!! *%s* does not exist\n\n", name);
        Debug(buf);
        return fresult;
    }

    // Otwórz plik do odczytu
    fresult = f_open(&fil, name, FA_READ);
    if (fresult != FR_OK)
    {
        char buf[100];
        sprintf(buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
        Debug(buf);
        return fresult;
    }

    // Alokacja pamięci dla bufora - uwzględniając rozmiar pliku
    DWORD fileSize = f_size(&fil);  // Rozmiar pliku
    *data = malloc(fileSize + 1); // Alokuj pamięć dla danych (dodaj +1 dla '\0')
    if (*data == NULL)  // Sprawdzenie alokacji
    {
    	Debug("ERROR: Memory allocation failed!\n");
        f_close(&fil);  // Zamknięcie pliku
        return FR_INT_ERR;
    }

    // Zresetowanie pamięci przed odczytem
    memset(*data, 0, fileSize + 1);  // Wypełnij bufor zerami

    // Odczyt danych z pliku
    fresult = f_read(&fil, *data, fileSize, &br);
    if (fresult != FR_OK || br != fileSize) {
        char buf[100];
        sprintf(buf, "ERROR!!! No. %d in reading file *%s*\n\n", fresult, name);
        Debug(buf);
        free(*data);  // Zwolnij pamięć w przypadku błędu
        f_close(&fil);
        return fresult;
    }

    // Dodanie null-terminatora na końcu zawartości pliku
    (*data)[fileSize] = '\0';

    // Zamknięcie pliku
    fresult = f_close(&fil);
    if (fresult != FR_OK) {
        char buf[100];
        sprintf(buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
        Debug(buf);
    }

    return fresult;
}


//
//    // Czytaj dane z pliku
//    fresult = f_read(&fil, *data, fileSize, &br);
//    if (fresult != FR_OK)
//    {
//        char buf[100];
//        sprintf(buf, "ERROR!!! No. %d in reading file *%s*\n\n", fresult, name);
//        Send_ToLCDDisplay(buf);
//    }
//    else
//    {
//        // Wyślij zawartość pliku przez LCD
//        //Send_ToLCDDisplay(*data);
//    }
//
//    // Zwolnienie pamięci
//    free(*data);
//
//    // Zamknięcie pliku
//    fresult = f_close(&fil);
//    if (fresult != FR_OK)
//    {
//        char buf[100];
//        sprintf(buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
//        Send_ToLCDDisplay(buf);
//    }
////    else
////    {
////        char buf[100];
////        sprintf(buf, "File *%s* CLOSED successfully\n", name);
////        Send_ToLCDDisplay(buf);
////    }
//
//    return fresult;
//}

FRESULT Read_And_Display_JPEG(const char *filename, uint16_t startDrawX, uint16_t startDrawY, uint16_t ImageWidth, uint16_t ImageHeight, uint16_t chunkRows)
{
    FIL fil;  // Obiekt pliku
    FRESULT result;
    UINT bytesRead;
    DWORD fileSize, offset = 0;
    uint16_t *imageData;
    uint16_t chunkSize = ImageWidth * 2 * chunkRows; // Każdy piksel RGB565 ma 2 bajty, wczytujemy cały wiersz
    uint16_t currentX = startDrawX;
    uint16_t currentY = startDrawY;

    // Otwórz plik do odczytu
    result = f_open(&fil, filename, FA_READ);
    if (result != FR_OK) {
    	Debug("ERROR: Unable to open file\n");
        HAL_Delay(5000);
        return result;
    }

    // Pobierz rozmiar pliku
    fileSize = f_size(&fil);
//    Send_ToLCDDisplay("Starting to display image...\n");

    // Dostosowanie fragmentów obrazu
    while (offset < fileSize)
    {
        // Alokacja bufora na fragment obrazu (szerokość ekranu)
        imageData = malloc(chunkSize);
        if (imageData == NULL)
        {
        	Debug("ERROR: Memory allocation failed");
            HAL_Delay(5000);
            f_close(&fil);
            return FR_INT_ERR;
        }

        // Ustaw wskaźnik pliku na odpowiednią pozycję (offset)
        result = f_lseek(&fil, offset);
        if (result != FR_OK)
        {
            Debug("ERROR: f_lseek failed");
            char debugMessage[50];
            snprintf(debugMessage, sizeof(debugMessage), "f_lseek result: %d", result);
            Debug(debugMessage);
            free(imageData);
            f_close(&fil);
            return result;
        }


        // Odczytaj fragment obrazu do bufora
        result = f_read(&fil, imageData, chunkSize, &bytesRead);
        if (result != FR_OK || bytesRead != chunkSize)
        {
            free(imageData);
            Debug("ERROR: Reading image chunk failed");
            HAL_Delay(5000);
            f_close(&fil);
            return FR_INT_ERR;
        }

        // Wyświetl wczytany fragment obrazu na ekranie
        uint16_t x = currentX;  // Zawsze zaczynaj od X
        uint16_t y = currentY;  // Zaczynamy od Y
        lcdDrawPixels(x, y, imageData, bytesRead / 2); // Każdy piksel RGB565 ma 2 bajty

        // Zaktualizuj offset, aby przejść do następnego fragmentu
        offset += bytesRead;

        // Przejście do nowej linii, jeśli osiągnięto szerokość ekranu
        currentY += chunkRows;
        if (currentY >= startDrawY + ImageHeight) {
            currentY = startDrawY;  // Zresetuj Y do początkowej pozycji
            currentX += ImageWidth;  // Przejdź do kolejnej kolumny
        }

        // Zwolnij pamięć
        free(imageData);
    }

    // Zamknięcie pliku
    result = f_close(&fil);
    if (result != FR_OK) {
    	Debug("ERROR: Could not close file");
        HAL_Delay(5000);
        return result;
    }

    //Send_ToLCDDisplay("Image displayed successfully\n");
    return FR_OK;
}



FRESULT Create_File (char *name)
{
	fresult = f_stat (name, &fno);
	if (fresult == FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* already exists!!!!\n use Update_File \n\n",name);
		Debug(buf);
		free(buf);
	    return fresult;
	}
	else
	{
		fresult = f_open(&fil, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in creating file *%s*\n\n", fresult, name);
			Debug(buf);
			free(buf);
		    return fresult;
		}
		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "*%s* created successfully\n Now use Write_File to write data\n",name);
			Send_ToLCDDisplay(buf);
			free(buf);
		}

		fresult = f_close(&fil);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in closing file *%s*\n\n", fresult, name);
			Debug(buf);
			free(buf);
		}
		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "File *%s* CLOSED successfully\n", name);
			Send_ToLCDDisplay(buf);
			free(buf);
		}
	}
    return fresult;
}

FRESULT Update_File (char *name, char *data)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		Debug (buf);
		free(buf);
	    return fresult;
	}

	else
	{
		 /* Create a file with read write access and open it */
	    fresult = f_open(&fil, name, FA_OPEN_APPEND | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
	    	Debug(buf);
	        free(buf);
	        return fresult;
	    }

	    /* Writing text */
	    fresult = f_write(&fil, data, strlen (data), &bw);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in writing file *%s*\n\n", fresult, name);
	    	Debug(buf);
	    	free(buf);
	    }

	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "*%s* UPDATED successfully\n", name);
	    	Send_ToLCDDisplay(buf);
	    	free(buf);
	    }

	    /* Close file */
	    fresult = f_close(&fil);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
	    	Debug(buf);
	    	free(buf);
	    }
	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "File *%s* CLOSED successfully\n", name);
	    	Send_ToLCDDisplay(buf);
	    	free(buf);
	     }
	}
    return fresult;
}

FRESULT Remove_File (char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		Debug (buf);
		free(buf);
		return fresult;
	}

	else
	{
		fresult = f_unlink (name);
		if (fresult == FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "*%s* has been removed successfully\n", name);
			Send_ToLCDDisplay (buf);
			free(buf);
		}

		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in removing *%s*\n\n",fresult, name);
			Debug (buf);
			free(buf);
		}
	}
	return fresult;
}

FRESULT Create_Dir (char *name)
{
    fresult = f_mkdir(name);
    if (fresult == FR_OK)
    {
    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "*%s* has been created successfully\n", name);
    	Send_ToLCDDisplay (buf);
    	free(buf);
    }
    else
    {
    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "ERROR No. %d in creating directory *%s*\n\n", fresult,name);
    	Debug(buf);
    	free(buf);
    }
    return fresult;
}

void Check_SD_Space (void)
{
    /* Check free space */
    f_getfree("", &fre_clust, &pfs);

    total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
    char *buf = malloc(30*sizeof(char));
    sprintf (buf, "SD CARD Total Size: \t%lu\n",total);
    Send_ToLCDDisplay(buf);
    free(buf);
    free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
    buf = malloc(30*sizeof(char));
    sprintf (buf, "SD CARD Free Space: \t%lu\n",free_space);
    Send_ToLCDDisplay(buf);
    free(buf);
}


