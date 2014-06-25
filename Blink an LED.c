   #include <stm32f0xx.h>

/*----------------------------------------------------------------------------
  wait function
 *----------------------------------------------------------------------------*/
volatile uint32_t cycleTime=1000;

void delay (unsigned int x)  {
  int  d;
  int a;
	a = 3000*x;
  for (d = 0; d < a; d++);             /* only to delay for LED flashes */
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) 
{
 
	RCC ->AHBENR |= (1<<19);  // Enable clock for port C
	RCC ->AHBENR |= (1<<17);  //Enable clock for port A
	GPIOC->MODER |= (1<<16);  //Enable Port C Pin8 as digital output
	GPIOC->MODER |= (1<<18);  // Enable Port C Pin9 as digital output
	GPIOA->MODER &= ~(GPIO_MODER_MODER0);  // Set Push Button as Input
	GPIOC ->ODR |= (1<<8);  // Turn GPIO Pin8 high

	
	while(1)
		{
			GPIOC ->ODR |= (1<<8);  // Turn on blue led
			delay(cycleTime/2);     
			GPIOC ->ODR &= ~(1<<8);   // Turn off blue led
			delay(cycleTime/2);
			
			
			if( GPIOA ->IDR & GPIO_IDR_0) // Check if push button pressed
			{
					GPIOC ->ODR |= (1<<9);  // Turn on green led
				  if (cycleTime>100)      // Decrement cycleTime
					{
						cycleTime = cycleTime-100;
					}
					else cycleTime = 1000;
					
					
					while (GPIOA->IDR & GPIO_IDR_0)  // While loop waits until the button is released
					{
					}
					GPIOC ->ODR &= ~(1<<9);  // Turns green led off
					
					
			}
			
    }
		return 0;
}