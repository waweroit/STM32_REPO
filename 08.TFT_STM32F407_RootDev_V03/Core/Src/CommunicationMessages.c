/*
 * CommunicationMessages.c
 *
 *  Created on: Feb 11, 2024
 *      Author: wawer
 */

#include <cJSON.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LoRa.h"
#include "waweroBasicFunction.h"
#include "CommunicationMessages.h"

LoRa *LoraDev;
int DevId;// my device ID

MessageDeviceInformatin MsDeviceInformation;
MessageConfirmation MsConfirmation;
MessageVoltage 		MsVoltage;
MessageTemperature 	MsTemp;
MessageHumidity 	MsHum;
MessageRelayStatus 	MsRelayStatus;
MessageRelaySet 	MsRelaySet;
MessageCarStatus 	MsCarStatus;

MessageRequest MsRequest;
MessageRequest MsBroadcast;

Device ConnectedDevices[10];

int LowVoltCount = LowVoltageWarningCount;
int VeryLowVoltCount = LowVoltageWarningCount;
int LowTempCount =LowTemperatureWarningCount;

int LoraReset(LoRa *dev)
{
	LoraDev = dev;
	return InitializeMyLoRa(LoraDev);
}

void MakeHandleToLoRa(LoRa *dev)
{
	LoraDev = dev;
}

void SetDevID(int devId)
{
	DevId = devId;
}


void LoRaReceiveMode()
{
	// START CONTINUOUS RECEIVING -----------------------------------
	LoRa_startReceiving(LoraDev);
}

int UnpackMessage(char *json_string)
{
	int result =0;
    cJSON *root = cJSON_Parse(json_string);
    if (root != NULL)
    {
		cJSON *mt = cJSON_GetObjectItem(root, "mt");
		if (mt != NULL || !cJSON_IsNumber(mt))
		{
			MessageType messageType = (MessageType)(mt->valueint);

				switch (messageType)
				{
					case Confirmation:
						MsConfirmation = UnpackMessageConfirmation(root);
						result=1;
						break;

					case DeviceInformation:
						MsDeviceInformation = UnpackMessageDeviceInformation(root);
						result=1;
						break;

					case Volt:
						MsVoltage = UnpackMessageVoltage(root);
						//PrintVoltageHD44780(MsVoltage.DevID, MsVoltage.Voltage);
						result=1;
						break;

					case Temperature:
						MsTemp = UnpackMessageTemperature(root);
						//PrintTempHD44780(MsTemp.DevID, MsTemp.Temperature);
						result=1;
						break;

					case Humidity:
						MsHum = UnpackMessageHumidity(root);
						//PrintHumHD44780(MsHum.DevID, MsHum.Humidity);
						result=1;
						break;

					case SilentVolt:
						MsVoltage = UnpackMessageVoltage(root);

						//PrintVoltageHD44780(MsVoltage.DevID, MsVoltage.Voltage);

						if(MsVoltage.Voltage > 11.2)
						{
							LowVoltCount = LowVoltageWarningCount;

						}
						else if(MsVoltage.Voltage >= 10.9 && MsVoltage.Voltage < 11.0)
						{

							if(LowVoltCount > 0)
							{
//								WarningBeep(4,500);
								LowVoltCount--;
							}
						}
						else if(MsVoltage.Voltage >= 10.3 && MsVoltage.Voltage < 10.5)
						{

							if(VeryLowVoltCount > 0)
							{
//								WarningBeep(6,500);

								VeryLowVoltCount--;
							}
						}
						else if(MsVoltage.Voltage > 0.0 && MsVoltage.Voltage < 10.3)
						{
							SendMessageSetRelay(MsVoltage.DevID, 2,  0); // wylacz grzanie
							DelayUS(1000*1000);
							SendMessageSetRelay(MsVoltage.DevID, 1,  1); // wlacz ladowanie
							if(VeryLowVoltCount > 0)
							{
//								WarningBeep(6,500);
								VeryLowVoltCount--;
							}

						}

						result=1;
						break;

					case SilentTemp:
						MsTemp = UnpackMessageTemperature(root);
						if(MsTemp.Temperature > 4)
						{
							LowTempCount = LowTemperatureWarningCount;
						}
						else if(MsTemp.Temperature <= 3.4)
						{

							if(LowTempCount > 0)
							{
//								WarningBeep(4,500);
								LowTempCount--;
							}
						}

						result=1;
						break;

					case SilentHum:
						MsHum = UnpackMessageHumidity(root);
						//PrintHumHD44780(MsHum.DevID, MsHum.Humidity);
						result=1;
						break;

					case Relay1Status:
						MsRelayStatus = UnpackMessageRelayStatus(root);
//						SetRelayStatus(&MsRelayStatus);
						result=1;
						break;

					case Relay2Status:
						MsRelayStatus = UnpackMessageRelayStatus(root);
//						SetRelayStatus(&MsRelayStatus);
						result=1;
						break;

					case Relay1Set:
						MsRelaySet = UnpackMessageRelaySet(root);
						result=1;
						break;

					case Relay2Set:
						MsRelaySet = UnpackMessageRelaySet(root);
						result=1;
						break;

					case CarStatus:
						MsCarStatus = UnpackMessageCarStatus(root);
//						PrintCarStatusHD44780(MsCarStatus.DevID, MsCarStatus.CarStatus);
						result=1;
						break;

					case Request:
						MsRequest = UnpackMessageRequest(root);
						result= MessageRequestAction(MsRequest);
						break;

					// Dodaj inne przypadki tutaj
					default:
					{
						// Obsługa nieznanego typu wiadomości
						result=55;
					}
					break;
				}
		}
		else
		{
			// Obsługa braku lub błędnego pola "mt"
			result=98;
		}
		//cJSON_DeleteWawero(root);
	    cJSON_Delete(root); // Zwolnienie pamięci zaalokowanej przez cJSON
    }
    else
    {
    	// Obsługa błędu parsowania JSON
		result=99;
    }

    return result;
}



