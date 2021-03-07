#include "clock.h"

oscillator::oscillator(uint32_t type, uint32_t HSEState, uint32_t LSEState,
	                   uint32_t HSIState, uint32_t MSIState, uint32_t HSICalibration,
	                   uint32_t MSICalibration, uint32_t MSIClockRange,
	                   uint32_t PLLState, uint32_t PLLM_Divider,
	                   uint32_t PLL_Source_MSI, uint32_t LSE_Drive,
	                   uint32_t PWR_RegScale) {
	OscillatorInit.OscillatorType = type;
	OscillatorInit.HSEState = HSEState;
	OscillatorInit.LSEState = LSEState;
	OscillatorInit.HSIState = HSIState;
	OscillatorInit.MSIState = MSIState;
	OscillatorInit.HSICalibrationValue = MSICalibration;
	OscillatorInit.MSICalibrationValue = MSICalibration;
	OscillatorInit.MSIClockRange = MSIClockRange;
	OscillatorInit.PLL.PLLState = PLLState;
	this->PLLM_Divider = PLLM_Divider;
	this->PLL_Source_MSI = PLL_Source_MSI;
	this->LSE_Drive = LSE_Drive;
	this->PWR_RegScale = PWR_RegScale;
}

int oscillator::Init() {
	int ret;
	/* Macro to configure the PLL multiplication factor */
	__HAL_RCC_PLL_PLLM_CONFIG(PLLM_Divider);
	/* Macro to configure the PLL clock source */
	__HAL_RCC_PLL_PLLSOURCE_CONFIG(PLL_Source_MSI);
	/* Configure LSE Drive Capability */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(LSE_Drive);
	/* Configure the main internal regulator output voltage */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_RegScale);

	ret = HAL_RCC_OscConfig(&OscillatorInit);

	return ret;
}

sys_clock::sys_clock(uint32_t type, uint32_t source, uint32_t AHBDivider,
             uint32_t APB1CLKDivider, uint32_t APB2CLKDivider,
             uint32_t AHB2CLK2Divider, uint32_t AHBCLK2Divider,
             uint32_t AHBCLK4Divider) {
	clockInit.ClockType = type;
	clockInit.SYSCLKSource = source;
	clockInit.AHBCLKDivider = AHBDivider;
	clockInit.APB1CLKDivider = APB1CLKDivider;
	clockInit.APB2CLKDivider = APB2CLKDivider;
	clockInit.AHBCLK2Divider = AHBCLK2Divider;
	clockInit.AHBCLK4Divider = AHBCLK4Divider;
}

int sys_clock::Init() {
	int ret;

	ret = HAL_RCC_ClockConfig(&clockInit, FLASH_LATENCY_1);
	HAL_RCCEx_EnableMSIPLLMode();

	return ret;
}

per_clock::per_clock(uint32_t selector, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ,
        uint32_t PLLR, uint32_t PLLSAI1ClockOut, uint32_t Usart1ClockSelection,
        uint32_t UsbClockSelection, uint32_t SmpsClockSelection,
        uint32_t SmpsDivSelection) {
	clockInit.PeriphClockSelection = selector;
	clockInit.PLLSAI1.PLLN = PLLN;
	clockInit.PLLSAI1.PLLP = PLLQ;
	clockInit.PLLSAI1.PLLQ = PLLR;
	clockInit.PLLSAI1.PLLR = PLLQ;
	clockInit.PLLSAI1.PLLSAI1ClockOut = PLLSAI1ClockOut;
	clockInit.Usart1ClockSelection = Usart1ClockSelection;
	clockInit.SmpsClockSelection = SmpsClockSelection;
}

int per_clock::Init() {
	int ret;

	ret = HAL_RCCEx_PeriphCLKConfig(&clockInit);

	return ret;
}


