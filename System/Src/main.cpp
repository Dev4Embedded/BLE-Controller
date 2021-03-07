#include "clock.h"
#include "gpio.h"

#include "stm32wbxx_hal.h"

#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOB

oscillator *osc;
sys_clock *sys_clk;
per_clock *per_clk;

gpio *LED;

void SystemClock_Config(void) {
	uint32_t osc_type = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE |
	                    RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;

	uint32_t clk_type = RCC_CLOCKTYPE_HCLK4 | RCC_CLOCKTYPE_HCLK2
	                   | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
	                   | RCC_CLOCKTYPE_PCLK2;

	uint32_t per_type = RCC_PERIPHCLK_SMPS | RCC_PERIPHCLK_USART1 |
	                    RCC_PERIPHCLK_USB;

	osc = new oscillator(osc_type, RCC_HSE_ON, RCC_LSE_ON, RCC_HSI_ON, RCC_MSI_ON,
	                     RCC_HSICALIBRATION_DEFAULT, RCC_MSICALIBRATION_DEFAULT,
	                     RCC_MSIRANGE_6, RCC_PLL_NONE, RCC_PLLM_DIV1,
	                     RCC_PLLSOURCE_MSI, RCC_LSEDRIVE_LOW,
	                     PWR_REGULATOR_VOLTAGE_SCALE1);

	sys_clk = new sys_clock(clk_type, RCC_SYSCLKSOURCE_HSE, RCC_SYSCLK_DIV1, RCC_HCLK_DIV1,
	                        RCC_HCLK_DIV1, RCC_HCLK_DIV1, RCC_SYSCLK_DIV1, RCC_SYSCLK_DIV1);

	per_clk = new per_clock(per_type, 24, RCC_PLLP_DIV2, RCC_PLLQ_DIV2, RCC_PLLR_DIV2,
	                        RCC_PLLSAI1_USBCLK, RCC_USART1CLKSOURCE_PCLK2,
	                        RCC_USBCLKSOURCE_PLLSAI1, RCC_SMPSCLKSOURCE_HSI,
	                        RCC_SMPSCLKDIV_RANGE0);
	(int)osc->Init();
	(int)sys_clk->Init();
	(int)per_clk->Init();
}

static void GPIO_Init(void)
{
	LED = new gpio(GPIOB, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
	               GPIO_SPEED_FREQ_LOW);

	LED->Init();
}

int main() {
	HAL_Init();
	SystemClock_Config();
	GPIO_Init();
	while (1) {
		LED->SetHI();
		volatile int i = 0;
		while (i < 300000)
			i++;
		LED->SetLO();
		i = 0;
		while (i < 300000)
			i++;
	}

	return 0;
}
