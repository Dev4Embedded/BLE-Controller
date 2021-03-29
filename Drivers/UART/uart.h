#ifndef __UART_H__
#define __UART_H__

#include <string>

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_uart.h"
#include "gpio.h"

class uart {
private:
	USART_TypeDef *instance;
	UART_HandleTypeDef main;
	UART_InitTypeDef init;
	gpio* pinTX;
	gpio* pinRX;
public:
	uart(USART_TypeDef *instance, uint32_t baudrate, uint32_t wordlen,
	     uint32_t stopbits, uint32_t parity, int16_t mode, uint32_t flowCtrl,
	     bool OneBitSampling, uint32_t clockPrescaler);
	~uart();

	UART_HandleTypeDef* GetMain();
	void SetPins(gpio* tx, gpio* rx);
	int Init();
	void Send(std::string str);
};

#endif
