#ifndef __ESP8266_H
#define __ESP8266_H

#include "main.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"
#include "gpio.h"
#include "DHT11.h"

#define AT_MODE2 		"AT+CWMODE=2\r\n"
#define AT_RST   		"AT+RST\r\n"
#define AT_CIPMUX 	"AT+CIPMUX=1\r\n"
#define AT_CIPSERVER "AT+CIPSERVER=1\r\n"


extern bool device_connect;
extern bool hand_shack;

void handle_esp8266(void);
bool start_esp8266(void);
void send_wifi(uint8_t *data,int size);
#endif