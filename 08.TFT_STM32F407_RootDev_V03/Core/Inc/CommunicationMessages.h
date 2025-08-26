/*
 * CommunicationMessages.h
 *
 *  Created on: Feb 11, 2024
 *      Author: wawer
 */

#ifndef INC_COMMUNICATIONMESSAGES_H_
#define INC_COMMUNICATIONMESSAGES_H_
#include "main.h"
#include "cJSON.h"

#define LowVoltageWarningCount	4
#define LowTemperatureWarningCount	2

typedef enum
{
    Confirmation,
    Volt,
    Temperature,
	Humidity,
	Relay1Status,
	Relay2Status,
	Relay1Set,
	Relay2Set,
	Request,
	SilentVolt,
	SilentTemp,
	SilentHum,
	CarStatus,
	DeviceInformation
} MessageType;

typedef enum
{
    ReqVolt,
	ReqTemperature,
	ReqHumidity,
	ReqRelay1Status,
	ReqRelay2Status,
	Broadcast,
    SilentReqVolt,
	SilentReqTemperature,
	SilentReqHumidity,
	ReqCarStatus,
	ReqRunServoIgnition,
	ReqDeviceInformatin
} RequestFor;

typedef struct
{
	int DevID;
	MessageType mt;
	int MsgNR;
	float Volt;
	float Temp;
	float Hum;
	float Pressure;
	float Altitude;
	int Relay1Status;
	int Relay2Status;
	int EngineStatus;
} MessageDeviceInformatin;

typedef struct
{
	int DevID;
	int Connected;
	float Volt;
	float Temp;
	float Hum;
	int RelayStatus;
} Device;

typedef struct
{
	int DevID;
	MessageType mt;

} MessageConfirmation;

typedef struct
{
	int DevID;
	MessageType mt;
	float Voltage;

} MessageVoltage;

typedef struct
{
	int DevID;
	MessageType mt;
	float Temperature;

} MessageTemperature;

typedef struct
{
	int DevID;
	MessageType mt;
	float Humidity;

} MessageHumidity;

typedef struct
{
	int DevID;
	MessageType mt;
	int RelayStatus; //0 off 1 on

} MessageRelayStatus;

typedef struct
{
	int DevID;
	MessageType mt;
	int RelaySet; //0 off 1 on

} MessageRelaySet;

typedef struct
{
	int DevID;
	MessageType mt;
	RequestFor request;

} MessageRequest;

typedef struct
{
	int DevID;
	MessageType mt;

} MessageBroadcast;

typedef struct
{
	int DevID;
	MessageType mt;
	int CarStatus; // 0 off 1 on

} MessageCarStatus;

int  LoraReset(LoRa *dev);
void MakeHandleToLoRa(LoRa *dev);
void SetDevID(int devId);
//void MakeHandleToDisplayBacklight(BacklightStatus *backlight);
void LoRaReceiveMode();

int SendMessageConfirmation();
int SendMessageDeviceInformation(MessageType mt, int MsgNR, float Volt, float Temp, float Hum, float Pressure, float Altitude, int Relay1Status, int Relay2Status, int EngineStatus);
int SendMessageMsVoltage(float Voltage, MessageType mt);
int SendMessageTemperature(float Temp, MessageType mt);
int SendMessageHumidity(float Hum, MessageType mt);
int SendMessageRelayStatus(int devId, int DevRel, int relayStatus);
int SendMessageSetRelay(int devId, int DevRel, int relaySet);
int SendMessageRequest(int devID, RequestFor req);
int SendMessageCarStatus(int Status);

int PackMessageConfirmation(MessageConfirmation *msg, char json_string[]);
int PackMessageDeviceInformation(MessageDeviceInformatin *msg, char json_string[]);
int PackMessageVoltage(MessageVoltage *msg, char json_string[]);
int PackMessageTemperature(MessageTemperature *msg, char json_string[]);
int PackMessageHumidity(MessageHumidity *msg, char json_string[]);
int PackMessageRelayStatus(MessageRelayStatus *msg, char json_string[]);
int PackMessageRelaySet(MessageRelaySet *msg, char json_string[]);
int PackMessageRequest(MessageRequest *msg, char json_string[]);
int PackMessageCarStatus(MessageCarStatus *msg, char json_string[]);



int UnpackMessage(char *json_string);
int MessageRequestAction(MessageRequest Request);

MessageConfirmation UnpackMessageConfirmation(cJSON *root);
MessageDeviceInformatin UnpackMessageDeviceInformation(cJSON *root);
MessageVoltage UnpackMessageVoltage(cJSON *root);
MessageVoltage UnpackMessageSilentVoltage(cJSON *root);
MessageTemperature UnpackMessageTemperature(cJSON *root);
MessageTemperature UnpackMessageSilentTemperature(cJSON *root);
MessageHumidity UnpackMessageHumidity(cJSON *root);
MessageHumidity UnpackMessageSilentHumidity(cJSON *root);
MessageRelayStatus UnpackMessageRelayStatus(cJSON *root);
MessageRelaySet UnpackMessageRelaySet(cJSON *root);
MessageRequest UnpackMessageRequest(cJSON *root);
MessageCarStatus UnpackMessageCarStatus(cJSON *root);


#endif /* INC_COMMUNICATIONMESSAGES_H_ */
