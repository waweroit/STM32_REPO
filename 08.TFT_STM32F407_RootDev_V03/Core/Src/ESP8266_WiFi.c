/*
 * ESP8266_WiFi.c
 *
 *  Created on: Dec 17, 2024
 *      Author: wawer
 */

#include "ESP8266_WiFi.h"
#include "actions.h"
#include "usart.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern TIM_HandleTypeDef htim3;

volatile bool isTransmissionComplete = false;
volatile uint8_t ActualBuffer = 0;
volatile uint16_t rxWriteIndex = 0;     // Wskaźnik zapisu
volatile uint8_t rxData = 0;
bool ESP8266_isConnected = false;
bool gotIPAddress = false;
volatile RX_Buffer BufferTable[BufferTableSize] = {0};


bool procesNTPTime = false;
uint8_t NTPpoolServer =0;
uint8_t maxNTPPoolServers= 3; //bo liczymy od zera :)

#pragma GCC optimize ("O0") // Wyłącza optymalizację
volatile bool WaitForNTPResp = false;
#pragma GCC optimize ("Og")  // Przywraca domyślną optymalizację

volatile bool StartCollectingNTPRespons = false;
volatile uint16_t NTPResponsLength =0;
bool TimerInit = false;

char txBuffer[TX_BUFFER_SIZE];

char WIFI_SSID[50]= {0};
char WIFI_PASSWORD[50]= {0};

const char* responseNames[] = {
    "AT_ECHO",
    "WIFI_DISCONNECT",
    "WIFI_CONNECTED",
    "WIFI_GOT_IP",
	"WIFI_NO_IP",
	"WIFI_SATUS_1_CONNECTED_NO_TCP_UPD",
	"WIFI_SATUS_2_CONNECTED_GOT_IP",
	"WIFI_SATUS_3_CONNECTED_ACTIVE",
	"WIFI_SATUS_4_CONNECTION_LOST",
	"WIFI_SATUS_5_NO_CONNECTION_TO_WIFI",
    "RESPONSE_OK",
    "RESPONSE_ALREADY_CONNECTED",
    "RESPONSE_CONNECT",
    "RESPONSE_CLOSED",
	"RESPONSE_NTP_RECEIVED48BYTES",
    "RESPONSE_IPD",
    "RESPONSE_IPD_FAIL",
    "RESPONSE_BUSY",
    "RESPONSE_ERROR",
    "RESPONSE_ERROR_LOSE",
    "RESPONSE_TIMEOUT",
    "RESPONSE_SEND_OK",
    "RESPONSE_END_OK",
    "RESPONSE_SEND_FAIL",
    "ESP_WAIT_FOR_DATA",
    "EMPTY_IGNORE",
    "OVERLOAD",
    "RESPONSE_UNKNOWN"
};
int intFifoRespSize = 0;
ResponseType acceptableResponses[respTableSize];


void InitESP8266()
{
	HAL_UART_DeInit(&huart1);
	DelayUS(500);
	MX_USART1_UART_Init();

	ClearResponseTable();
	ClearResponseTableBuffer();

	HAL_UART_Receive_IT(ESP_USART, (uint8_t *)&rxData, 1);  // Włącz przerwania RX
	ResetESPPinout();
	ESPDisableEcho();
	ESPSetStationMode();
	ESPDisableMultipleConnection();
//	ESPSetRFPower();
//	ConnectToWiFi(WIFI_SSID, WIFI_PASSWORD);
	//SetWifiStatusFlagsLVGL();
}

void ClearResponseTable()
{
	intFifoRespSize = 0;
	for(int i=0; i < respTableSize; i++)
	{
		acceptableResponses[i] = EMPTY_IGNORE;
	}
}

void ClearResponseTableBuffer()
{
	// inicjalizacja tabeli :)
	for(int i=0; i < BufferTableSize; i++)
	{
	  BufferTable[i].rxWasReaded = true;
	  BufferTable[i].rxWriteLength = 0;
	  BufferTable[i].rxWriteLength = RESPONSE_UNKNOWN;
	  memset((void *)BufferTable[i].rxBuffer, 0, RX_BUFFER_SIZE);
	}
}

