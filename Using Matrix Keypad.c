#include <STM32F0xx.h>



int BlinkRate; // variable for blinkrate
int flag=1;//variable to reset button press check
int Ticker;//=0
int BlinkSpeed=500;
int ScanCode=1;
int GetKey();
int row=0;
int column=0;
int temp=0;

uint32_t Button_Press(void)// button function
{
	return (GPIOA->IDR & (1UL << 0));//PA0;  read button press
}

int Getkey() 
{
	for (row=0; row<4; row++)
	{
		GPIOC->ODR |= (0x0F);
		GPIOC->ODR &= ~(1<<row);
		for(column=0; column<4; column++)
		{
			if ((GPIOC->IDR & (1<< (column+4)))==0)
			{
			return (4*row+column+1);
			}
		}
	}
	
	return 0;
}





void SysTick_Handler(void)
{
temp=GetKey();
if (temp!=0)
{
ScanCode = temp;
}
//ScanCode=10;
ScanCode=GetKey();
Ticker++;
}

int main(void)
{ 
//initilize SysTick clock and led 
	RCC->AHBENR |= (1UL << 19);// Enable clock for GPIOC that is why it is shifted 19 to get to port c
	
//Configure LED (PC.8..9) pins as push-pull outputs // force 0 or 1
	GPIOC->MODER  &= ~((3UL << 2 * 8)); //to get 0's(&= ~)
	GPIOC->MODER  |=  ((1UL << 2 * 8)); //to get 1's(|=)
	GPIOC->OTYPER &= ~((1UL <<     8));
	SysTick_Config(SystemCoreClock/1000); //1 milisecond; SystemCoreClock works at 48MHZ 

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

// MATRIX OUTPUTS
    GPIOC->MODER |= (GPIO_MODER_MODER0_0);  //Enable PC0 as digital output
    GPIOC->MODER |= (GPIO_MODER_MODER1_0);  //Enable PC1 as digital output
    GPIOC->MODER |= (GPIO_MODER_MODER2_0);  //Enable PC2 as digital output
    GPIOC->MODER |= (GPIO_MODER_MODER3_0);  //Enable PC3 as digital output
    
// Green and Blue LED OUTPUTS
    GPIOC->MODER |= (1<<16);  //Enable Port C Pin8 as digital output (Blue Led)
    
    
// Set all rows to be high
    GPIOC->ODR |= (GPIO_ODR_0); 
    GPIOC->ODR |= (GPIO_ODR_1);
    GPIOC->ODR |= (GPIO_ODR_2);
    GPIOC->ODR |= (GPIO_ODR_3);    
    
	
//Initilize button press
	BlinkRate = 1;

	
	
//Infinite While Loop//
	while(1)
	{	
		if(ScanCode!= 0)
		{
		BlinkRate=ScanCode;
		}

		BlinkSpeed = (1000/(BlinkRate*2)); //function 



		//Blue LED //	

		if(Ticker > BlinkSpeed) // 500 is going to become a variable BlinkSpeed
		{
		 GPIOC->ODR ^= (1 << 8); // switches existing state (exclusive OR) to opposite using ouput data reg
		Ticker = 0; //Reset Ticker
		}

	}
}



