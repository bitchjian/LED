#include "delay.h"
#include "led.h"
int main(void)
{	
	LED_Init();//LED≥ı ºªØ
	delay_init();
	while(1)
	{
		LED0 = !LED0;
		LED1 = !LED1;
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
	}
}

