#include "main.h"


#define LED_RED 0
#define delay_ms(x) for(uint32_t i=0; i<x*10000;i++)__nop();
int main(void)
{
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[8] = PORT_PCR_MUX(1);
	PTB->PDDR |= (1<<8);
	
	while(1)
	{
			PTB->PTOR |= (1<<8);
			delay_ms(500);
	}
}