int MessageRequestAction(MessageRequest Request)
{
	int result =0;
	//float temp, hum;

	if(Request.DevID == DevId)
	{
		switch (Request.request)
		{
			case ReqDeviceInformatin:
				//result = SendMessageDeviceInformation(Dev, MsgNR, float Volt, float Temp, float Hum, float Pressure, float Altitude, int Relay1Status, int Relay2Status, int EngineStatus);
				break;

			case ReqVolt:

				//result = SendMessageMsVoltage(ReadVoltageFromADC(3.3f));
				break;

			case ReqTemperature:
//				if(CheckConnectionToSHT31Sensor())
//				{
//					if(ReadSHT31SensorTempHum(&temp, &hum))
//						result = SendMessageTemperature(temp, Temperature);
//				}
				break;

			case ReqHumidity:
//				if(CheckConnectionToSHT31Sensor())
//				{
//					if(ReadSHT31SensorTempHum(&temp, &hum))
//						result = SendMessageHumidity(hum, Humidity);
//				}
				break;

			case ReqRelay1Status:
//				SendMessageRelayStatus(DevId, 1, 2);
				result=1;
				break;

			case ReqRelay2Status:
//				SendMessageRelayStatus(DevId, 2, 2);
				result=1;
				break;

//			case Broadcast:
//				SendMessageConfirmation();
//				result=1;
//				break;

			// Dodaj inne przypadki tutaj
			default:
				// Obsługa nieznanego typu wiadomości
				result=55;
				break;
		}
	}
	else if(Request.request == Broadcast)
	{
		result = SendMessageConfirmation();
	}

	if(result == 0)
	{
		if(LoraDev->current_mode != RXCONTIN_MODE)
		{
//			PrintLoraStatus(LoraReset(LoraDev));
			LoRaReceiveMode();
			result=1;
		}
		result=1;
	}
	else if(result == 1)
	{
		if(LoraDev->current_mode != RXCONTIN_MODE)
		{
			LoRaReceiveMode();
		}
	}
	return result;
}


int SendMessageConfirmation()
{
	MsConfirmation.DevID = DevId;
	MsConfirmation.mt = Confirmation;

	char TxBuffer[128];
	int length = PackMessageConfirmation(&MsConfirmation, (char*)TxBuffer);

//	int isSuccess = LoRa_transmit(LoraDev, (uint8_t*)TxBuffer, length, 500);
	int isSuccess = LoRa_transmit(LoraDev, (uint8_t*)TxBuffer, length, 500);
	memset(TxBuffer, 0, sizeof TxBuffer);
	return isSuccess;
}

