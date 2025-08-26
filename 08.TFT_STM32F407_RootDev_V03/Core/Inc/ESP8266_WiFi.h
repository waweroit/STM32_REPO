/*
 * ESP8266_WiFi.h
 *
 *  Created on: Dec 17, 2024
 *      Author: wawer
 */
#pragma once
#ifndef INC_ESP8266_WIFI_H_
#define INC_ESP8266_WIFI_H_
#include "stm32f4xx_hal.h"
#include "waweroBasicFunction.h"
#include "DataTime.h"
#include "SPITouchScreen.h"
#include <stdbool.h>
#include <string.h> // Potrzebne do strcmp, strcpy
#include <stdio.h>
#include "tim.h"

#define ESP_USART &huart1
#define TIMERSEC &htim3

// Rozmiar bufora odbiorczego
#define RX_BUFFER_SIZE 200
#define TX_BUFFER_SIZE 200

#define BufferTableSize 4

#define respTableSize 8

//#define WIFI_SSID "w@wero"
//#define WIFI_PASSWORD "Dupcia2912"

typedef enum {
	AT_ECHO,
    WIFI_DISCONNECT,
    WIFI_CONNECTED,
    WIFI_GOT_IP,
	WIFI_NO_IP,
	WIFI_SATUS_1_CONNECTED_NO_TCP_UPD,
	WIFI_SATUS_2_CONNECTED_GOT_IP,
	WIFI_SATUS_3_CONNECTED_ACTIVE,
	WIFI_SATUS_4_CONNECTION_LOST,
	WIFI_SATUS_5_NO_CONNECTION_TO_WIFI,
    RESPONSE_OK,
	RESPONSE_ALREADY_CONNECTED,
    RESPONSE_CONNECT,
    RESPONSE_CLOSED,
	RESPONSE_NTP_RECEIVED48BYTES,
	RESPONSE_IPD,
	RESPONSE_IPD_FAIL,
	RESPONSE_BUSY,
    RESPONSE_ERROR,
	RESPONSE_ERROR_LOSE,
	RESPONSE_TIMEOUT,
	RESPONSE_SEND_OK,
	RESPONSE_END_OK,
	RESPONSE_SEND_FAIL,
	ESP_WAIT_FOR_DATA,
	EMPTY_IGNORE,
	OVERLOAD,
    RESPONSE_UNKNOWN
} ResponseType;

typedef struct {
	bool rxWasReaded;
	uint16_t rxWriteLength;
    char rxBuffer[RX_BUFFER_SIZE];
} RX_Buffer;


extern ResponseType acceptableResponses[respTableSize];

extern volatile bool isTransmissionComplete;

extern volatile uint8_t ActualBuffer;
extern volatile uint16_t rxWriteIndex;
extern volatile uint8_t rxData;
extern bool ESP8266_isConnected;
extern bool gotIPAddress;
extern volatile RX_Buffer BufferTable[BufferTableSize];

extern bool procesNTPTime;
extern volatile bool WaitForNTPResp;

extern volatile bool StartCollectingNTPRespons;
extern volatile uint16_t NTPResponsLength;
extern bool TimerInit;

extern char WIFI_SSID[50];
extern char WIFI_PASSWORD[50];

// Funkcje
void InitESP8266();
void ResetESPPinout();
void ESPDisableEcho();
void ESPSetStationMode();
void ESPDisableMultipleConnection();
void ESPSetRFPower();
void ESPCheckVerion();
void ESPSendResetEsp();
void ESPSendFactoryReset();
void ChangeUSARTSpeed(int speed);

HAL_StatusTypeDef SendToESP(const char *command);
void ConnectToWiFi(const char *ssid, const char *password);
bool CheckWiFiConnectionStatus();
bool CheckCIPSTATUS();

ResponseType IsRespondCorrect(const ResponseType RespondExpected, int waitTimeMS);
ResponseType ParseResponseType(const int checkBufferIndex);
ResponseType ProcessReceivedData(const ResponseType expected);
bool isIncluded(ResponseType response);

void ClearResponseTable();
void ClearResponseTableBuffer();

//bool ProcessNTPResponse(const uint8_t *response);
bool GetTimeFromNTP();
//void IncrementLocalTime(ActualDate *date);


//void GetTimeFromHTTP(void);
//void ProcessHTTPResponse(char *dateTimeStart);

#endif /* INC_ESP8266_WIFI_H_ */
