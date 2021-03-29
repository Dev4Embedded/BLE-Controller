#include "clock.h"
#include "gpio.h"
#include "uart.h"

#include "stm32wbxx_hal.h"

#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOB
#define STLINK_RX_Pin GPIO_PIN_6
#define STLINK_RX_GPIO_Port GPIOB
#define STLINK_TX_Pin GPIO_PIN_7
#define STLINK_TX_GPIO_Port GPIOB

oscillator *osc;
sys_clock *sys_clk;
per_clock *per_clk;

gpio *LED_blue;
gpio *LED_green;
gpio *LED_red;
gpio *uart_rx;
gpio *uart_tx;

uart *uart_dev;

void SystemClock_Config(void) {
	uint32_t osc_type = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE |
	                    RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;

	uint32_t clk_type = RCC_CLOCKTYPE_HCLK4 | RCC_CLOCKTYPE_HCLK2
	                   | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
	                   | RCC_CLOCKTYPE_PCLK2;

	uint32_t per_type = RCC_PERIPHCLK_SMPS | RCC_PERIPHCLK_USART1;

	osc = new oscillator(osc_type, RCC_HSE_ON, RCC_LSE_ON, RCC_HSI_ON, RCC_MSI_ON,
	                     RCC_HSICALIBRATION_DEFAULT, RCC_MSICALIBRATION_DEFAULT,
	                     RCC_MSIRANGE_6, RCC_PLL_NONE, RCC_PLLM_DIV1,
	                     RCC_PLLSOURCE_MSI, RCC_LSEDRIVE_LOW,
	                     PWR_REGULATOR_VOLTAGE_SCALE1);

	sys_clk = new sys_clock(clk_type, RCC_SYSCLKSOURCE_HSE, RCC_SYSCLK_DIV1, RCC_HCLK_DIV1,
	                        RCC_HCLK_DIV1, RCC_HCLK_DIV1, RCC_SYSCLK_DIV1, RCC_SYSCLK_DIV1);

	per_clk = new per_clock(per_type, 24, RCC_PLLP_DIV2, RCC_PLLQ_DIV2, RCC_PLLR_DIV2,
	                        RCC_PLLSAI1_USBCLK, RCC_USART1CLKSOURCE_PCLK2,
	                        RCC_SMPSCLKSOURCE_HSI, RCC_SMPSCLKDIV_RANGE0);
	(int)osc->Init();
	(int)sys_clk->Init();
	(int)per_clk->Init();
}

static void GPIO_Init(void)
{
	LED_blue = new gpio(GPIOB, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
	                    GPIO_SPEED_FREQ_LOW);
	LED_green = new gpio(GPIOB, GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
	                     GPIO_SPEED_FREQ_LOW);
	uart_rx = new gpio(STLINK_RX_GPIO_Port, STLINK_RX_Pin, GPIO_MODE_AF_PP,
	                   GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_AF7_USART1);
	uart_tx = new gpio(STLINK_TX_GPIO_Port, STLINK_TX_Pin, GPIO_MODE_AF_PP,
	                   GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_AF7_USART1);
	LED_red = new gpio(GPIOB, GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
	                   GPIO_SPEED_FREQ_LOW);
	LED_blue->Init();
	LED_green->Init();
	LED_red->Init();
	uart_rx->Init();
	uart_tx->Init();
}

static void UART_Init()
{
	uart_dev = new uart(LPUART1,
	                    115200,
	                    UART_WORDLENGTH_8B,
	                    UART_STOPBITS_1,
	                    UART_PARITY_NONE,
	                    UART_MODE_TX_RX,
	                    UART_HWCONTROL_NONE,
	                    true,
	                    UART_PRESCALER_DIV1);

	uart_dev->SetPins(uart_tx, uart_rx);
	uart_dev->Init();
}

int main() {
	HAL_Init();
	SystemClock_Config();
	GPIO_Init();
	UART_Init();
	while (1) {
		LED_blue->SetHI();
		LED_red->SetLO();
		uart_dev->Send("Blink\n\r");
		volatile int i = 0;
		while (i < 300000)
			i++;
		LED_blue->SetLO();
		LED_red->SetHI();

		i = 0;
		while (i < 300000)
			i++;
	}

	return 0;
}
