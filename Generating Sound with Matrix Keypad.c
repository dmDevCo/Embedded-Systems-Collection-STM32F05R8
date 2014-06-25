#include "stm32f0xx.h"


TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0;


void TIM_Config(void);
int theRealGetKey(void);
void PushButtonCheck(void);
void Error(void);
int KeypadInput[5]={0};   
int a=0, b=0;
int Ch1Pulse=50;
int Ch2Pulse=50;
int flag1=0, flag2=0;
void GetFrequency(void);
void shiftleft(void);
int tempFrequency=0;
void CheckDutyCycle(void);
int c;
int i =0;
int flag=0;
int Setting=0;
int scancode;
int frequency=0;
int row =0, column=0;




int main(void)
{
	
	// Initializations for KeyPad and Leds
	{
 
  
  // *******************************************************************************************************
  
  	RCC ->AHBENR |= (1<<19);  // Enable clock for port C
		RCC ->AHBENR |= (1<<17);  // Enable clock for port C

	// Sets intputs as pull up
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR4_0);  // Set PC4 as Pull up
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR5_0);  // Set PC5 as Pull up
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR6_0);  // Set PC6 as Pull up
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR7_0);  // Set PC7 as Pull up
	
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR4_1);  // Set PC4 as Pull up  
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR5_1);  // Set PC5 as Pull up
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6_1);  // Set PC6 as Pull up
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR7_1);  // Set PC7 as Pull up
	
		// KEYPAD INPUTS
	GPIOC->MODER &= ~(GPIO_MODER_MODER4);  // Set PC4 as Input
	GPIOC->MODER &= ~(GPIO_MODER_MODER5);  // Set PC5 as Input
	GPIOC->MODER &= ~(GPIO_MODER_MODER6);  // Set PC6 as Input
	GPIOC->MODER &= ~(GPIO_MODER_MODER7);  // Set PC7 as Input
	
	//Clear led modes
	GPIOC->MODER &= ~(GPIO_MODER_MODER8); 
	GPIOC->MODER &= ~(GPIO_MODER_MODER9);
	
	// Sets PA0 as input
	GPIOA->MODER &= ~(GPIO_MODER_MODER0);
	
	// MATRIX OUTPUTS
	GPIOC->MODER |= (GPIO_MODER_MODER0_0);  //Enable PC0 as digital output
	GPIOC->MODER |= (GPIO_MODER_MODER1_0);  //Enable PC1 as digital output
	GPIOC->MODER |= (GPIO_MODER_MODER2_0);  //Enable PC2 as digital output
	GPIOC->MODER |= (GPIO_MODER_MODER3_0);  //Enable PC3 as digital output
	
	// LEDs as outputs
	GPIOC->MODER |= (GPIO_MODER_MODER8_0);  
	GPIOC->MODER |= (GPIO_MODER_MODER9_0);	
	
}

  
  
  
 //  ********************************************************************************************************
  
// Initializations for Tim1
{
  
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

  /* TIM Configuration */
  TIM_Config();
  
  /* TIM1 Configuration ---------------------------------------------------
   Generate PWM signals with 4 different duty cycles:
   TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)    
    => TIM1CLK = PCLK2 = SystemCoreClock
   TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
   SystemCoreClock is set to 48 MHz for STM32F0xx devices
   
   The objective is to generate 4 PWM signal at 261 KHz:
     - TIM1_Period = (SystemCoreClock / 261) - 1
   The channel 1 and channel 1N duty cycle is set to 50%
   The channel 2 and channel 2N duty cycle is set to 25%

   The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100  // Channel pulse says how long the output will be high for
   
   Note: 
    SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
    Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
    function to update SystemCoreClock variable value. Otherwise, any configuration
    based on this variable will be incorrect. 
  ----------------------------------------------------------------------- */

  

  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 1000;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;       //  This is the line that changes the auto relaod register
  
  
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1 and 2 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);



  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  }
  
  
  
 

	/* Infinite loop */
  while (1)
  {  
     PushButtonCheck();
	

    scancode = theRealGetKey();
	
		if (scancode==0)
		{
				flag1=0;
		}
	

		if(scancode == (13))
		{
			GetFrequency();	
		}

		
if (flag1==0)
{
		CheckDutyCycle();
		
		if(scancode == 12)
		{
			Error();
			flag1 = 1;
			
		}
		
				if(scancode == 16)
		{
			Error();
			flag1 = 1;
			
		}
}		
		
    
      
      
      TimerPeriod = (48000 / frequency) - 1;
      
      if(frequency==0)
      {
          TimerPeriod=0;
      }
		
     /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
      Channel1Pulse = (uint16_t) (((uint32_t) Ch1Pulse * (TimerPeriod - 1)) / 100);
      /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 */
      Channel2Pulse = (uint16_t) (((uint32_t) Ch2Pulse * (TimerPeriod - 1)) / 100);
      
      
      TIM1->ARR = TimerPeriod ;
      TIM1->CCR1 = Channel1Pulse;
      TIM1->CCR2 = Channel2Pulse;  
  }

}


int theRealGetKey()
{

  for(row=0; row<4; row++)
  {
      GPIOC->ODR |= (0x0F);
      GPIOC->ODR &= ~(1<<row);
    
       for(column=0; column<4; column++)
       {
         if((GPIOC->IDR & (1<< (column+4)))==0)
         { 
            return (4*row+column+1);
         }
       }
  }
  
  return 0;
}

