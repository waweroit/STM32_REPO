/*
 * ZMPT101BSource.cpp
 *
 *  Created on: Aug 22, 2024
 *      Author: waweroIT
 */
#include "ZMPT101BHeader.h"
#include "math.h"
//#include "tim.h"
#include <stdlib.h>
#include <stdio.h>

uint32_t micros_counter = 0;
int32_t zero = ZEROPOINT;
uint32_t min = ADC_SCALE;
uint32_t max = 0;

float Vzero = VREF / 2.0;
float Vref = VREF;
float sensitivity;


//test only
int saturationCount = 0;
int rawDMARead = 0;


ADC_HandleTypeDef* hadc;


void Initialize_ZMPT101B(ADC_HandleTypeDef* _hadc)
{
    hadc = _hadc;
    //sensitivity = 0.005; //orginalne
    sensitivity = 0.00268; //orginalne

}

uint16_t analogRead() {

    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
    uint16_t adcValue = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);

    return adcValue;
}

int calibrate() {
    uint32_t acc = 0;

    for (int i = 0; i < 1000; i++) {
        acc += analogRead();
    }
    zero = acc / 1000;

    return zero;
}

int calibrateLive() {
    uint32_t acc = 0;

    for (int i = 0; i < 100; i++) {
        acc += analogRead();
        delayMicroseconds(1000);
    }
    zero = acc / 100;

    return zero;
}

void calibrateLowNoiseAndZeroPoint()
{
	min = ADC_SCALE;
	max = 0;
	uint32_t acc = 0;
	uint32_t countMiddleZero = 0;

    for (int i = 0; i < 1000; i++)
    {
    	acc = analogRead();
    	countMiddleZero += acc;
		if(acc <= min)
			min = acc;
        delayMicroseconds(1000);
    }

//    zero = countMiddleZero / 100;
//    if(min < 300)
//    	zero = zero - min;
    if(min < (ZEROPOINT / 2 ))
		zero = ZEROPOINT - min;
    else
    {
    	zero = min;
    }
}

// modyfikuje zeropoint(2048) o odczytaną najniższą wartosc z ADC
// przyklad najniższa wartość z tablicy to 50 wiec zero = (ZEROPOINT 2048) - najniższa
// wiec zero = 2048 - 50.
void calibrateLowNoisePointFromDMA(uint32_t *Buffer, int size)
{
	min = ADC_SCALE;
	uint32_t acc = 0;

    for (int i = 0; i < size; i++)
    {
    	acc = Buffer[i];
		if(acc <= min)
			min = acc;
    }

    if(min < (ZEROPOINT / 2))
		zero = ZEROPOINT - min;
    else
    {
    	zero = min;
    }
}

// modyfikuje zeropoint(2048) o średnią wartość odczytaną z ADC
// tolerant np 50
void calibrateAverageZeroPointFromDMA(uint32_t *Buffer, int size, int tolerance)
{
	// Użycie typu uint64_t dla sumy w celu uniknięcia przepełnienia przy dużych buforach
	uint64_t sum = 0;

	// Iteracja wskaźnikowa przez bufor
	uint32_t* ptr = Buffer;
	uint32_t* end = Buffer + size;

	// Oblicz sumę wartości w buforze
	while (ptr < end) {
		sum += *ptr++;
	}

	// Obliczenie średniej wartości
	uint32_t average = sum / size;

	// Sprawdzenie, czy różnica przekracza margines tolerancji
	if (zero > average + tolerance || zero < average - tolerance)
	{
		zero = average;  // Aktualizacja zerowego punktu odniesienia
	}
}

// Funkcja kalibrująca punkt zerowy na podstawie różnicy między średnią a minimalną wartością z bufora DMA
// tylko wtedy, gdy różnica przekracza ustalony próg tolerancji.
// Parametry:
// - Buffer: wskaźnik na bufor danych z DMA
// - size: rozmiar bufora
// - tolerance: próg tolerancji dla zmiany wartości `zero`
void calibrateZeroPointWithTolerance(uint32_t *Buffer, int size, int tolerance)
{
    min = ADC_SCALE;  // Inicjalizacja minimalnej wartości jako maksymalna możliwa wartość ADC
    max = 0;
    uint64_t sum = 0;          // Użycie uint64_t, aby uniknąć przepełnienia przy dużej liczbie próbek

    if (Buffer || size != 0)
    {
		// Iteracja przez bufor w celu obliczenia sumy i znalezienia minimalnej wartości
		for (int i = 0; i < size; i++)
		{
			uint32_t value = Buffer[i];
			sum += value;  // Dodawanie wartości do sumy

			// Znajdowanie minimalnej wartości
			if (value < min)
			{
				min = value;
			}
			if (value > max)
			{
				max = value;
			}
		}

		// Obliczenie średniej wartości
		uint32_t average = sum / size;

		// Wprowadzenie tolerancji, aby zredukować wpływ gwałtownych zmian na zero
		if (abs((int32_t)(zero - average)) > tolerance)
		{
			zero = average;
		}
    }
}


