#include "tim.h"
#include "main.h"
uint32_t temp =0;
void HSYNC_Init(void)
{
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	PORTB->PCR[9] |= PORT_PCR_MUX(2);
	PORTB->PCR[10] |= PORT_PCR_MUX(2);
	
	TPM0->SC |= TPM_SC_PS(0);
	TPM0->SC |= TPM_SC_CMOD(1);
	TPM0->MOD = 1190;
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;
	TPM0->CONTROLS[2].CnSC |= (TPM_CnSC_ELSB_MASK | TPM_CnSC_MSB_MASK);
	//1 tick  = 0.0208us
	TPM0->CONTROLS[2].CnV = 86;
	TPM0->CONTROLS[1].CnSC |= (TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK);
	TPM0->CONTROLS[1].CnV = 146;
	TPM0->CONTROLS[1].CnSC |= (TPM_CnSC_CHIE_MASK);
	NVIC_SetPriority(TPM0_IRQn, 1);
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	NVIC_EnableIRQ(TPM0_IRQn);
}
void TPM0_IRQHandler(void)
{
	TPM0->CONTROLS[1].CnSC |= TPM_CnSC_CHF_MASK;
	temp++;
	if(temp >= 1000)
	{
		PTB->PTOR |= (1<<8);
		temp =0;
	}
}