int SendMessageDeviceInformation(MessageType mt, int MsgNR, float Volt, float Temp, float Hum, float Pressure, float Altitude, int Relay1Status, int Relay2Status, int EngineStatus)
{
	MsDeviceInformation.DevID = DevId;
	MsDeviceInformation.mt = mt;
	MsDeviceInformation.MsgNR = MsgNR;
	MsDeviceInformation.Volt = Volt;
	MsDeviceInformation.Temp = Temp;
	MsDeviceInformation.Hum = Hum;
	MsDeviceInformation.Pressure = Pressure;
	MsDeviceInformation.Altitude = Altitude;
	MsDeviceInformation.Relay1Status = Relay1Status;
	MsDeviceInformation.Relay2Status = Relay2Status;
	MsDeviceInformation.EngineStatus = EngineStatus;

	char TxBuffer[128];
	int length = PackMessageDeviceInformation(&MsDeviceInformation, (char*)TxBuffer);

//	int isSuccess = LoRa_transmit(LoraDev, (uint8_t*)TxBuffer, length, 500);
	int isSuccess = LoRa_transmit(LoraDev, (uint8_t*)TxBuffer, length, 500);
	memset(TxBuffer, 0, sizeof TxBuffer);
	return isSuccess;
}

int SendMessageMsVoltage(float Voltage, MessageType mt)
{
	MsVoltage.DevID = DevId;
	MsVoltage.mt = mt;
	MsVoltage.Voltage = Voltage;

	char TxBuffer[128];
	int length = PackMessageVoltage(&MsVoltage, (char*)TxBuffer);

	int isSuccess = LoRa_transmit(LoraDev, (uint8_t*)TxBuffer, length, 500);
	memset(TxBuffer, 0, sizeof TxBuffer);
	return isSuccess;
}

int SendMessageTemperature(float Temp, MessageType mt)
{
	MsTemp.DevID = DevId;
	MsTemp.mt = mt;
	MsTemp.Temperature = Temp;

	char TxBuffer[128];
	int length = PackMessageTemperature(&MsTemp, (char*)TxBuffer);

	int isSuccess = LoRa_transmit(LoraDev, (uint8_t*)TxBuffer, length, 500);
	memset(TxBuffer, 0, sizeof TxBuffer);
	return isSuccess;
}

int SendMessageHumidity(float Hum, MessageType mt)
{
	MsHum.DevID = DevId;
	MsHum.mt = mt;
	MsHum.Humidity = Hum;

	char TxBuffer[128];
	int length = PackMessageHumidity(&MsHum, (char*)TxBuffer);

	int isSuccess = LoRa_transmit(LoraDev, (uint8_t*)TxBuffer, length, 500);
	memset(TxBuffer, 0, sizeof TxBuffer);
	return isSuccess;
}

int SendMessageRelayStatus(int devId, int DevRel, int relayStatus)
{
	MsRelayStatus.DevID = devId;
	if(DevRel == 1)
		MsRelayStatus.mt = Relay1Status;
	else if(DevRel == 2)
		MsRelayStatus.mt = Relay2Status;
	MsRelayStatus.RelayStatus = relayStatus;
	char TxBuffer[128];
	int length = PackMessageRelayStatus(&MsRelayStatus, (char*)TxBuffer);

	int isSuccess = LoRa_transmit(LoraDev, (uint8_t*)TxBuffer, length, 500);
	memset(TxBuffer, 0, sizeof TxBuffer);
	return isSuccess;
}

int SendMessageSetRelay(int devId, int DevRel, int relaySet)
{
	MsRelaySet.DevID = devId;
	if(DevRel == 1)
		MsRelaySet.mt = Relay1Set;
	else if(DevRel == 2)
		MsRelaySet.mt = Relay2Set;
	MsRelaySet.RelaySet = relaySet;
	char TxBuffer[128];
	int length = PackMessageRelaySet(&MsRelaySet, (char*)TxBuffer);

	int isSuccess = LoRa_transmit(LoraDev, (uint8_t*)TxBuffer, length, 500);
	memset(TxBuffer, 0, sizeof TxBuffer);
	return isSuccess;
}

int SendMessageRequest(int devID, RequestFor req)
{
	MsRequest.DevID = devID;
	MsRequest.mt = Request;
	MsRequest.request = req;

	char TxBuffer[128];
	int length = PackMessageRequest(&MsRequest, (char*)TxBuffer);

	int isSuccess = LoRa_transmit(LoraDev, (uint8_t*)TxBuffer, length, 500);

	memset(TxBuffer, 0, sizeof TxBuffer);
	return isSuccess;
}