void ResetESPPinout()
{
	HAL_GPIO_WritePin(ESP_ResetPin_GPIO_Port, ESP_ResetPin_Pin, GPIO_PIN_RESET);
	DelayUS(1000000);
	HAL_GPIO_WritePin(ESP_ResetPin_GPIO_Port, ESP_ResetPin_Pin, GPIO_PIN_SET);
	DelayUS(1000000); // Czekaj na gotowość ESP-01
}

void ESPDisableEcho()
{
	SendToESP("ATE0\r\n");  // Wylacz Echo
	IsRespondCorrect(RESPONSE_OK,2000);
}

void ESPDisableMultipleConnection()
{
	SendToESP("AT+CIPMUX=0\r\n");;
	IsRespondCorrect(RESPONSE_OK,2000);
}

void ESPSetStationMode()
{
	SendToESP("AT+CWMODE_CUR=1\r\n");;
	IsRespondCorrect(RESPONSE_OK,2000);
}

void ESPSetRFPower()
{
	SendToESP("AT+RFPOWER=82\r\n");;
	IsRespondCorrect(RESPONSE_OK,2000);
}

void ESPCheckVerion()
{
	//Debug("Send AT+GMR to ESP:\r\n");
	SendToESP("AT+GMR\r\n");
	IsRespondCorrect(RESPONSE_OK,2000);
}

void ESPSendResetEsp()
{
//		Debug("Send RST to ESP:\r\n");
	SendToESP("AT+RST\r\n");  // Restart ESP
	IsRespondCorrect(RESPONSE_OK,5000);
	DelayUS(3000000);
}

void ESPSendFactoryReset()
{
	SendToESP("AT+RESTORE\r\n");  // Factory reset
	DelayUS(3000000);
}

void ChangeUSARTSpeed(int speed)
{
	char command[50];
	snprintf (command, sizeof(command), "AT+UART_CUR=%d,8,1,0,0\r\n", speed);
	SendToESP(command);
//	SendToESP("AT+UART_CUR=9600,8,1,0,0\r\n");

	if(debugOn)
	{
		snprintf (command, sizeof(command), "Send BaudRate %d config\r\n", speed);
		Debug(command);
	}
	DelayUS(1000000);
	HAL_UART_AbortReceive_IT(ESP_USART);
	*ESP_USART.Instance = USART1;
	*ESP_USART.Init.BaudRate = speed;
	if (HAL_UART_Init(ESP_USART) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_UART_Receive_IT(ESP_USART, (uint8_t *)&rxData, 1);  // Włącz przerwania RX
	IsRespondCorrect(RESPONSE_OK,5000);
}
// Funkcja wysyłania danych do ESP-01
HAL_StatusTypeDef SendToESP(const char *command)
{
	ClearResponseTableBuffer();
//    HAL_StatusTypeDef retStatus = HAL_UART_Transmit(ESP_USART, (uint8_t*)command, strlen(command), HAL_MAX_DELAY);
    HAL_StatusTypeDef retStatus = HAL_UART_Transmit_IT(ESP_USART, (uint8_t*)command, strlen(command));

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
    	    	if(debugOn)
    	    	{
    				Debug("SEND Timeout\r\n");
    	    	}
    			break;
    		}
        }

        if(debugOn)
        	Debug(command);
//        else
//            DelayUS(100);

        isTransmissionComplete = false; // Reset flagi

//        memset((void*)command, 0, sizeof(command));
    }
    else
    {
    	Debug("BLAD TRANSMISJI");
        // Obsłuż błąd transmisji
    }

    return retStatus;
}

// Funkcja konfigurująca Wi-Fi
void ConnectToWiFi(const char *ssid, const char *password)
{
    char command[50];
    snprintf (command, sizeof(command), "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", ssid, password);
    SendToESP(command);

    if(debugOn)
    {
    	Debug(command);
    }
    IsRespondCorrect(WIFI_CONNECTED,10000);
	if(isIncluded(WIFI_CONNECTED) || isIncluded(WIFI_GOT_IP))
	{
		//IsRespondCorrect(WIFI_GOT_IP,2000);
		ESP8266_isConnected = true;
	}
	else
	{
		ESP8266_isConnected = false;
	}
}

