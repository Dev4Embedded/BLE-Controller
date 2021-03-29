#include <stdlib.h>
#include <string>
#include <vector>

#include "uart.h"
#include "stm32wbxx_hal_uart.h"

uart::uart(USART_TypeDef *inst, uint32_t baudrate, uint32_t wordlen,
	       uint32_t stopbits, uint32_t parity, int16_t mode, uint32_t flowCtrl,
	       bool OneBitSampling, uint32_t clockPrescaler) {
//	main.Instance = inst;
//	init.BaudRate = baudrate;
//	init.WordLength = wordlen;
//	init.StopBits = stopbits;
//	init.Parity = parity;
//	init.Mode = mode;
//	init.HwFlowCtl = flowCtrl;
//	if (OneBitSampling)
//		init.OverSampling = UART_OVERSAMPLING_16;
//	init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//	init.ClockPrescaler = clockPrescaler;

//	main.Init = init;
//	main.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	main.Instance = USART1;
	main.Init.BaudRate = 115200;
	main.Init.WordLength = UART_WORDLENGTH_8B;
	main.Init.StopBits = UART_STOPBITS_1;
	main.Init.Parity = UART_PARITY_NONE;
	main.Init.Mode = UART_MODE_TX_RX;
	main.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	main.Init.OverSampling = UART_OVERSAMPLING_16;
	main.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	main.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	main.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

}

int uart::Init() {
	 __HAL_RCC_USART1_CLK_ENABLE();
	if (HAL_UART_Init(&main))
		return -1;

	if (HAL_UARTEx_SetTxFifoThreshold(&main, UART_TXFIFO_THRESHOLD_1_8))
		return -1;

	if (HAL_UARTEx_SetRxFifoThreshold(&main, UART_RXFIFO_THRESHOLD_1_8))
		return -1;

	if (HAL_UARTEx_DisableFifoMode(&main))
		return -1;

	return 0;
}

UART_HandleTypeDef* uart::GetMain() {
	UART_HandleTypeDef* ret = &main;
	return ret;
}

void uart::SetPins(gpio* tx, gpio* rx) {
	pinTX = tx;
	pinRX = rx;
}

void uart::Send(std::string str) {
	int len;
	uint8_t *data_buff;
	std::vector<uint8_t> converter(str.begin(), str.end());

	len = str.length();
	data_buff = &converter[0];

	HAL_UART_Transmit(&main, data_buff, len, 0xFFFF);

}