int SendMessageCarStatus(int Status)
{
	MsCarStatus.DevID = DevId;
	MsCarStatus.mt = CarStatus;
	MsCarStatus.CarStatus = Status;

	char TxBuffer[128];
	int length = PackMessageCarStatus(&MsCarStatus, (char*)TxBuffer);

	int isSuccess = LoRa_transmit(LoraDev, (uint8_t *)TxBuffer, length, 500);
	memset(TxBuffer, 0, sizeof TxBuffer);
	return isSuccess;
}

// Funkcja do pakowania struktury MessageConfirmation do JSON
int PackMessageConfirmation(MessageConfirmation *msg, char json_string[])
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "DevID", msg->DevID);
    cJSON_AddNumberToObject(root, "mt", msg->mt);
    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    //    size_t length = strlen(json);
    //    strcpy(json_string, json);

        int length = sprintf(json_string, json);

        free(json);
    return length;
}


int PackMessageDeviceInformation(MessageDeviceInformatin *msg, char json_string[])
{
    cJSON *root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "DevID", msg->DevID);
    cJSON_AddNumberToObject(root, "mt", msg->mt);
    cJSON_AddNumberToObject(root, "MsgNR", msg->MsgNR);
    cJSON_AddNumberToObject(root, "Volt", msg->Volt);
    cJSON_AddNumberToObject(root, "Temp", msg->Temp);
    cJSON_AddNumberToObject(root, "Hum", msg->Hum);
    cJSON_AddNumberToObject(root, "Pres", msg->Pressure);
    cJSON_AddNumberToObject(root, "Alti", msg->Altitude);
    cJSON_AddNumberToObject(root, "Rel1S", msg->Relay1Status);
    cJSON_AddNumberToObject(root, "Rel2S", msg->Relay2Status);
    cJSON_AddNumberToObject(root, "EngS", msg->EngineStatus);

    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    //    size_t length = strlen(json);
    //    strcpy(json_string, json);

        int length = sprintf(json_string, json);

        free(json);
    return length;
}

// Funkcja do pakowania struktury MessageVoltage do JSON
int PackMessageVoltage(MessageVoltage *msg, char json_string[])
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "DevID", msg->DevID);
    cJSON_AddNumberToObject(root, "mt", msg->mt);
    cJSON_AddNumberToObject(root, "Voltage", msg->Voltage);
    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    //    size_t length = strlen(json);
    //    strcpy(json_string, json);

        int length = sprintf(json_string, json);

        free(json);
    return length;
}
// Funkcja do pakowania struktury MessageTemperature do JSON
int PackMessageTemperature(MessageTemperature *msg, char json_string[])
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "DevID", msg->DevID);
    cJSON_AddNumberToObject(root, "mt", msg->mt);
    cJSON_AddNumberToObject(root, "Temperature", msg->Temperature);
    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    //    size_t length = strlen(json);
    //    strcpy(json_string, json);

        int length = sprintf(json_string, json);

        free(json);
    return length;
}

// Funkcja do pakowania struktury MessageHumidity do JSON
int PackMessageHumidity(MessageHumidity *msg, char json_string[])
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "DevID", msg->DevID);
    cJSON_AddNumberToObject(root, "mt", msg->mt);
    cJSON_AddNumberToObject(root, "Humidity", msg->Humidity);
    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    //    size_t length = strlen(json);
    //    strcpy(json_string, json);

        int length = sprintf(json_string, json);

        free(json);
    return length;
}

// Funkcja do pakowania struktury MessageRelayStatus do JSON
int PackMessageRelayStatus(MessageRelayStatus *msg, char json_string[])
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "DevID", msg->DevID);
    cJSON_AddNumberToObject(root, "mt", msg->mt);
    cJSON_AddNumberToObject(root, "RelayStatus", msg->RelayStatus);
    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    //    size_t length = strlen(json);
    //    strcpy(json_string, json);

        int length = sprintf(json_string, json);

        free(json);
    return length;
}