bool CheckWiFiConnectionStatus()
{
    ResponseType respond = IsRespondCorrect(WIFI_DISCONNECT,0);
    if(respond == WIFI_DISCONNECT)
    {
		ESP8266_isConnected=false;
		//SetWifiStatusFlagsLVGL();

		return CheckCIPSTATUS();
    }
	return true;
}

bool CheckCIPSTATUS()
{
	SendToESP("AT+CIPSTATUS\r\n");  // Restart ESP
	IsRespondCorrect(WIFI_SATUS_2_CONNECTED_GOT_IP,5000);

	if(isIncluded(WIFI_CONNECTED) || isIncluded(WIFI_GOT_IP)|| isIncluded(WIFI_SATUS_1_CONNECTED_NO_TCP_UPD) || isIncluded(WIFI_SATUS_2_CONNECTED_GOT_IP) || isIncluded(WIFI_SATUS_3_CONNECTED_ACTIVE))
	{
		ESP8266_isConnected=true;


		//SetWifiStatusFlagsLVGL();


		return true;
	}
	else
	{
		ESP8266_isConnected=false;


		//SetWifiStatusFlagsLVGL();


		InitESP8266();
		return false;
	}
}

// Funkcja mapująca odpowiedzi ESP na typ
ResponseType ParseResponseType(const int checkBufferIndex)
{

	ResponseType respond = RESPONSE_UNKNOWN;


	if (BufferTable[checkBufferIndex].rxBuffer[0] == '\0')
		respond = EMPTY_IGNORE;
	else
		if (BufferTable[checkBufferIndex].rxBuffer[0] =='\r' && BufferTable[checkBufferIndex].rxBuffer[1]=='\n')
		respond = EMPTY_IGNORE;

	else if (BufferTable[checkBufferIndex].rxBuffer[0] == '>' )
		respond = ESP_WAIT_FOR_DATA;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "WIFI DISCONNECT"))
		respond = WIFI_DISCONNECT;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "WIFI CONNECTED"))
		respond = WIFI_CONNECTED;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "WIFI GOT IP"))
		respond = WIFI_GOT_IP;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "no ip"))
		respond = WIFI_NO_IP;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "STATUS:1"))
		respond = WIFI_SATUS_1_CONNECTED_NO_TCP_UPD;
	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "STATUS:2"))
		respond = WIFI_SATUS_2_CONNECTED_GOT_IP;
	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "STATUS:3"))
		respond = WIFI_SATUS_3_CONNECTED_ACTIVE;
	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "STATUS:4"))
		respond = WIFI_SATUS_4_CONNECTION_LOST;
	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "STATUS:5"))
		respond = WIFI_SATUS_5_NO_CONNECTION_TO_WIFI;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "ALREADY CONNECTED"))
		respond = RESPONSE_ALREADY_CONNECTED;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "CONNECT"))
		respond = RESPONSE_CONNECT;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "CLOSED"))
		respond = RESPONSE_CLOSED;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "Recv 48 bytes"))
		respond = RESPONSE_NTP_RECEIVED48BYTES;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "IPD"))
		respond = RESPONSE_IPD;//+IPD

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "busy"))
		respond = RESPONSE_BUSY;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "SEND OK"))
		respond = RESPONSE_SEND_OK;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "END OK"))
		respond = RESPONSE_END_OK;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "OK") || strstr((void*)BufferTable[checkBufferIndex].rxBuffer, " OK"))
		respond = RESPONSE_OK;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "SEND FAIL"))
		respond = RESPONSE_SEND_FAIL;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "ERROR"))
		respond = RESPONSE_ERROR;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "CIP LOSE"))
		respond = RESPONSE_ERROR_LOSE;

//	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, ">"))
//		respond = ESP_WAIT_FOR_DATA;

	else if (strstr((void*)BufferTable[checkBufferIndex].rxBuffer, "AT"))
		respond = AT_ECHO;

	else
		respond = RESPONSE_UNKNOWN;

//	snprintf (debugData, sizeof(debugData),"ParseResponseType: %d %s\r\n", respond, responseNames[respond]);
//	Debug(debugData);

	return respond;

}