float calibrateVoltage() {
//    uint32_t acc = 0;
//
//    for (int i = 0; i < 100; i++) {
//        acc += analogRead();
//        delayMicroseconds(1000);
//    }
    //zero = acc / 100; // wyremowalem jesli bedzie taka potrzeba uzyj calibrate()
    Vzero = zero * Vref / ADC_SCALE;


    return Vzero;
}


float getVoltageDC() {
    int32_t acc = 0;

    for (int i = 0; i < 10; i++) {
        acc += analogRead() - zero;
    }
    float V = (float)acc / 10.0 / ADC_SCALE * Vref / sensitivity;

    return V;
}

float getVoltageAC(uint16_t frequency) {
    uint32_t period = 1000000 / frequency;
    uint32_t t_start = micros();

    uint32_t Vsum = 0;
    uint32_t measurements_count = 0;
    int32_t Vnow;

    while (micros() - t_start < period) {
        Vnow = analogRead() - zero;
        Vsum += Vnow * Vnow;
        measurements_count++;
    }
    float Vrms = sqrt(Vsum / measurements_count) / ADC_SCALE * Vref / sensitivity;

    return Vrms;
}

//float getVoltageACFromDMABuff(uint32_t *Buffer, int size)
//{
//
//	uint32_t Vsum = 0;
//	uint32_t measurements_count = 0;
//	int32_t Vnow;
//
//	for(int i = 0; i < size; i ++)
//	{
//		Vnow = Buffer[i] - zero;
//		Vsum += Vnow * Vnow;
//		measurements_count++;
//	}
//
//	float Vrms = sqrt(Vsum / measurements_count) / ADC_SCALE * Vref / sensitivity;
//
//	return Vrms;
//}

// Zoptymalizowana funkcja
float getVoltageACFromDMABuff(uint32_t *Buffer, int size, bool adjSensivity)
{
    uint64_t Vsum = 0;     // Suma kwadratów
    int32_t Vnow;          // Aktualna wartość pomiaru przesunięta względem zera
    uint32_t *ptr = Buffer;  // Wskaźnik na początek bufora
    uint32_t *end = Buffer + size;  // Wskaźnik na koniec bufora
    int countWithinZero = 0; // Licznik próbek w granicach punktu zero
	int tolerance = 400; //

    if (!Buffer || size < 1) {
        return 0.0f;
    }

	// Wyliczenie progu 80% dla wcześniejszego zakończenia pętli
	int thresholdWithinZero = size * 0.8;  // Próg dla zakończenia, jeśli 80% próbek mieści się w granicach zero

    // Iteracja przez bufor za pomocą wskaźników
    while (ptr < end) {
        Vnow = (int32_t)(*ptr++) - zero;  // Odczyt wartości i przesunięcie wskaźnika
        Vsum += (uint32_t)(Vnow * Vnow);  // Sumowanie kwadratów

        // Sprawdzanie, czy wartość mieści się w granicach tolerancji
		if (abs(Vnow) < tolerance)
		{
			countWithinZero++; // Zliczanie próbek w okolicy zero
		}
	    // Sprawdzanie, czy przekroczono próg 80%
	    if (countWithinZero >= thresholdWithinZero || (zero - min) < 400)
	    {
	        return 0.0;  // Zwracaj 0V, jeśli 80% próbek jest blisko zerowego punktu
	    }
    }


    // Obliczanie wartości skutecznej (Vrms)
	//float Vrms = sqrtf((float)Vsum / size) / ADC_SCALE * Vref / sensitivity;
    float Vrms = sqrtf((float)Vsum / size) / max * Vref / sensitivity;


    // test only - dostrojenie sensivity
    if(adjSensivity== true)
    {
		 if(Vrms < 236)
			sensitivity -= 0.00001;
		else if(Vrms > 237)
			sensitivity += 0.00001;
    }

    return Vrms;
}

int checkADCSaturationTest(uint32_t* Buffer, int size)
{
    saturationCount = 0;

    if (!Buffer || size == 0) {
        return 1;
    }

    for (int i = 0; i < size; i++)
    {
    	rawDMARead = Buffer[i];
        if (Buffer[i] == 4095)
        {
            saturationCount++;
        }
        delayMicroseconds(1000);
    }

    // Jeżeli więcej niż 10% próbek ma wartość 4095, oznacza to problem z saturacją - wymagana kalibracja
    if (saturationCount > (size / 10)) {
    	return 1;
    }

    return 0;
}


uint32_t micros() {
    //return __HAL_TIM_GET_COUNTER(&htim10);  // Zakładam, że używasz timera do śledzenia czasu
	return HAL_GetTick() * 1000;
}

void delayMicroseconds(uint32_t us) {
    uint32_t start = micros();
    while ((micros() - start) < us);
}
