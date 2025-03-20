#include "esp8266.h"
#include "DHT11.h"


bool device_connect = false;

bool hand_shack = false;

static  void handle_wifi_data(void);
bool start_esp8266(void)
{ 
	
			HAL_UART_Transmit(&huart2,(uint8_t*)(AT_MODE2),13,0xffff);
			HAL_Delay(100);
	    while(1)
			{
				 if(rx2_end_flag)
					{
						//printf("%s\r\n",uart2_rx);
						rx2_end_flag = false;
						rx2_count=0;
						HAL_UART_Receive_DMA(&huart2,uart2_rx,UART2_SIZE);  //需要重新启动DMA
						break;
					}
			}
			
			HAL_UART_Transmit(&huart2,(uint8_t*)(AT_RST),8,0xffff);
			HAL_Delay(100);
	    while(1)
			{
				 if(rx2_end_flag)
					{
						//printf("%s\r\n",uart2_rx);
						rx2_end_flag = false;
						rx2_count=0;
						HAL_UART_Receive_DMA(&huart2,uart2_rx,UART2_SIZE);  //需要重新启动DMA
						break;
					}
			}
			
			HAL_UART_Transmit(&huart2,(uint8_t*)(AT_CIPMUX),13,0xffff);
			HAL_Delay(100);
	    while(1)
			{
				 if(rx2_end_flag)
					{
							//printf("%s\r\n",uart2_rx);
						rx2_end_flag = false;
						rx2_count=0;
						HAL_UART_Receive_DMA(&huart2,uart2_rx,UART2_SIZE);  //需要重新启动DMA
						break;
					}
			}
			
				 HAL_UART_Transmit(&huart2,(uint8_t*)(AT_CIPSERVER),16,0xffff);
			   HAL_Delay(100);
				 rx2_end_flag = false;
				 rx2_count=0;
				HAL_UART_Receive_DMA(&huart2,uart2_rx,UART2_SIZE);  //需要重新启动DMA
			
			 return true;
			 
}

void handle_esp8266(void)
{
	    uint8_t data[3]={'1','2','3'};
	   // char* connected ="0,CONNECT";
			char * connected = "+STA_CONNECTED";
			if(device_connect==false)
			{

				 	if(rx2_end_flag)
					{
						//printf("%s\r\n",uart2_rx);
						rx2_end_flag = false;

						if(memcmp(uart2_rx,connected,14)==0)
						{
								device_connect=true;
								//printf("connected\r\n");
						}
				
						rx2_count=0;
						memset(uart2_rx,0,10000);
						HAL_UART_Receive_DMA(&huart2,uart2_rx,UART2_SIZE);  //需要重新启动DMA
//						OLED_ShowString(1,50,(uint8_t*)"hello",16,1);
//					  OLED_Refresh();
					} 
			}
			else
			{
//				printf("handle_esp8266\r\n");
					if(rx2_end_flag)
					{
												//printf("%s\r\n",uart2_rx);
						rx2_end_flag = false;
						//printf("%s\r\n",uart2_rx);
						handle_wifi_data();
						rx2_count=0;
						memset(uart2_rx,0,10000);
						HAL_UART_Receive_DMA(&huart2,uart2_rx,UART2_SIZE);  //需要重新启动DMA
					} 
			}

			
}

void send_wifi(uint8_t *data,int size)
{
	  int offset = 0;  // 记录当前写入位置
	 if(hand_shack)
	 {
			char send_data[50]={0};
			char send_data1[50]={0};
			sprintf(send_data,"AT+CIPSEND=0,%d\r\n",size);
			HAL_UART_Transmit(&huart2,(uint8_t*)(send_data),16,0xffff);

			HAL_Delay(100);
		
			for(int i=0;i<size;i++)
			{
					offset += sprintf(send_data1 + offset, "%d ", data[i]);
			}
			sprintf(send_data1+size,"\r\n");
			HAL_UART_Transmit(&huart2,(uint8_t*)send_data1,size+2,0xffff);
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
	char * conect =  "\r\n+IPD,0,11:1234567890";
	char * set_rh = "\r\n+IPD,0,5:10";
	char * set_th = "\r\n+IPD,0,5:11";
	if(memcmp(conect,uart2_rx,22)==0)
	{
			hand_shack = true;
		  printf("hand shake\r\n");
	}
	
	if(memcmp(set_rh,uart2_rx,13)==0) //设置湿度值
	{
					rh_bang = (uart2_rx[13]-0x30)*10+(uart2_rx[14]-0x30);
		     printf("set_rh");
		     printf("%d\r\n",rh_bang);
	}
	
	if(memcmp(set_th,uart2_rx,13)==0) //设置温度值
	{
					th_bang = (uart2_rx[13]-0x30)*10+(uart2_rx[14]-0x30);
	}
		
}