// Funkcja sprawdzająca, czy odpowiedź jest na liście
bool isIncluded(ResponseType response)
{
    for (size_t i = 0; i < respTableSize; i++)
    {
        if (response == acceptableResponses[i])
        {
        	acceptableResponses[i] = EMPTY_IGNORE;
            return true;  // Znaleziono odpowiedź na liście
        }
    }
    return false;
}

ResponseType IsRespondCorrect(const ResponseType ResponsExpected, int waitTimeMS)
{
	ClearResponseTable();
	ResponseType respons = EMPTY_IGNORE;
	int breakAfterMS = waitTimeMS;
	int breakTimeNOW = 0;
	int unknownMax = 3;
	int breakTimeStart = HAL_GetTick();

	while(true)
	{
		breakTimeNOW = HAL_GetTick();

		if(((uint32_t)(breakTimeNOW - breakTimeStart) >= breakAfterMS) && waitTimeMS > 0)
		{
			if(debugOn)
			{
				snprintf (debugData, sizeof(debugData),"TIMEOUT on expected: %s\r\n",  responseNames[ResponsExpected]);
				Debug(debugData);
			}
			// Time Elapsed return false
			respons = RESPONSE_TIMEOUT;
			break;
		}
		else if (waitTimeMS == 0) // pojedyncze sprawdzenie czy cos tam jest
		{
			return ProcessReceivedData(ResponsExpected);
		}

		respons = ProcessReceivedData(ResponsExpected);

		if(ResponsExpected == respons)
		{
			break;
		}

		else if(unknownMax > 0 && respons == RESPONSE_UNKNOWN )
		{
			// reset Timer
			breakTimeStart = HAL_GetTick();
			unknownMax--;
		}
		else if(unknownMax == 0 && respons == RESPONSE_UNKNOWN )
		{
			respons = RESPONSE_UNKNOWN;
			break;
		}
		else if(respons == EMPTY_IGNORE)
		{
		}
		else if(ResponsExpected == RESPONSE_ERROR)
		{
			respons = RESPONSE_ERROR;
			break;
		}

	//DelayUS(200);


	}

	return respons;
}