// Funkcja do pakowania struktury MessageRelaySet do JSON
int PackMessageRelaySet(MessageRelaySet *msg, char json_string[])
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "DevID", msg->DevID);
    cJSON_AddNumberToObject(root, "mt", msg->mt);
    cJSON_AddNumberToObject(root, "RelaySet", msg->RelaySet);
    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    //    size_t length = strlen(json);
    //    strcpy(json_string, json);

        int length = sprintf(json_string, json);

        free(json);
    return length;
}

// Funkcja do pakowania struktury MessageRequest do JSON
int PackMessageRequest(MessageRequest *msg, char json_string[])
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "DevID", msg->DevID);
    cJSON_AddNumberToObject(root, "mt", msg->mt);
    cJSON_AddNumberToObject(root, "RequestFor", msg->request);
    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

//    size_t length = strlen(json);
//    strcpy(json_string, json);

    int length = sprintf(json_string, json);

    free(json);
    return length;
}

// Funkcja do pakowania struktury MessageCarStatus do JSON
int PackMessageCarStatus(MessageCarStatus *msg, char json_string[])
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "DevID", msg->DevID);
    cJSON_AddNumberToObject(root, "mt", msg->mt);
    cJSON_AddNumberToObject(root, "CarStatus", msg->CarStatus);
    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    //    size_t length = strlen(json);
    //    strcpy(json_string, json);

        int length = sprintf(json_string, json);

        free(json);
    return length;
}

//====================================================================
MessageConfirmation UnpackMessageConfirmation(cJSON *root)
{
    MessageConfirmation msg_confirmation = {0};
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        if (devID != NULL && mt != NULL) {
            msg_confirmation.DevID = devID->valueint;
            msg_confirmation.mt = mt->valueint;
        }
    }
    return msg_confirmation;
}

MessageDeviceInformatin UnpackMessageDeviceInformation(cJSON *root)
{
	MessageDeviceInformatin msg_DeviceInformatin = {0};
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        cJSON *msgNR = cJSON_GetObjectItem(root, "MsgNR");
        cJSON *volt = cJSON_GetObjectItem(root, "Volt");
        cJSON *temp = cJSON_GetObjectItem(root, "Temp");
        cJSON *hum = cJSON_GetObjectItem(root, "Hum");
        cJSON *pres = cJSON_GetObjectItem(root, "Pres");
        cJSON *alti = cJSON_GetObjectItem(root, "Alti");
        cJSON *rel1S = cJSON_GetObjectItem(root, "Rel1S");
        cJSON *rel2S = cJSON_GetObjectItem(root, "Rel2S");
        cJSON *engS = cJSON_GetObjectItem(root, "EngS"); // engine status

        if (devID != NULL && mt != NULL) {
        	msg_DeviceInformatin.DevID = devID->valueint;
        	msg_DeviceInformatin.mt = mt->valueint;
        	msg_DeviceInformatin.MsgNR = msgNR->valueint;
        	msg_DeviceInformatin.Volt = (float)volt->valuedouble;
        	msg_DeviceInformatin.Temp = (float)temp->valuedouble;
        	msg_DeviceInformatin.Hum = (float)hum->valuedouble;
        	msg_DeviceInformatin.Pressure = (float)pres->valuedouble;
        	msg_DeviceInformatin.Altitude = (float)alti->valuedouble;
        	msg_DeviceInformatin.Relay1Status = rel1S->valueint;
        	msg_DeviceInformatin.Relay2Status = rel2S->valueint;
        	msg_DeviceInformatin.EngineStatus = engS->valueint;
        }
    }
    return msg_DeviceInformatin;
}

MessageVoltage UnpackMessageVoltage(cJSON *root)
{
    MessageVoltage msg_voltage = {0}; // Inicjalizacja zerami, można też użyć memset
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        cJSON *voltage = cJSON_GetObjectItem(root, "Voltage");
        if (devID != NULL && mt != NULL && voltage != NULL) {
            msg_voltage.DevID = devID->valueint;
            msg_voltage.mt = mt->valueint;
            msg_voltage.Voltage = (float)voltage->valuedouble;
        }
    }
    return msg_voltage;
}

MessageVoltage UnpackMessageSilentVoltage(cJSON *root)
{
    MessageVoltage msg_voltage = {0}; // Inicjalizacja zerami, można też użyć memset
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        cJSON *voltage = cJSON_GetObjectItem(root, "SilentVolt");
        if (devID != NULL && mt != NULL && voltage != NULL) {
            msg_voltage.DevID = devID->valueint;
            msg_voltage.mt = mt->valueint;
            msg_voltage.Voltage = (float)voltage->valuedouble;
        }
    }
    return msg_voltage;
}