void PushButtonCheck(void)
{
	
	int x=0;
	
	if((GPIOA->IDR & 1) == 0)
	{
		flag =0;
	}
	
	if(flag==0)
	{
		if((GPIOA->IDR & 1) != 0)
		{
			while(x<10000)
			{
				x++;
			}
		
			if((GPIOA->IDR & 1) != 0)
			{
					Setting++;
					if(Setting>3)
					{
						Setting=0;
					}
					
					flag = 1;
			}
		}
}
	
	switch(Setting)
	{
		case 0: 
			GPIOA->AFR[1] |= (1<<7);  // Turn off Speaker 2 using AFRH 
			GPIOA->AFR[1] &= ~(1<<3);
			GPIOC->ODR &= ~(1<<9);
			GPIOC->ODR |= (1<<8);
			break;
		
		case 1: 
			GPIOA->AFR[1] &= ~(1<<7);
			GPIOA->AFR[1] |= (1<<3);
		
			GPIOC->ODR |= (1<<9);
			GPIOC->ODR &= ~(1<<8);
			break;
		
		case 2: 
			GPIOA->AFR[1] &= ~(1<<7);
			GPIOA->AFR[1] &= ~(1<<3);
		
			GPIOC->ODR |= (1<<9);
			GPIOC->ODR |= (1<<8);
			break;
		
		case 3: 
			GPIOA->AFR[1] |= (1<<7);
			GPIOA->AFR[1] |= (1<<3);
			GPIOC->ODR &= ~(1<<9);
			GPIOC->ODR &= ~(1<<8);
			break;
	}
	
	
}




void Error()
{
	int a=0; 
	int b=0;
	int c=0;
	
	for(a=0; a<10; a++)
	{
	for(b=0; b<150000; b++)
	{
	GPIOC->ODR |= (1<<8);
	GPIOC->ODR |= (1<<9);
	}
	for(b=0; b<150000; b++)
		{
	GPIOC->ODR &= ~(1<<8);
	GPIOC->ODR &= ~(1<<9);
		}
	}
	for(c=0; c<=4; c++)
	{
	KeypadInput[c] = 0;
	}
	}


void GetFrequency()
{
	while(scancode!=15)
	{
		scancode = theRealGetKey();
		
		if (scancode==0)
		{
			int x=0;
			while(x<10000)
			{
				x++;
			}
			if (scancode==0)
			{
				flag2=0;
			}
		}
		
		if(flag2==0)
		{
		if(scancode!=0)
		{
			int x=0;
			while(x<10000)
			{
				x++;
			}
				
			if(scancode!=0)
			{
				GPIOC->ODR |= (1<<9);
				x=0;
				while(x<100000)
				{
				x++;
				}
				GPIOC->ODR &= ~(1<<9);
					switch (scancode)
				{
						case 1:
										shiftleft();
										KeypadInput[4]= scancode;
										flag2=1;
										break;
						case 2:
										shiftleft();
										KeypadInput[4]= scancode;
										flag2=1;
										break;
						case 3:
										shiftleft();
										KeypadInput[4]= scancode;
										flag2=1;
										break;
						case 4:
										Error();
										break;
						case 5:
										shiftleft();
										KeypadInput[4]= (scancode-1);
										flag2=1;
										break;
						case 6:
										shiftleft();
										KeypadInput[4]= (scancode-1);
										flag2=1;
										break;
						case 7:
										shiftleft();
										KeypadInput[4]= (scancode-1);
										flag2=1;
										break;
						case 8:
										Error();
										break;
						case 9:
										shiftleft();
										KeypadInput[4]= (scancode-2);
										flag2=1;
										break;
						case 10:
										shiftleft();
										KeypadInput[4]= (scancode-2);
										flag2=1;
										break;
						case 11:
										shiftleft();
										KeypadInput[4]= (scancode-2);
										flag2=1;
										break;
						case 12:
										Error();
										break;
						case 14:
										shiftleft();
										KeypadInput[4]= 0;
										flag2=1;
										break;
						
						case 16:
										Error();
										break;
						default:
										break;
					}
		
				}
			}
			}	
	}
	
	tempFrequency= (10000*	KeypadInput[0]);
	tempFrequency= tempFrequency+(1000*	KeypadInput[1]);
	tempFrequency= tempFrequency+(100*	KeypadInput[2]);
	tempFrequency= tempFrequency+(10*	KeypadInput[3]);
	tempFrequency= tempFrequency+(1*	KeypadInput[4]);
	
	if(tempFrequency>=20 && tempFrequency<=20000)
	{
		frequency = tempFrequency;
		
		for(c=0; c<=4; c++)
		{
			KeypadInput[c] = 0;
		}
	}
	else Error();
	// change array into an int
		//  check if frequency is within 20-20000
}

void shiftleft()
{
	KeypadInput[0]= KeypadInput[1];
	KeypadInput[1]= KeypadInput[2];
	KeypadInput[2]= KeypadInput[3];
	KeypadInput[3]= KeypadInput[4];
}

void CheckDutyCycle()
{
	if(scancode == 4)
		{
			Ch1Pulse=(Ch1Pulse+5);
			flag1 = 1;
			if(Ch1Pulse>95)
			{
				Ch1Pulse=0;
			}
		}
		
		if(scancode == 8)
		{
			Ch2Pulse=(Ch2Pulse+5);
			flag1 = 1;
			if(Ch2Pulse>95)
			{
				Ch2Pulse=0;
			}
		}
}

/**
  * @brief  Configure the TIM1 Pins.
  * @param  None
  * @retval None
  */
void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA Clocks enable */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* GPIOA Configuration: Channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);


}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
    
     

  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
