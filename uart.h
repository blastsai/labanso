#ifndef UART_H
#define UART_H

#include "MKL46Z4.h"
#include <stdio.h>

#define UART_BAUDRATE 115200 // UART baud rate

void init_UART(void);
void UART_SendChar(unsigned char data);
void UART_SendDecimal(int16_t n, char buffer[]);
void UART_SendString(const char* str);

#endif