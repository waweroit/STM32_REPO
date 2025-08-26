/*
 * USARTCommunication.c
 *
 *  Created on: Jul 14, 2025
 *      Author: wawer
 */

#include "USARTCommunication.h"

bool isTransmissionComplete = false;
char txBuffer[TX_BUFFER_SIZE];

void InitUSART()
{
	HAL_UART_DeInit(Channel_USART);
//	DelayUS(500);
	HAL_Delay(100);
	MX_USART1_UART_Init();


	//HAL_UART_Receive_IT(Channel_USART, (uint8_t *)&rxData, 1);  // Włącz przerwania RX
}


HAL_StatusTypeDef Send(const char *text)
{
//    HAL_StatusTypeDef retStatus = HAL_UART_Transmit(ESP_USART, (uint8_t*)command, strlen(command), HAL_MAX_DELAY);
    HAL_StatusTypeDef retStatus = HAL_UART_Transmit_IT(Channel_USART, (uint8_t*)text, strlen(text));

    if (retStatus == HAL_OK)
    {
    	int breakTimeStart = HAL_GetTick();
    	int breakAfterMS = 10000;
    	int breakTimeNOW = 0;

        while (!isTransmissionComplete)
        {
        	breakTimeNOW = HAL_GetTick();

    		if((uint32_t)(breakTimeNOW - breakTimeStart) >= breakAfterMS)
    		{
    			// send timeout
    		}
        }


        isTransmissionComplete = false; // Reset flagi

    }
    else
    {

        // Obsłuż błąd transmisji
    }
    return retStatus;
}
