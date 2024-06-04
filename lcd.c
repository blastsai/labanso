/*  
	SLCD Library by Filip Swiatek
	Designed to use with FRDM KL46Z Board.
*/
#include "lcd.h"


void init_lcd(void){
	MCG->C1  |= MCG_C1_IRCLKEN_MASK | MCG_C1_IREFSTEN_MASK;
	SIM->SCGC5 |= SIM_SCGC5_SLCD_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
	
	LCD->GCR|=LCD_GCR_PADSAFE_MASK; 
	LCD->GCR &= ~LCD_GCR_LCDEN_MASK;
	
	PORTB->PCR[7] |= PORT_PCR_MUX(0); //LCD_P7
	PORTB->PCR[8] |= PORT_PCR_MUX(0); //LCD_P8
	PORTB->PCR[10] |= PORT_PCR_MUX(0); //LCD_P10
	PORTB->PCR[11] |= PORT_PCR_MUX(0); //LCD_P11
	PORTB->PCR[21] |= PORT_PCR_MUX(0); //LCD_P17
	PORTB->PCR[22] |= PORT_PCR_MUX(0); //LCD_P18
	PORTB->PCR[23] |= PORT_PCR_MUX(0); //LCD_P19
	PORTC->PCR[17] |= PORT_PCR_MUX(0); //LCD_P37
	PORTC->PCR[18] |= PORT_PCR_MUX(0); //LCD_P38
	PORTD->PCR[0] |= PORT_PCR_MUX(0); //LCD_P40
	PORTE->PCR[4] |= PORT_PCR_MUX(0); //LCD_P52
	PORTE->PCR[5] |= PORT_PCR_MUX(0); //LCD_P53
	
	LCD->GCR = 
	LCD_GCR_RVTRIM(0x00) |
	LCD_GCR_CPSEL_MASK | // charge pump settings
	LCD_GCR_LADJ(0x03) | // adjust to screen's capacitance
	LCD_GCR_ALTDIV(0x00) | 
	LCD_GCR_SOURCE_MASK |
	LCD_GCR_VSUPPLY_MASK |
	LCD_GCR_LCLK(0x01) |  
	LCD_GCR_DUTY(0x03); // adjust duty cycle
	
	//Config blink mode
	LCD->AR = LCD_AR_BRATE(0x0); // LCD blinking rate
	LCD->FDCR=0U;
	
	//Config pin cho LCD
	LCD->PEN[0] = LCD_PEN_PEN(1<<7) 
								|LCD_PEN_PEN(1<<8) 
								|LCD_PEN_PEN(1<<10)
								|LCD_PEN_PEN(1<<11)
								|LCD_PEN_PEN(1<<17)
								|LCD_PEN_PEN(1<<18)
								|LCD_PEN_PEN(1<<19);
	LCD->PEN[1] = LCD_PEN_PEN(1<<5) 
								|LCD_PEN_PEN(1<<6) 
								|LCD_PEN_PEN(1<<8)
								|LCD_PEN_PEN(1<<20)
								|LCD_PEN_PEN(1<<21);
								
	//Config back plane, front plane
	LCD->BPEN[0] = LCD_BPEN_BPEN(1<<18) | LCD_BPEN_BPEN(1<<19);
	LCD->BPEN[1] = LCD_BPEN_BPEN(1<<8) | LCD_BPEN_BPEN(1<<20);
	
	//Config COM pin
	LCD->WF8B[40] = (1<<0) | (1 << 4);
	LCD->WF8B[52] = (1<<1) | (1 << 5);
	LCD->WF8B[19] = (1<<2) | (1 << 6);
	LCD->WF8B[18] = (1<<3) | (1 << 7);
	
	LCD->GCR &= ~LCD_GCR_PADSAFE_MASK;
	LCD->GCR |= LCD_GCR_LCDEN_MASK;
}
	
const static uint8_t ABC[10] = {0xEE, 0x66, 0xCC, 0xEE, 0x66, 0xAA, 0xAA, 0xEE, 0xEE, 0xEE}; //Trang thai cac segment ABCDp cua 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
const static uint8_t FGED[10] = {0xBB,0x00,0x77,0x55,0xCC,0xDD,0xFF,0x00,0xFF,0xDD}; //Trang thai cua cac segment FGED cua 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
const static uint8_t digit_ABC[4] = {11, 38, 8, 17};//Tuong ung voi cac digit 4, 3, 2, 1; de nguoc de khi write vao cac digit cho thuan tien
const static uint8_t digit_FGED[4] = {10, 53, 7, 37};
	
	
	
void sLCD_write(uint8_t digit, uint8_t number) {
		LCD->WF8B[digit_ABC[digit]] = ABC[number];
		LCD->WF8B[digit_FGED[digit]] = FGED[number];
}

void sLCD_write_number(uint16_t number) {
	if(number>9999) return;
	uint8_t numberdigi[4]={0,0,0,0};
	
	while(number>=1000)
	{
		numberdigi[3]++;
		number=number-1000;
	}
	while(number>=100)
	 {
		numberdigi[2]++;
		number=number-100;
	}
	 while(number>=10)
	 {
		numberdigi[1]++;
		number=number-10;
	}
	 numberdigi[0]=number;
	
//	int j=4;
//	if (numberdigi[3] == 0)
//	{
//		j--;
//		if (numberdigi[2] == 0) {
//			j--;
//			if (numberdigi[1] == 0) {
//				j--;
//				if (numberdigi[0] == 0) {
//					j--;
//				}
//			}
//		}
//	}
	 
	for(uint8_t i=0;i<4;i++)
	{
		sLCD_write(i,numberdigi[i]);
	}
}