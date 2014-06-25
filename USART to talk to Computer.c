
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

// Variables
uint8_t mycharacter='D', dummy;
int x=0;

// Function Prototypes
uint8_t getChar(void);
void putChar(uint8_t character);



int main (void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

  //Configure USART2 pins:  Rx and Tx ----------------------------
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //Configure USART2 setting:         ----------------------------
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);

  USART_Cmd(USART2,ENABLE);
	
	
		
	
  while(1)
  {
		dummy = getChar();
		if(dummy!= 0xFF)
		{
			mycharacter=dummy;
			
			if(mycharacter>=97 && mycharacter<=122)
			{
			putChar(mycharacter-32);
			}
			
			else if (mycharacter==0xA || mycharacter==0xD)
			{
				putChar(0xA);
				putChar(0xD);
			}
			
			else
				putChar(mycharacter);
		}
  }

  return 0;
}



uint8_t getChar()
{
	if((USART2->ISR & (1<<5))!=0)
	{
		return (USART2->RDR & (uint16_t)0x0FF);
	}
	return 0xFF;
}


void putChar(uint8_t character)
{
	int i=0;
	while((USART2->ISR & (1<<7))==0)
	{}
		
	USART2->TDR = (character & (uint16_t)0x01FF);
		for(i=0; i<20000; i++)
		{}
			
	
}