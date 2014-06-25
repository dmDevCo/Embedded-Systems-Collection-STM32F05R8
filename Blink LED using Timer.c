#include <stm32f0xx.h>
	 
	 
volatile uint32_t NumberOfBlinks=1;
volatile uint32_t Ticker=0;
volatile uint32_t TickerTwo=0;
int i =0;
volatile uint32_t BlinkSpeed = 500;



void GreenLed(void)
{
	// *****  Debouncing   *****
   if( GPIOA ->IDR & GPIO_IDR_0)  // Check if push button pressed
	{
	  i=0;
	  while(i<10000)   // Kill time for Debouncing
	  {
		i++;
	  }
				
				
	  if( GPIOA ->IDR & GPIO_IDR_0)  // Check if button pressed again
		{
		  GPIOC ->ODR |= (1<<9);  // Turn on green led
		
                  if (NumberOfBlinks<=10)      // Increment blinks per second
			{
			  NumberOfBlinks++;
                        }
                  
		  if( NumberOfBlinks > 10)	//Reset the number of blinks if greater than 10
			{
			  NumberOfBlinks=1;
			}
				
		  BlinkSpeed = (1000/(NumberOfBlinks*2));  // Set new value for BlinkSpeed (ms)
					
		  while (GPIOA->IDR & GPIO_IDR_0)  // While loop waits until the button is released
		  {
		  }
		  GPIOC ->ODR &= ~(1<<9);  // Turns green led off			
		}
	}
}


	 


void SysTick_Handler(void)
{
	    GreenLed();
	    Ticker++;		
}
	




	 
int main (void) 
{
	
	RCC ->AHBENR |= (1<<19);  // Enable clock for port C
	RCC ->AHBENR |= (1<<17);  //Enable clock for port A
	GPIOC->MODER |= (1<<16);  //Enable Port C Pin8 as digital output
	GPIOC->MODER |= (1<<18);  // Enable Port C Pin9 as digital output
	GPIOA->MODER &= ~(GPIO_MODER_MODER0);  // Set Push Button as Input
	
	SysTick_Config(SystemCoreClock / 1000);  // 1 millisecond
	
	
	
	
	BlinkSpeed= (1000/(NumberOfBlinks*2));
	
	while(1)
		{
			

			GPIOC ->ODR |= (1<<8);  // Turn on blue led
			
			Ticker = 0;
				while(Ticker<(BlinkSpeed))
				{
					//Do nothing
				}

			
			GPIOC ->ODR &= ~(1<<8);   // Turn off blue led
			
				Ticker=0;
				while(Ticker<(BlinkSpeed))
				{
					//Do nothing
				}
		}
	
} 
