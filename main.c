#include "lcd.h"
#include "led.h"
#include "magnetometor.h"
#include "uart.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

int16_t x_offset = 0;
int16_t y_offset = 0;
int16_t x_max = -1000;
int16_t x_min = 1000;
int16_t y_max = -1000;
int16_t y_min = 1000;
float x_scale = 1, y_scale = 1;
char a[30];

extern volatile uint8_t isSyspause;

int16_t x, y;

void mag_calibrate() {
	uint16_t i =0;
	while (i++ < 3000) {
		x = read_mag(0x01) << 8 | read_mag(0x02);
		y = read_mag(0x03) << 8 | read_mag(0x04);
		if (x > x_max) 
			x_max = x;
		if (x < x_min)
			x_min = x;
		if (y > y_max)
			y_max = y;
		if (y < y_min)
			y_min = y;
		delay(15);
	}
	x_offset = (x_max + x_min) / 2;
	y_offset = (y_max + y_min) / 2;
	x_scale = (x_max - x_min) * 1000;
	y_scale = (y_max - y_min) * 1000;
}

int main() {
	
	init_I2C();
	init_mag();
	init_lcd();
	init_LED();
	init_SW();
	init_SysTick_interrupt();
	mag_calibrate();
	
	int16_t angle;
	float x_float, y_float;
	while (1) {
		if (!isSyspause) {
			x = read_mag(0x01) << 8 | read_mag(0x02);
			y = read_mag(0x03) << 8 | read_mag(0x04);

			x_float = (float)(x - x_offset) * y_scale;
			y_float = (float)(y - y_offset) * x_scale;
			angle = (int16_t) (atan2(y_float, x_float) * 180 / 3.1415);
			if (angle < 0)
				angle += 360;
			sLCD_write_number(angle);
			delay(200);
		}
		else {
			
		}
	}
}