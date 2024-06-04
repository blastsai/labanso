#include "uart.h"

//UART0,PTA1=RX and PTA2=TX
void init_UART(void)
{
	// Enable clock to UART0
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	
	// Select MCGFLLCLK clock for UART0
	SIM->SOPT2 = 0x04000000;
	
    // Enable clock to Port A
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    
    // Configure pins for UART0 (PTA1 as RX, PTA2 as TX)
    PORTA->PCR[1] = PORT_PCR_MUX(2); // PTA1 as RX
    PORTA->PCR[2] = PORT_PCR_MUX(2); // PTA2 as TX
	
	// Disable UART0 transmitter and receiver while configuring
	UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
	
    // Configure for receiver and transmitter use 8 bit data characters
	UART0->C1 &= ~(UART0_C1_M_MASK  );
    
	// Configure for receiver and transmitter use 8 bit or 9 bit data characters
	UART0->C4 &= ~(UART0_C4_M10_MASK);
    
	// Configure for no parity
	UART0->C1 &= ~(UART0_C1_PE_MASK );
    
	// Configure LSB (bit 0) is first bit transmitted following start bit
	UART0->S2 &= ~(UART0_S2_MSBF_MASK);
    
	// Configure received data not inverted
	UART0->S2 &= ~(UART0_S2_RXINV_MASK);
		
	// Configure transmitted data not invert
	UART0->C3 &= ~(UART0_C3_TXINV_MASK);
    
	// Configure for 1 stop bit
	UART0->BDH &= ~(UART0_BDH_SBNS_MASK);

	/* Configure receiver interrupt enable for RDRF */
    // UART0->C2 |= UART0_C2_RIE_MASK;
	
    
    // Set baud rate
    uint16_t sbr = SystemCoreClock / (16 * UART_BAUDRATE);
    UART0->BDH = (sbr >> 8) & UART_BDH_SBR_MASK;
    UART0->BDL = (sbr & UART_BDL_SBR_MASK);
    
    // Enable UART0 transmitter and receiver
    UART0->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
}

void UART_SendChar(unsigned char data)
{
	// Wait until transmit buffer is empty
	while(!(UART0->S1 & UART_S1_TDRE_MASK));
	
	// Send data
	UART0->D = data;
}

void UART_SendDecimal(int16_t n, char buffer[])
{
	int count = sprintf(buffer, "%d", n);
	int i = 0;
	while (i < count) {
		UART_SendChar(buffer[i]);
		i++;
	}
}

void UART_SendString(const char* str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}