ResponseType ProcessReceivedData(const ResponseType expected)
{
	ResponseType response = EMPTY_IGNORE;

	for(int i = 0; i < BufferTableSize; i ++ )
	{
		if(BufferTable[i].rxWasReaded == false)
		{
			response = ParseResponseType(i);

			if( response == AT_ECHO)
			{
			}
			else if( response == WIFI_DISCONNECT)
			{
				if(debugOn)
					Debug("ESP: Rozlaczono z siecia Wi-Fi.\r\n");
				ESP8266_isConnected=false;

				//SetWifiStatusFlagsLVGL();
				//ConnectToWiFi(WIFI_SSID, WIFI_PASSWORD);
			}

			else if( response ==  WIFI_CONNECTED)
			{
				if(debugOn)
					Debug("ESP: Polaczono z siecia Wi-Fi.\r\n");
				gotIPAddress = true;
				ESP8266_isConnected=true;

				//SetWifiStatusFlagsLVGL();
			}

			else if( response ==  RESPONSE_ALREADY_CONNECTED)
			{
				if(debugOn)
					Debug("ESP: Polaczono RESPONSE_ALREADY_CONNECTED.\r\n");
			}

			else if( response ==  WIFI_GOT_IP)
			{
				if(debugOn)
					Debug("ESP: Otrzymano adres IP. Polaczenie Wi-Fi jest gotowe.\r\n");
				gotIPAddress = true;
				ESP8266_isConnected=true;

				//SetWifiStatusFlagsLVGL();
			}

			else if( response ==  WIFI_NO_IP)
			{
				if(debugOn)
					Debug("ESP: Brak adresu IP. Brak polaczenie Wi-Fi.\r\n");
				gotIPAddress = false;
				ESP8266_isConnected=false;

				//SetWifiStatusFlagsLVGL();
			}

			else if( response ==  RESPONSE_OK)
			{
				if(debugOn)
					Debug("ESP: Komenda zakonczona sukcesem.\r\n");
			}

			else if( response ==  RESPONSE_CONNECT)
			{
				if(debugOn)
				{
					Debug("ESP: CONNECTED.\r\n");
				}
			}

			else if( response ==  WIFI_SATUS_1_CONNECTED_NO_TCP_UPD)
			{
				if(debugOn)
				{
					Debug("Polaczono z siecia Wi-Fi, ale brak aktywnego polaczenia TCP/UDP.\r\n");
				}
			}
			else if( response ==  WIFI_SATUS_2_CONNECTED_GOT_IP)
			{
				if(debugOn)
				{
					Debug("Polaczono z siecia Wi-Fi, gotowe do polaczenia TCP/UDP.\r\n");
				}
			}
			else if( response ==  WIFI_SATUS_3_CONNECTED_ACTIVE)
			{
				if(debugOn)
				{
					Debug("Aktywne polaczenie TCP/UDP jest zestawione..\r\n");
				}
			}
			else if( response ==  WIFI_SATUS_4_CONNECTION_LOST)
			{
				if(debugOn)
				{
					Debug("Polaczono TCP/UDP zostalo zamkniete lub zerwane.\r\n");
				}
			}
			else if( response ==  WIFI_SATUS_5_NO_CONNECTION_TO_WIFI)
			{
				if(debugOn)
				{
					Debug("Modul nie jest polaczony z siecia Wi-Fi.\r\n");
				}
			}

			else if(response ==  RESPONSE_CLOSED)
			{
				if(debugOn)
				{
					Debug("ESP: CLOSED.\r\n");
				}
			}

			else if( response ==  RESPONSE_NTP_RECEIVED48BYTES && response == expected)
			{
				if(debugOn)
					Debug("ESP: RESPONSE_NTP_RECEIVED48BYTES.\r\n");
				//IsRespondCorrect(RESPONSE_SEND_OK,1000);
			}

			else if( response ==  RESPONSE_IPD && response == expected)
			{
				if(debugOn)
					Debug("ESP: RESPONSE_IPD\r\n");

				// trzeba tu jeszcze bedzie dodać z kad jest odpowiedz
				// jakis ogolny ustawić  enum w jakim miejscu sie znajduje i jaka wiadomosc zostala wyslana do jakiego hosta :D
				if( strstr((void *)BufferTable[i].rxBuffer, "IPD,48") && expected == RESPONSE_IPD)
				{
					char *dataStart = strstr((void *)BufferTable[i].rxBuffer, ":");
					if (dataStart != NULL)
					{
						dataStart++;  // Przesuń wskaźnik za ":"
						if(ProcessNTPResponse((uint8_t *)dataStart) != true)
							response = RESPONSE_IPD_FAIL;
					}
				}
				else
				{
					if(debugOn)
					{
						Debug("Otrzymano odpowiedz z serwera:\r\n");
						Debug((void *)BufferTable[i].rxBuffer);
					}
					response = RESPONSE_IPD_FAIL;
//			        if (strstr((void *)BufferTable[i].rxBuffer, "datetime") && expected == RESPONSE_IPD)
//			        {
//			        	if(debugOn)
//			        	{
//							Debug("Otrzymano odpowiedź z serwera:\r\n");
//							Debug((void *)BufferTable[i].rxBuffer);
//			        	}
//
//			            // Znalezienie pola "datetime"
//			            char *dateTimeStart = strstr((void *)BufferTable[i].rxBuffer, "\"datetime\":\"");
//			            if (dateTimeStart)
//			            {
//			                ProcessHTTPResponse(dateTimeStart);
//			            }
//			            else
//			            {
//			            	if(debugOn)
//			            		Debug("Nie znaleziono pola datetime w odpowiedzi.\r\n");
//			            }
//			        }
				}
			}

			else if( response ==  RESPONSE_ERROR)
			{
				if(debugOn)
					Debug("ESP: Wystapil blad podczas wykonywania komendy.\r\n");
				memset((void *)BufferTable[i].rxBuffer, 0, RX_BUFFER_SIZE);
				BufferTable[i].rxWasReaded = true;
				break;
			}

			else if( response ==  RESPONSE_ERROR_LOSE)
			{
				if(debugOn)
					Debug("ESP: RESPONSE_ERROR_LOSE.\r\n");
			}

			else if( response ==  RESPONSE_SEND_FAIL)
			{
				if(debugOn)
					Debug("ESP: RESPONSE_SEND_FAIL.\r\n");
			}

			else if( response ==  RESPONSE_BUSY)
			{
				if(debugOn)
					Debug("ESP: RESPONSE_BUSY.\r\n");
			}

			else if( response ==  RESPONSE_SEND_OK)
			{
				if(debugOn)
					Debug("ESP: RESPONSE_SEND_OK.\r\n");
			}

			else if( response ==  RESPONSE_END_OK)
			{
				if(debugOn)
					Debug("ESP: RESPONSE_END_OK.\r\n");
			}

			else if( response ==  ESP_WAIT_FOR_DATA)
			{
				if(debugOn)
					Debug("ESP: ESP_WAIT_FOR_DATA.\r\n");
			}

			else if( response ==  EMPTY_IGNORE)
			{
				memset((void *)BufferTable[i].rxBuffer, 0, RX_BUFFER_SIZE);
				BufferTable[i].rxWasReaded = true;
			}

			else if( response ==  RESPONSE_UNKNOWN)
			{
				if(debugOn)
				{
					snprintf (debugData, sizeof(debugData), "ESP: Nieznana odpowiedz: '%s' \r\n", BufferTable[i].rxBuffer);
					Debug(debugData);
				}

				memset((void *)BufferTable[i].rxBuffer, 0, RX_BUFFER_SIZE);
				BufferTable[i].rxWasReaded = true;
			}

			if(expected == response)
			{
//				snprintf (debugData, sizeof(debugData), "Match: %d expected: %s response: %s\r\n", i,  responseNames[*expected], responseNames[response]);
//				Debug(debugData);
				memset((void *)BufferTable[i].rxBuffer, 0, RX_BUFFER_SIZE);
				BufferTable[i].rxWasReaded = true;
				if(intFifoRespSize < respTableSize)
				{
					acceptableResponses[intFifoRespSize] = response;
					intFifoRespSize++;
				}
				return response;
			}
			else
			{
				memset((void *)BufferTable[i].rxBuffer, 0, RX_BUFFER_SIZE);
				BufferTable[i].rxWasReaded = true;
				if(intFifoRespSize < respTableSize)
				{
					acceptableResponses[intFifoRespSize] = response;
					intFifoRespSize++;
				}
				else
					return OVERLOAD;
			}

		}
	}
	return response;
}



