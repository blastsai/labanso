#include "magnetometor.h"

void init_I2C()  
{
		SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
		SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

		// configure GPIO for I2C function
		PORTE->PCR[24] = PORT_PCR_MUX(5);
		PORTE->PCR[25] = PORT_PCR_MUX(5);


		I2C0->F  = I2C_F_MULT(2) | I2C_F_ICR(0); // baudrate
		I2C0->C1 = I2C_C1_IICEN_MASK; // enable IIC
}

static void delay2(void)
{
    for (int i=0; i<100; i++);
}

void I2C_write(uint8_t data)
{
    I2C0->D = data;
		while((I2C0->S & I2C_S_IICIF_MASK)==0);
		I2C0->S = I2C_S_IICIF_MASK;
}

void init_mag(void)
{
    I2C0->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK;
    I2C_write(0x0E << 1); 
    I2C_write(0x10); 
    I2C_write(0x01); 
    I2C0->C1 &= ~I2C_C1_MST_MASK & ~I2C_C1_TX_MASK;
		delay2();
}

uint8_t read_mag(uint8_t reg)
{
    uint8_t value = 0;

    I2C0->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK;
    I2C_write(0x0E << 1); 
    I2C_write(reg); 
    
    I2C0->C1 |= I2C_C1_RSTA_MASK; 
    I2C_write((0x0E << 1) | 1); 
    
		I2C0->C1 &= ~I2C_C1_TX_MASK; 
    
		I2C0->C1 |=	I2C_C1_TXAK_MASK;

		value = I2C0->D; 
		while((I2C0->S & I2C_S_IICIF_MASK)==0);
		I2C0->S = I2C_S_IICIF_MASK;
    I2C0->C1 &= ~I2C_C1_MST_MASK & ~ I2C_C1_TX_MASK;
			
		value = I2C0->D;
    delay2();
    return value;
}
