#include "stm32f0xx.h"

//Initializations
int BlinkSpeed = 500, GreenSpeed=500, NumberOfBlinks=1, NumberOfBlinks1=1, Ticker=0, Ticker1=0, Ticker2=0, row =0, column=0, flag=0;
int PotVoltage=0, flag1=0, flag2=0;
int i =0;



//Function Prototypes
void SysTick_Handler(void);
int theRealGetKey(void);
void delay(void);




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
	
	GPIOA->MODER |= (GPIO_MODER_MODER0);
}

RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
 
// Configures Systick Timer (1 millisecond)
	SysTick_Config(SystemCoreClock / 1000);  

// Initializations for ADC
{
	ADC1->CR |= (1);
	ADC1->CHSELR |= (1);

	delay();
}


 

	/* Infinite loop */
 while (1)
 {  
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

void delay(void)
{
		Ticker2=0;		// 2 Milisecond Delay
	while (Ticker2<1000)
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

