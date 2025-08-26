/*
 * USARTCommunication.h
 *
 *  Created on: Jul 14, 2025
 *      Author: wawer
 */

#ifndef INC_USARTCOMMUNICATION_H_
#define INC_USARTCOMMUNICATION_H_

#include <stdbool.h>
#include <string.h> // Potrzebne do strcmp, strcpy
#include "usart.h"

#define Channel_USART &huart1

#define RX_BUFFER_SIZE 200
#define TX_BUFFER_SIZE 200

extern bool isTransmissionComplete;
extern char txBuffer[TX_BUFFER_SIZE];


HAL_StatusTypeDef Send(const char *text);

#endif /* INC_USARTCOMMUNICATION_H_ */
