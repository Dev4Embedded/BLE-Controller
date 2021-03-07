#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "stm32wbxx_hal.h"

class oscillator {
private:
	RCC_OscInitTypeDef OscillatorInit;
	int PLLM_Divider;
	int PLL_Source_MSI;
	int LSE_Drive;
	int PWR_RegScale;

public:
	oscillator(uint32_t type, uint32_t HSEState, uint32_t LSEState,
	           uint32_t HSIState, uint32_t MSIState, uint32_t HSICalibration,
	           uint32_t MSICalibration, uint32_t MSIClockRange,
	           uint32_t PLLState, uint32_t PLLM_Divider,
	           uint32_t PLL_Source_MSI, uint32_t LSE_Drive,
	           uint32_t PWR_RegScale);

	~oscillator();
	int Init();
};

class sys_clock {
private:
	RCC_ClkInitTypeDef clockInit;

public:
	sys_clock(uint32_t type, uint32_t source, uint32_t AHBDivider,
	          uint32_t APB1CLKDivider, uint32_t APB2CLKDivider,
	          uint32_t AHB2CLK2Divider, uint32_t AHBCLK2Divider,
	          uint32_t AHBCLK4Divider);
	~sys_clock();
	int Init();
};

class per_clock {
private:
	RCC_PeriphCLKInitTypeDef clockInit;

public:
	per_clock(uint32_t selector, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ,
	          uint32_t PLLR, uint32_t PLLSAI1ClockOut, uint32_t Usart1ClockSelection,
	          uint32_t UsbClockSelection, uint32_t SmpsClockSelection,
	          uint32_t SmpsDivSelection);
	~per_clock();
	int Init();
};

#endif

