/*
 * SPITouchScreen.c
 *
 *  Created on: Nov 17, 2024
 *      Author: wawer
 */
#include "SPITouchScreen.h"
#include <stdlib.h>

//extern volatile int spi_dma_done;  // Deklaracja
extern SPI_HandleTypeDef hspi2;  // Deklaracja

volatile uint8_t TSInterrupt = 0;
volatile int IsReceivedNewCoords=0;

volatile uint16_t touch_x = 0;
volatile uint16_t touch_y = 0;


uint16_t xCoordinates[NUM_COORDINATES];  // Tablica na współrzędne X // Deklaracja
uint16_t yCoordinates[NUM_COORDINATES];  // Tablica na współrzędne Y // Deklaracja

int CalibrateX = 0;
int CalibrateY = 0;


static uint8_t releaseCount = 0;

//==========================================================================================================================
void initTouchScr()
{
	CalibrateX = scaleBetween(0, 0, XPT2046_ScrWidth, 0, (TouchPointXStart +TouchPointXEnd )/2);
	CalibrateY = scaleBetween(5, 0, XPT2046_ScrHigh, 0, (TouchPointYStart +TouchPointYEnd )/2);
}

uint16_t ReadTouchCoordinate(uint8_t command)
{
    uint8_t tx_data[3];  // Bufor na komendę i dane
    uint8_t rx_data[3];  // Bufor na odbiór danych
    uint16_t coordinate = 0;

    HAL_GPIO_WritePin(TouchCS_Port, TouchCS_Pin, GPIO_PIN_RESET);  // CS low org
    tx_data[0] = command;  // Wysyłamy komendę do odczytu
    HAL_SPI_TransmitReceive(&hspi2, tx_data, rx_data, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(TouchCS_Port, TouchCS_Pin, GPIO_PIN_SET);  // CS high org

    // Koordynata znajduje się w 2 bajtach (MSB i LSB)
    coordinate = ((rx_data[1] << 8) | rx_data[2]) >> 3;  // Dane 12-bitowe
    return coordinate;
}


// Funkcja do wyboru urządzenia na magistrali SPI (aktywacja CS)
void Touch_Select() {
    HAL_GPIO_WritePin(TouchCS_Port, TouchCS_Pin, GPIO_PIN_RESET);
}

// Funkcja do odznaczenia urządzenia na magistrali SPI (dezaktywacja CS)
void Touch_Unselect() {
    HAL_GPIO_WritePin(TouchCS_Port, TouchCS_Pin, GPIO_PIN_SET);
}

// Funkcja wysyła dane przez SPI i odbiera odpowiedź
uint8_t Touch_SPI_Transfer(uint8_t data) {
    uint8_t receivedData = 0;
    HAL_SPI_TransmitReceive(&hspi2, &data, &receivedData, 1, HAL_MAX_DELAY);
    return receivedData;
}

// Funkcja odczytu koordynatów X i Y
void Touch_ReadCoordinates(uint16_t* x, uint16_t* y)
{
    uint8_t tempXH, tempXL, tempYH, tempYL;

    Touch_Select(); // CS Low
    // Wysyłanie komendy odczytu X
//    Touch_SPI_Transfer(0xD0);  // Komenda dla odczytu X
    Touch_SPI_Transfer(XPT2046_CMD_X);  // Komenda dla odczytu X 0xD0
    tempXH = Touch_SPI_Transfer(0x00);  // Odczyt wyższego bajtu X
    tempXL = Touch_SPI_Transfer(0x00);  // Odczyt niższego bajtu X

    // Wysyłanie komendy odczytu Y
//    Touch_SPI_Transfer(0x90);  // Komenda dla odczytu Y
    Touch_SPI_Transfer(XPT2046_CMD_Y);  // Komenda dla odczytu Y 0x90
    tempYH = Touch_SPI_Transfer(0x00);  // Odczyt wyższego bajtu Y
    tempYL = Touch_SPI_Transfer(0x00);  // Odczyt niższego bajtu Y

    Touch_Unselect(); // CS High

    // Łączenie bajtów w 16-bitowe wartości X i Y
    *x = (tempXH << 8 | tempXL) >> 3;  // Usunięcie nieużywanych bitów
    *y = (tempYH << 8 | tempYL) >> 3;  // Usunięcie nieużywanych bitów

}

// gen by Open AI
void XPT2046_Select() {
    HAL_GPIO_WritePin(TouchCS_Port, TouchCS_Pin, GPIO_PIN_RESET);  // CS Low
}

void XPT2046_Unselect() {
    HAL_GPIO_WritePin(TouchCS_Port, TouchCS_Pin, GPIO_PIN_SET);  // CS High
}

uint16_t XPT2046_ReadCoordinate(uint8_t command)
{
    uint8_t tx_buffer[3] = {command, 0x00, 0x00};
    uint8_t rx_buffer[3] = {0};

    XPT2046_Select();
    HAL_SPI_TransmitReceive(&hspi2, tx_buffer, rx_buffer, 3, HAL_MAX_DELAY);
    XPT2046_Unselect();

    // Składanie 12-bitowego wyniku z odbioru
    return ((rx_buffer[1] << 8) | rx_buffer[2]) >> 3;
}

void XPT2046_ReadCoordinates(uint16_t* x, uint16_t* y)
{
    *x = XPT2046_ReadCoordinate(XPT2046_CMD_X);  // Odczyt X org
    *y = XPT2046_ReadCoordinate(XPT2046_CMD_Y);  // Odczyt Y org
}
//==========================================================================================================================
// prototyp odczytu po DMI CIRCULAR
//#define NUM_COORDINATES 20
//#define DMA_BUFFER_SIZE (NUM_COORDINATES * 4)
//
//// Bufory
//uint8_t txBuffer[DMA_BUFFER_SIZE];  // Bufor danych do wysłania
//uint8_t rxBuffer[DMA_BUFFER_SIZE];  // Bufor danych odbieranych przez DMA
//
//void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
//{
//    if (hspi == &hspi2) {
//        uint16_t coordinateCount = 0;
//
//        // Przejdź przez bufor DMA i odczytaj współrzędne
//        for (uint16_t i = 0; i < DMA_BUFFER_SIZE; i += 4) {
//            // Odczytaj X i Y z rxBuffer
//            uint16_t x = (rxBuffer[i] << 8) | rxBuffer[i + 1];
//            uint16_t y = (rxBuffer[i + 2] << 8) | rxBuffer[i + 3];
//
//            // Przesunięcie bitowe dla 12-bitowego wyniku
//            x >>= 3;
//            y >>= 3;
//
//            // Zapisz wartości do tablic współrzędnych, o ile mieszczą się w limicie
//            if (coordinateCount < NUM_COORDINATES) {
//                xCoordinates[coordinateCount] = x;
//                yCoordinates[coordinateCount] = y;
//                coordinateCount++;
//            } else {
//                break; // Zatrzymaj, jeśli osiągnięto maksymalny rozmiar
//            }
//        }
//
//        // Dezaktywuj CS po zakończeniu transmisji
//        HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);
//
//        // Ustaw flagę zdarzenia dotyku
//        TSInterrupt = 1;
//    }
//}
//
//
//// wywolane w main po inicjalizacji MX_DMA_Init(); MX_SPI2_Init();
//void PrepareTxBuffer(void)
//{
//    for (int i = 0; i < DMA_BUFFER_SIZE; i += 4) {
//        txBuffer[i] = XPT2046_CMD_X;  // Komenda odczytu X
//        txBuffer[i + 1] = 0x00;      // Dummy byte
//        txBuffer[i + 2] = XPT2046_CMD_Y;  // Komenda odczytu Y
//        txBuffer[i + 3] = 0x00;      // Dummy byte
//    }
//}
//
//void Touch_ReadMultipleCoordinatesDMA(void)
//{
//    HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);  // Aktywuj CS
//
//    // Rozpocznij transmisję SPI z DMA
//    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive_DMA(&hspi2, txBuffer, rxBuffer, DMA_BUFFER_SIZE);
//
//    if (status != HAL_OK) {
//        HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);  // Dezaktywuj CS w razie błędu
//        Error_Handler();  // Obsługa błędu (opcjonalnie)
//    }
//}
//==========================================================================================================================

bool CheckCoordinatesWithinRadius(int FromPointX, int FromPointY, int InRadius, int PointXInRange, int PointYInRange)
{
    int deltaX = PointXInRange - FromPointX;
    int deltaY = PointYInRange - FromPointY;
    int distanceSquared = deltaX * deltaX + deltaY * deltaY;
    int radiusSquared = InRadius * InRadius;

    return distanceSquared <= radiusSquared; // Sprawdzenie, czy odległość kwadratowa jest w granicach promienia
}

bool CheckCoordinatesWithinRectangle(int FromPointX, int FromPointY, int RectangleWidth, int RectangleHeight, int PointXInRange, int PointYInRange)
{
    return (PointXInRange >= FromPointX - RectangleWidth && PointXInRange <= FromPointX + RectangleWidth &&
            PointYInRange >= FromPointY - RectangleHeight && PointYInRange <= FromPointY + RectangleHeight);
//    return (PointXInRange >= FromPointX - RectangleWidth / 2 && PointXInRange <= FromPointX + RectangleWidth / 2 &&
//              PointYInRange >= FromPointY - RectangleHeight / 2 && PointYInRange <= FromPointY + RectangleHeight / 2);
}

int scaleBetween(int value, int oldMin, int oldMax, int newMin, int newMax)
{
//    if (max == min) {
//        return minAllowed; // Uniknięcie dzielenia przez zero
//    }

//    int retVal = (unscaledNum - min) * (maxAllowed - minAllowed) / (max - min) + minAllowed;
    int retVal=  ((value - oldMin) * (newMax - newMin)) / (oldMax - oldMin) + newMin;

    // Zabezpieczenie wyniku w zakresie [minAllowed, maxAllowed]
//    if (retVal < minAllowed) {
//        retVal = minAllowed;
//    } else if (retVal > maxAllowed) {
//        retVal = maxAllowed;
//    }
    retVal = abs(retVal);
    return retVal;
}

//int scaleBetween(int unscaledNum, int minAllowed, int maxAllowed, int min, int max)
//{
//	unscaledNum = unscaledNum*100;  // to sie sprawdza gdy mamy floaty !
//	minAllowed = minAllowed*100;
//	maxAllowed = maxAllowed*100;
//	min = min*100;
//	max = max*100;
//
//	int retVal = ((maxAllowed - minAllowed) * (unscaledNum - min) / (max - min) + minAllowed ) /100;
//	retVal = abs(retVal);
//
//
//  return retVal;
//}


void sortArray(uint16_t arr[], uint16_t size)
{
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Zamiana elementów
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void TouchInterrupt(volatile uint16_t *touch_x, volatile uint16_t *touch_y, volatile int *IsReceivedNewCoords)
{
	const uint16_t OdrzucSkrajne = 2; // Liczba próbek do odrzucenia (najmniejszych i największych)
	uint16_t xSum = 0, ySum = 0;
	uint16_t validSamples = NUM_COORDINATES - OdrzucSkrajne; // Liczba próbek po odrzuceniu

	uint16_t xCoord =0;
	uint16_t yCoord =0;

	for(uint8_t d = 0; d < NUM_COORDINATES; d++)
	{
		XPT2046_ReadCoordinates(&xCoord, &yCoord);
		xCoordinates[d]=xCoord;
		yCoordinates[d]=yCoord;
		xCoord=0;
		yCoord =0;
	}

	sortArray(xCoordinates, NUM_COORDINATES);
	sortArray(yCoordinates, NUM_COORDINATES);
	// Sumowanie próbek po odrzuceniu skrajnych wartości
	for (uint8_t i = OdrzucSkrajne; i < NUM_COORDINATES -OdrzucSkrajne ; i++)
	{
		xSum += xCoordinates[i];
		ySum += yCoordinates[i];
	}

	// Obliczanie średniej
	uint16_t x = xSum / validSamples;
	uint16_t y = ySum / validSamples;

	// Weryfikacja współrzędnych w zakresie dotyku
	if (x >= TouchPointXStart && x <= TouchPointXEnd && y >= TouchPointYStart && y <= TouchPointYEnd)
	{
		releaseCount = 0;
		lcdOrientationTypeDef lcdOrientation = lcdGetOrientation();
//		touch_x = x;
//		touch_y = y;
		uint16_t DrawPointX = 0;
		uint16_t DrawPointY = 0;

		if(lcdOrientation == LCD_ORIENTATION_PORTRAIT || lcdOrientation == LCD_ORIENTATION_PORTRAIT_MIRROR)
		{
			*touch_x = x-CalibrateX;
			*touch_y = y+CalibrateY;
			DrawPointX =(uint16_t) XPT2046_ScrWidth - scaleBetween(*touch_x, TouchPointXStart, TouchPointXEnd, 0, XPT2046_ScrWidth);
			DrawPointY =(uint16_t) scaleBetween(*touch_y, TouchPointYStart, TouchPointYEnd, 0, XPT2046_ScrHigh);

		}
		else if (lcdOrientation == LCD_ORIENTATION_LANDSCAPE || lcdOrientation == LCD_ORIENTATION_LANDSCAPE_MIRROR)
		{
			*touch_x = x-CalibrateX;
			*touch_y = y+CalibrateY;
			DrawPointY =(uint16_t) scaleBetween(*touch_x, TouchPointXStart, TouchPointXEnd, 0, XPT2046_ScrWidth);
			DrawPointX =(uint16_t) scaleBetween(*touch_y, TouchPointYStart, TouchPointYEnd, 0, XPT2046_ScrHigh);
		}

//		lcdDrawCircle(DrawPointX,DrawPointY, 10,COLOR_RED);
//		lcdSetTextFont(&Font12);
//		lcdSetCursor(0, lcdGetHeight() - lcdGetTextFont()->Height - 1);
//		lcdSetTextColor(COLOR_WHITE, COLOR_BLACK);
//		lcdPrintf("                              ");
//		lcdSetCursor(0, lcdGetHeight() - lcdGetTextFont()->Height - 1);
//		lcdPrintf("X:Y:%u:%u PP:%u:%u", *touch_x, *touch_y, DrawPointX, DrawPointY);

		*touch_x = DrawPointX;
		*touch_y = DrawPointY;
		*IsReceivedNewCoords = 1;
	}
	else
	{
		releaseCount++;
		if (releaseCount >= 3)
		{
			*IsReceivedNewCoords = 0;
			releaseCount = 0;
		}
	}

	//DelayUS(50);

//	__HAL_GPIO_EXTI_CLEAR_FLAG(TOUCH_INTERRUPT_Pin);
//	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void DrawRectangle(uint16_t color, int cx, int cy, int width, int hight)
{
	lcdDrawCircle(cx, cy, 10, COLOR_BLACK);
	 cx = cx - width / 2;
	 cx = cx;
	 if(cx <=0) cx =0;

	 cy = cy - hight / 2;
	 cy = cy;
	 if(cy <=0) cy =0;
    lcdDrawRect(cx, cy, width, hight, color);
}


void testRectsWawero(uint16_t color, int width, int hight, int pointerSensivitySize)
{
	lcdFillRGB(COLOR_BLACK);
	lcdSetTextFont(&Font12);
	lcdSetTextColor(COLOR_WHITE, COLOR_BLACK);
	lcdSetCursor(0, 0);
	lcdPrintf("Size h:%u  w:%u PSS: %u", width, hight, pointerSensivitySize);

	int cx = lcdGetWidth()  / 2 - width/2;
	int cy = lcdGetHeight() / 2 - hight/2;
  //lcdFillRGB(COLOR_BLACK);
    lcdDrawRect(cx, cy, width, hight, color);
}

void testCircleWawero(uint16_t color, int radious, int pointerSensivitySize)
{
	lcdFillRGB(COLOR_BLACK);
	lcdSetTextFont(&Font12);
	lcdSetTextColor(COLOR_WHITE, COLOR_BLACK);
	lcdSetCursor(0, 0);
	lcdPrintf("Size r:%u PSS:%u", radious, pointerSensivitySize);

	int cx = lcdGetWidth()  / 2;
	int cy = lcdGetHeight() / 2;
    lcdDrawCircle(cx, cy, radious /2, color);
}

