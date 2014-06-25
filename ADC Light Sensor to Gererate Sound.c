#include "stm32f0xx.h"


int BlinkSpeed = 500, GreenSpeed=500, NumberOfBlinks=1, NumberOfBlinks1=1, Ticker=0, Ticker1=0, Ticker2=0, row =0, column=0, flag=0;
int PotVoltage=0, LightVoltage=0, flag1=0, flag2=0, frequency=261, TimerPeriod=0, Channel1Pulse = 0, Channel2Pulse = 0, Ch1Pulse=50, Ch2Pulse=50;
int i =0, k=0, d=1, Ticker3=0, Setting=0, p=0, flag3=0, OldTimerPeriod, OldLightVoltage;
float frequencyArray[200];

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

void SysTick_Handler(void);
int theRealGetKey(void);
void delay(void);
void TIM_Config(void);
void PushButtonCheck(void);





int main(void)
{	
	
// Initializations for KeyPad and Leds
	{
  
  	RCC ->AHBENR |= (1<<19);  // Enable clock for port C
		RCC ->AHBENR |= (1<<17);  // Enable clock for port A

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
	
	
	// MATRIX OUTPUTS
	GPIOC->MODER |= (GPIO_MODER_MODER0_0);  //Enable PC0 as digital output
	GPIOC->MODER |= (GPIO_MODER_MODER1_0);  //Enable PC1 as digital output
	GPIOC->MODER |= (GPIO_MODER_MODER2_0);  //Enable PC2 as digital output
	GPIOC->MODER |= (GPIO_MODER_MODER3_0);  //Enable PC3 as digital output
	
	// LEDs as outputs
	GPIOC->MODER |= (GPIO_MODER_MODER8_0);  
	GPIOC->MODER |= (GPIO_MODER_MODER9_0);	
	
	GPIOA->MODER &= ~(GPIO_MODER_MODER0);    // Pushbutton as input
	GPIOA->MODER |= (GPIO_MODER_MODER1);		// Set PA 1 as analog
	GPIOA->MODER |= (GPIO_MODER_MODER2);    // Set PA 2 as analog
}


// Initializations for TIM1
	{
	/* TIM Configuration */
	  TIM_Config();

	// ADC clock enable
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
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


// Configures Systick Timer (1 millisecond)
	SysTick_Config(SystemCoreClock / 1000);  


// Initializations for ADC
{
	ADC1->CR |= (1);
	ADC1->CHSELR |= (1<<2);
	
}





	/* Infinite loop */
 while (1)
 {  
	 
	 
	 // Using the pushbutton to turn speakers on and off
{
	 PushButtonCheck();
	 
	 
	 switch(Setting)
	{
		case 0: 
			GPIOA->AFR[1] &= ~(1<<7);  // Turn off Speaker 2 using AFRH 
			GPIOA->AFR[1] &= ~(1<<3);

			break;
		
		case 1: 
			GPIOA->AFR[1] |= (1<<7);
			GPIOA->AFR[1] |= (1<<3);

			break;
		
	}
}
	

			ADC1->CHSELR &= ~(1<<1);
			ADC1->CHSELR |= (1<<2);
			delay();
	 		ADC1->CR |= (1<<2);		// Start convertions								
			delay();							// delay
			PotVoltage = (ADC1->DR & (0xFFF));    // Get reading from Pot
   
	for(i=0; i<=19; i++) 
	{
			if ((PotVoltage>(i*200)) && PotVoltage<=((i*200)+200))
			{
					NumberOfBlinks1=(i+1);
					GreenSpeed = (1000/(NumberOfBlinks1*2));
			}
	}
	 
	  ADC1->CHSELR &= ~(1<<2);
		ADC1->CHSELR |= (1<<1);
		delay();
		ADC1->CR |= (1<<2);		// Start convertions								
		delay();							// delay
		OldLightVoltage = LightVoltage;
		LightVoltage = (ADC1->DR & (0xFFF));    // Get reading from Pot
		
			
	
			
		frequency= (((LightVoltage*2)/5)+100);
					
			
	

	 NumberOfBlinks = theRealGetKey();    // Update NumberOfBlinks from Keypad
	 if(NumberOfBlinks!=0)
		{
			BlinkSpeed = (1000/(NumberOfBlinks*2));
		}
		
		
		
	 // Flash Blue Led
		if((Ticker>(BlinkSpeed)) && (flag==0))
		{	
			Ticker = 0;
			flag=1;
			GPIOC ->ODR |= (1<<8);  // Turn on blue led
		}		
		if((Ticker>(BlinkSpeed)) && (flag==1))  // Turn off blue led
		{	
			Ticker=0;
			flag=0;
			GPIOC ->ODR &= ~(1<<8);
		}
		
		
		
		// Flash Green Led
				if((Ticker1>(GreenSpeed)) && (flag1==0))
		{	
			Ticker1 = 0;
			flag1=1;
			GPIOC ->ODR |= (1<<9);  // Turn on blue led
		}		
		if((Ticker1>(GreenSpeed)) && (flag1==1))  // Turn off blue led
		{	
			Ticker1=0;
			flag1=0;
			GPIOC ->ODR &= ~(1<<9);
		}
		
		
		// Change the Frequency for speakers
		{		
			OldTimerPeriod=TimerPeriod;
			TimerPeriod = (48000 / frequency) - 1;
				
			if(frequency==0)
			{
			  TimerPeriod=0;
			}
		
		    /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
		    Channel1Pulse =  (( Ch1Pulse * (TimerPeriod - 1)) / 100);
		  
		    /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 */
		    Channel2Pulse =  (( Ch2Pulse * (TimerPeriod - 1)) / 100);
      
			
			// Update the period of TIM1 which changes the frequency
		    if(TIM1->CNT > TimerPeriod)
		    {TIM1->CNT = 0;}
		
	
			TIM1->ARR = TimerPeriod ;
			TIM1->CCR1 = Channel1Pulse;
			TIM1->CCR2 = Channel2Pulse;
	
		}	
	}
}		
		
 
 
 // Function Definitions
 
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




void delay(void)
{
	Ticker2=0;						
	while (Ticker2<500)
	{
		Ticker2++;
	}
	Ticker2=0;
}




void SysTick_Handler(void)
{
	    Ticker++;	
			Ticker1++;

			
}




void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA Clocks enable */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* GPIOA Configuration: Channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);

}




void PushButtonCheck(void)
{
	
	int p=0;
	
	if((GPIOA->IDR & 1) == 0)
	{
		flag3 =0;
	}
	
	if(flag3==0)
	{
		if((GPIOA->IDR & 1) != 0)
		{
			while(p<10000)
			{
				p++;
			}
		
			if((GPIOA->IDR & 1) != 0)
			{
					Setting++;
					if(Setting>1)
					{
						Setting=0;
					}
					
					flag3 = 1;
			}
		}
}
}

