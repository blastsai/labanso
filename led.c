#include "led.h"

uint32_t volatile greenled = 0, redled = 0, msTick = 0;
volatile uint8_t isSyspause = 0;
void init_SysTick_interrupt() {
	SysTick->LOAD = SystemCoreClock / 1000;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |SysTick_CTRL_TICKINT_Msk |SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void) {
	msTick++;
	if (!isSyspause) {
		greenled++;
		redled = 0;
		PTE->PSOR |= 1 << 29;
	}
	if (isSyspause) {
		redled++;
		greenled = 0;
		PTD->PSOR |= 1 << 5;
	}
	if (greenled > 1000) {
		PTD->PTOR |= 1 << 5;
		greenled = 0;
	}
	if (redled > 500) {
		PTE->PTOR |= 1 << 29;
		redled = 0;
	}
}	

void delay(uint16_t delay) {
	while (msTick < delay);
	msTick = 0;
}

void init_LED() {
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
	PORTD->PCR[5] |= PORT_PCR_MUX(1);
	PTD->PDDR |= 1 << 5;
	PTD->PSOR |= 1 << 5;
	
	PORTE->PCR[29] |= PORT_PCR_MUX(1);
	PTE->PDDR |= 1 << 29;
	PTE->PSOR |= 1 << 29;
}

void init_SW() {
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC->PCR[3] |= PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x0A) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PTC->PDDR &= ~(1 << 3);
	
	PORTC->PCR[12] |= PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x0A) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PTC->PDDR &= ~(1 << 12);
	
	NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
	
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
}

void PORTC_PORTD_IRQHandler(void) {
	if (!(PTC->PDIR & 1 << 12)) {
		NVIC_SystemReset();
		PORTC->PCR[12] |= PORT_PCR_ISF_MASK;  
	}
	if (!(PTC->PDIR & 1 << 3)) {
		isSyspause = 1 - isSyspause;
		PORTC->PCR[3] |= PORT_PCR_ISF_MASK;
	}
}