MessageTemperature UnpackMessageTemperature(cJSON *root)
{
    MessageTemperature msg_temperature = {0};
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        cJSON *temperature = cJSON_GetObjectItem(root, "Temperature");
        if (devID != NULL && mt != NULL && temperature != NULL) {
            msg_temperature.DevID = devID->valueint;
            msg_temperature.mt = mt->valueint;
            msg_temperature.Temperature = (float)temperature->valuedouble;
        }
    }
    return msg_temperature;
}

MessageTemperature UnpackMessageSilentTemperature(cJSON *root)
{
    MessageTemperature msg_temperature = {0};
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        cJSON *temperature = cJSON_GetObjectItem(root, "SilentTemp");
        if (devID != NULL && mt != NULL && temperature != NULL) {
            msg_temperature.DevID = devID->valueint;
            msg_temperature.mt = mt->valueint;
            msg_temperature.Temperature = (float)temperature->valuedouble;
        }
    }
    return msg_temperature;
}

MessageHumidity UnpackMessageHumidity(cJSON *root)
{
    MessageHumidity msg_humidity = {0};
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        cJSON *humidity = cJSON_GetObjectItem(root, "Humidity");
        if (devID != NULL && mt != NULL && humidity != NULL) {
            msg_humidity.DevID = devID->valueint;
            msg_humidity.mt = mt->valueint;
            msg_humidity.Humidity = (float)humidity->valuedouble;
        }
    }
    return msg_humidity;
}
MessageHumidity UnpackMessageSilentHumidity(cJSON *root)
{
    MessageHumidity msg_humidity = {0};
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        cJSON *humidity = cJSON_GetObjectItem(root, "SilentHum");
        if (devID != NULL && mt != NULL && humidity != NULL) {
            msg_humidity.DevID = devID->valueint;
            msg_humidity.mt = mt->valueint;
            msg_humidity.Humidity = (float)humidity->valuedouble;
        }
    }
    return msg_humidity;
}

MessageRelayStatus UnpackMessageRelayStatus(cJSON *root)
{
    MessageRelayStatus msg_relay_status = {0};
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        cJSON *relayStatus = cJSON_GetObjectItem(root, "RelayStatus");
        if (devID != NULL && mt != NULL && relayStatus != NULL) {
            msg_relay_status.DevID = devID->valueint;
            msg_relay_status.mt = mt->valueint;
            msg_relay_status.RelayStatus = relayStatus->valueint;
        }
    }
    return msg_relay_status;
}


MessageRelaySet UnpackMessageRelaySet(cJSON *root)
{
    MessageRelaySet msg_relay_set = {0};
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        cJSON *relaySet = cJSON_GetObjectItem(root, "RelaySet");
        if (devID != NULL && mt != NULL && relaySet != NULL) {
            msg_relay_set.DevID = devID->valueint;
            msg_relay_set.mt = mt->valueint;
            msg_relay_set.RelaySet = relaySet->valueint;
        }
    }
    return msg_relay_set;
}

// Funkcja do rozpakowania ciągu znaków JSON do struktury MessageRequest
MessageRequest UnpackMessageRequest(cJSON *root)
{
    MessageRequest msg_request = {0}; // Inicjalizacja struktury zerami
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        cJSON *request = cJSON_GetObjectItem(root, "RequestFor");
        if (devID != NULL && mt != NULL && request != NULL) {
            msg_request.DevID = devID->valueint;
            msg_request.mt = mt->valueint;
            msg_request.request = request->valueint;
        }
    }
    return msg_request;
}

MessageCarStatus UnpackMessageCarStatus(cJSON *root)
{
	MessageCarStatus msg_CarStaus = {0};
    if (root != NULL) {
        cJSON *devID = cJSON_GetObjectItem(root, "DevID");
        cJSON *mt = cJSON_GetObjectItem(root, "mt");
        cJSON *carstatus = cJSON_GetObjectItem(root, "CarStatus");
        if (devID != NULL && mt != NULL && carstatus != NULL) {
        	msg_CarStaus.DevID = devID->valueint;
        	msg_CarStaus.mt = mt->valueint;
        	msg_CarStaus.CarStatus = (int)carstatus->valueint;
        }
    }
    return msg_CarStaus;
}
