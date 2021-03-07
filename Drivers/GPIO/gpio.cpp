#include "gpio.h"

gpio::gpio(GPIO_TypeDef* type, uint32_t pin, uint32_t mode, uint32_t pull,
           uint32_t speed, uint32_t alternate) {
	this->type = type;
	init.Pin = pin;
	init.Mode = mode;
	init.Pull = pull;
	init.Speed = speed;
	init.Alternate = alternate;
};

gpio::gpio(GPIO_TypeDef* type, uint32_t pin, uint32_t mode, uint32_t pull,
           uint32_t speed) {
	this->type = type;
	init.Pin = pin;
	init.Mode = mode;
	init.Pull = pull;
	init.Speed = speed;
};

gpio::gpio(GPIO_TypeDef* type, uint32_t pin, uint32_t mode) {
	this->type = type;
	init.Pin = pin;
	init.Mode = mode;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_FREQ_LOW;
};

gpio::~gpio(){

}

void gpio::Init() {
	int ptr = int(this->type);
	const int gpiob = int(GPIOB);
	const int gpioc = int(GPIOC);
	const int gpiod = int(GPIOD);
	const int gpioe = int(GPIOE);
	const int gpioh = int(GPIOH);

	switch (ptr) {
		case gpiob: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
		case gpioc: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
		case gpiod: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
		case gpioe: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
		case gpioh: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
		default: return;
	}

	HAL_GPIO_Init(this->type, &init);
}

void gpio::SetHI() {
	HAL_GPIO_WritePin(type, init.Pin, GPIO_PIN_SET);
}

void gpio::SetLO() {
	HAL_GPIO_WritePin(type, init.Pin, GPIO_PIN_RESET);
}