bool GetTimeFromNTP()
{
    // Ustawienia połączenia
	ResponseType isRespondOK = RESPONSE_UNKNOWN;
	ResponseType isIPDRespondOK = RESPONSE_UNKNOWN;
	ResponseType isRespondConnect = RESPONSE_UNKNOWN;
	bool changeServerPool = false;
	int TimeoutCount = 2;

	for(int respondTimeout =0; (respondTimeout < TimeoutCount) && ESP8266_isConnected; respondTimeout++)
	{
	//	Debug("Creating NTP connection\r\n");
		int server = NTPpoolServer;

		snprintf (txBuffer, sizeof(txBuffer), "AT+CIPSTART=\"UDP\",\"%d.pl.pool.ntp.org\",123\r\n", server);
	//	snprintf (txBuffer, sizeof(txBuffer), "AT+CIPSTART=\"UDP\",\"tempus%d.gum.gov.pl\",123\r\n", server);
		SendToESP(txBuffer);

		isRespondConnect = IsRespondCorrect(RESPONSE_CONNECT,10000);

		if(RESPONSE_CONNECT == isRespondConnect || isIncluded(RESPONSE_ALREADY_CONNECTED))// || RESPONSE_SEND_OK == isRespondOK)
		{

			if(debugOn)
			{
				snprintf (txBuffer, sizeof(txBuffer), "Otwarcie polaczenia do: %d.pl.pool.ntp.org \r\n", server);
	//    	snprintf (txBuffer, sizeof(txBuffer), "Otwarcie polaczenia do: tempus%d.gum.gov.pl \r\n", server);
				Debug(txBuffer);
			}
	//		uint8_t ntpRequest[48] = {0};
	//		ntpRequest[0] = 0x1B;  // Ustawienia protokołu NTP
	//		snprintf (txBuffer, sizeof(txBuffer), "AT+CIPSEND=%d\r\n", sizeof(ntpRequest));
			// Wyślij rzeczywisty pakiet NTP
			//HAL_UART_Transmit_IT(ESP_USART, ntpRequest, sizeof(ntpRequest));

			// Wysłanie zapytania NTP

			NTPResponsLength = 55;
			WaitForNTPResp = true;

			snprintf (txBuffer, sizeof(txBuffer), "AT+CIPSENDEX=48\r\n");
			SendToESP(txBuffer);
	//		DelayUS(50000);
			isRespondOK = IsRespondCorrect(RESPONSE_OK,5000);
			if(RESPONSE_OK == isRespondOK)
			{
	//			isRespondOK = IsRespondCorrect(ESP_WAIT_FOR_DATA,60000);
	//			if(ESP_WAIT_FOR_DATA == isRespondOK)
					snprintf (txBuffer, sizeof(txBuffer), "1B00000000000000000000000000000000000000000000000000000000000000000000000000000000");
					SendToESP(txBuffer);

					isIPDRespondOK = IsRespondCorrect(RESPONSE_IPD,5000);
					if(isIPDRespondOK != RESPONSE_IPD)
					{
						if(debugOn)
						{
							snprintf (debugData, sizeof(debugData),"Wait for IPD data fail, return: %s\r\n", responseNames[isIPDRespondOK]);
							Debug(debugData);
						}

						if(isIPDRespondOK == RESPONSE_TIMEOUT || isIncluded(RESPONSE_IPD_FAIL))
							changeServerPool = true;
					}

					WaitForNTPResp = false;
					StartCollectingNTPRespons = false;


			}
			else
			{
				if(debugOn)
				{
					snprintf (debugData, sizeof(debugData),"Problem wyslaniem AT+CIPSEND=48, return: %s\r\n", responseNames[isRespondOK]);
					Debug(debugData);
				}
			}


		}
		else
		{
			changeServerPool = true;
		}

		if(changeServerPool == true)
		{
			if(debugOn)
			{
				snprintf (debugData, sizeof(debugData), "Problem z polaczeniem do %d.pl.pool.ntp.org, connection: %s return: %s\r\n", server, responseNames[isRespondConnect], responseNames[isRespondOK]);
				Debug(debugData);
			}

			if(NTPpoolServer < maxNTPPoolServers)
				NTPpoolServer++;
			else
				NTPpoolServer = 0;
		}



		WaitForNTPResp = false;
		StartCollectingNTPRespons = false;

		SendToESP("AT+CIPCLOSE\r\n"); // wysylane w przypadku gdy nie otrzyma response
		isRespondOK = IsRespondCorrect(RESPONSE_OK,2000);
		if(debugOn)
		{
			snprintf (debugData, sizeof(debugData), "Zakonczenie polaczenia: %d.pl.pool.ntp.org\r\n", server);
			Debug(debugData);
		}

		if(isIPDRespondOK == RESPONSE_IPD)
			break;
	}

	if(isIPDRespondOK == RESPONSE_IPD)
		return true;

	else
		return false;
}

