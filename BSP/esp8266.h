#ifndef __ESP8266_H
#define __ESP8266_H

#include "main.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"
#include "gpio.h"
#include "DHT11.h"

#define AT_MODE2 		"AT+CWMODE=2\r\n"   //13
#define AT_SET      "AT+CWSAP=\"ESP8266\",\"0123456789\",11,0\r\n"  //38
#define AT_RST   		"AT+RST\r\n"  //8
#define AT_CIPMUX 	"AT+CIPMUX=1\r\n"
#define AT_CIPSERVER "AT+CIPSERVER=1,5000\r\n"  //start



//#define CMGF "AT+CMGF=1\r\n";
//#define CMGS "+8613880785811\r\n";
//#define CM_MESSAGE  "eat medicine\r\n"

extern bool device_connect;
extern bool hand_shack;

void handle_esp8266(void);
bool start_esp8266(void);
void send_wifi(char *data,int size);
void uart3_send_message(void);
void uart3_send_messageCALL(void);
void uart3_send_messageCALL2(void);
#endif