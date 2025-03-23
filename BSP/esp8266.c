#include "esp8266.h"
#include "DHT11.h"
#include "tim.h"


bool device_connect = false;


static  void handle_wifi_data(void);
bool start_esp8266(void)
{ 
	
			HAL_UART_Transmit(&huart2,(uint8_t*)(AT_MODE2),13,0xffff);
			HAL_Delay(1000);
      HAL_UART_Transmit(&huart2,(uint8_t*)(AT_SET),38,0xffff);
				HAL_Delay(1000);
			
			HAL_UART_Transmit(&huart2,(uint8_t*)(AT_RST),8,0xffff);
			HAL_Delay(4000);
			
			HAL_UART_Transmit(&huart2,(uint8_t*)(AT_CIPMUX),13,0xffff);
			HAL_Delay(1000);
			
		  HAL_UART_Transmit(&huart2,(uint8_t*)(AT_CIPSERVER),21,0xffff);
			HAL_Delay(1000);
			
				__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);  
			HAL_UART_Receive_DMA(&huart2,uart2_rx,1000);   
			
			 return true;
			 
}

void handle_esp8266(void)
{
//	char *send = "hello\r\n";
	char *wifi_connect = "0,CONNECT";
	char *wifi_rec = "\r\n+IPD,";
	char *layon = "\r\n+IPD,0,5:layon";
	char *timeset2 = "\r\n+IPD,0,5:set02";
	char *timeset3 = "\r\n+IPD,0,5:set03";
	char *timeset4 = "\r\n+IPD,0,5:set04";
	char *timeset5 = "\r\n+IPD,0,5:set05";
	char *timeset6 = "\r\n+IPD,0,5:set06";
	char *timeset7 = "\r\n+IPD,0,5:set07";
	char *timeset8 = "\r\n+IPD,0,5:set08";
	char *timeset9 = "\r\n+IPD,0,5:set09";
	char *timeset10 = "\r\n+IPD,0,5:set10";
	if(rx2_end_flag)
	{
		 //printf("HANDLE %s\r\n",uart2_rx);
			rx2_end_flag = false;
		
		  if(memcmp(uart2_rx,wifi_connect,9)==0)  //wifi已连接
			{
						//printf("yes\r\n");
						device_connect=true;
			}
			
			if(memcmp(uart2_rx,layon,15)==0)
			{
				   HAL_GPIO_WritePin(GPIOB, LAY_Pin, GPIO_PIN_RESET);
			}
			
			
			if(memcmp(uart2_rx,timeset2,16)==0)
			{
				  time_set = 2;
			}
			
						if(memcmp(uart2_rx,timeset3,16)==0)
			{
				  time_set = 3;
			}
			
						if(memcmp(uart2_rx,timeset4,16)==0)
			{
				  time_set = 5;
			}
			
						if(memcmp(uart2_rx,timeset6,16)==0)
			{
				  time_set = 6;
			}
			
						if(memcmp(uart2_rx,timeset8,16)==0)
			{
				  time_set = 8;
			}
			
						if(memcmp(uart2_rx,timeset9,16)==0)
			{
				  time_set = 9;
			}
			
						if(memcmp(uart2_rx,timeset10,16)==0)
			{
				printf("yes\r\n");
				  time_set = 10;
			}

			rx2_count=0;
			memset(uart2_rx,0,UART2_SIZE);
			HAL_UART_Receive_DMA(&huart2,uart2_rx,UART2_SIZE);  //需要重新启动DMA
	} 
				
}

void send_wifi(char *data,int size)
{
	  int send_size=0;
	  if(size<10)
		{
			 send_size=1; 
		}
		
		if(size>10&&size<100)
		{
				send_size=2;
		}
		
			
	  if(device_connect)
		{
			char send_data[50]={0};
			char send_data1[50]={0};
			sprintf(send_data,"AT+CIPSEND=0,%d\r\n",size);
			HAL_UART_Transmit(&huart2,(uint8_t*)(send_data),15+send_size,0xffff);

			HAL_Delay(100);
//		  HAL_UART_Transmit(&huart2,(uint8_t*)(data),size,0xffff);
			HAL_UART_Transmit(&huart2,(uint8_t*)data,size,0xffff);
		}


}


//handle WiF
//+IPD,0,11:1234567890  //这个是处理握手信号的


//+IPD,0,5:10XX 药箱温度deepseek
//+IPD,0,5:11xx 药箱湿度


//data 10:open lay
//data 11:close lay

//data 20:open fan
//data 21:close fan

//data 30:+ moto
//data 31:-moto

static  void handle_wifi_data(void)
{
		
}