//void GetTimeFromHTTP(void)
//{
//    ResponseType isRespondOK;
//
//    // Otwarcie połączenia TCP
//    Debug("Laczenie z serwerem timeapi.org ...\r\n");
////    SendToESP("AT+CIPSTART=\"TCP\",\"timeapi.io\",80\r\n");
//    SendToESP("AT+CIPSTART=\"SSL\",\"timeapi.io\",443\r\n");
//
//    isRespondOK = IsRespondCorrect(RESPONSE_CONNECT, 5000);
//    if (isRespondOK == RESPONSE_CONNECT)
//    {
////    	if(debugOn)
////    		Debug("Otwarto polaczenie z serwerem.\r\n");
//
////         Przygotowanie zapytania HTTP GET
////        const char *httpRequest = "GET /api/TimeZone/Europe/Warsaw HTTP/1.1\r\n"
////        	    					"Host: timeapi.io\r\n"
////        	    					"Connection: close\r\n\r\n";
//        const char *httpRequest =
//            "GET /api/TimeZone/Europe/Warsaw HTTP/1.1\r\n"
//            "Host: timeapi.io\r\n"
//            "Connection: close\r\n\r\n";
//
//		NTPResponsLength = 343;
//		WaitForNTPResp = true;
////		DelayUS(50000);
//        char command[50];
//        snprintf (command, sizeof(command), "AT+CIPSENDEX=%d\r\n", strlen(httpRequest));
////        snprintf (command, sizeof(command), "AT+CIPSEND=87\r\n");
//        SendToESP(command);
//		DelayUS(5000);
//
//
//        isRespondOK = IsRespondCorrect(RESPONSE_OK, 10000);
//        if (isRespondOK == RESPONSE_OK)
//        {
//			if(debugOn)
//				Debug("Wyslano dane do serwera HTTP.\r\n");
//
////        	isRespondOK = IsRespondCorrect(ESP_WAIT_FOR_DATA,60000);
////			if(ESP_WAIT_FOR_DATA == isRespondOK)
////			{
//
//
//				// Wysłanie żądania HTTP
//				SendToESP(httpRequest);
//				if(debugOn)
//					Debug("Wyslano zapytanie HTTP.\r\n");
//
//				// Oczekiwanie na odpowiedź
//				isRespondOK = IsRespondCorrect(RESPONSE_IPD, 15000);
//				if (isRespondOK == RESPONSE_IPD)
//				{
//					if(debugOn)
//						Debug("Odebrano odpowiedz z serwera HTTP.\r\n");
//				}
//				else
//				{
//					Debug("Blad podczas odbierania odpowiedzi HTTP.\r\n");
//				}
////			}
////			else
////			{
////				if(debugOn)
////				{
////			    //char debugData[100];
////			    snprintf (debugData, sizeof(debugData),"ESP Wait for data fail, return: %s\r\n", responseNames[isRespondOK]);
////				Debug(debugData);
////				}
////			}
////
//        }
//        else
//        {
//    		Debug("Blad podczas wysylania danych HTTP.\r\n");
//        }
//
//    	WaitForNTPResp = false;
//    	StartCollectingNTPRespons = false;
//    }
//    else
//    {
//		Debug("Blad podczas otwierania polaczenia z serwerem.\r\n");
//    }
//
//
//
//    // Zamknięcie połączenia
//    SendToESP("AT+CIPCLOSE\r\n");
//	if(debugOn)
//	{
//    	Debug("Polaczenie z serwerem zostalo zamkniete.\r\n");
//	}
//}
//
//void ProcessHTTPResponse(char *dateTimeStart)
//{
//    if (dateTimeStart) {
//        dateTimeStart += 11; // Pomijamy "datetime":"
//        char *dateTimeEnd = strstr(dateTimeStart, "\",");
//        if (dateTimeEnd) {
//            *dateTimeEnd = '\0'; // Zakończ string na końcu daty
//
//            if(debugOn)
//            {
//            	Debug("Odebrana data i godzina:\r\n");
//            	Debug(dateTimeStart);
//            }
//
//        	procesNTPTime = true;
//        	if(TimerInit == false)
//        	  if (HAL_TIM_Base_Start_IT(TIMERSEC) == HAL_OK)
//        	  {
//        		  TimerInit = true;
//        	  }
//
//            // Przetworzenie daty/godziny
//            sscanf(dateTimeStart, "%d-%d-%dT%d:%d:%d",
//                   &DateNow.year, &DateNow.month, &DateNow.day,
//                   &DateNow.hour, &DateNow.minute, &DateNow.second);
//
//            const char *dayOfWeek = GetDayOfWeek(DateNow.year, DateNow.month, DateNow.day);
//            strcpy((char *)DateNow.DayName, dayOfWeek);
//
//
//
//            char debugData[200];
//            snprintf (debugData, sizeof(debugData), "Przetworzono datę i czas: %04d-%02d-%02d %02d:%02d:%02d (%s)\r\n",
//                    DateNow.year, DateNow.month, DateNow.day,
//                    DateNow.hour, DateNow.minute, DateNow.second, dayOfWeek);
//            Debug(debugData);
//        }
//        else
//        {
//            Debug("Nie mozna znalezc konca pola datetime.\r\n");
//        }
//    }
//}
//
