#include "tim.h"
#include "main.h"
uint16_t temp =0;
uint8_t porch = 0;
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
	//TPM0->SC |= TPM_SC_TOIE_MASK;
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
void VSYNC_Init(void)
{
//TPM1 CH1 CH2
	PORTB->PCR[13] |= PORT_PCR_MUX(1);
	PTB->PDDR |= (1<<13);
	/*SIM->SCGC6 |=SIM_SCGC6_TPM1_MASK;
	SIM->SOPT4 |=SIM_SOPT2_TPMSRC(1);
	
	
	TPM1->CONF |= TPM_CONF_TRGSEL(8);
	//TPM1->CONF |= TPM_CONF_CSOT_MASK;
	TPM1->SC &=~TPM_SC_CPWMS_MASK;
	TPM1->SC |= TPM_SC_CMOD(2);
	TPM1->SC |= TPM_SC_PS(0);
	
	TPM1->MOD = 625;
	TPM1->CONTROLS[1].CnSC |= (TPM_CnSC_ELSB_MASK | TPM_CnSC_MSB_MASK);
	TPM1->CONTROLS[1].CnV = 2;
	*/
}
void TPM0_IRQHandler(void)
{
	//TPM0->SC |= TPM_SC_TOF_MASK;
	temp++;
	if(temp >= 622)
	{
		PTB->PTOR |=(1<<8);
		PTB->PSOR |= (1<<13);
		temp=0;
		porch = 1;
	}
	if(porch == 1 && temp >=22)
	{
		PTB->PCOR |=(1<<13);
		porch = 0;
	}
	TPM0->CONTROLS[1].CnSC |= TPM_CnSC_CHF_MASK;
}
