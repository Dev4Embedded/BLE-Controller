#ifndef __GPIO_HPP__
#define __GPIO_HPP__

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_gpio.h"

class gpio {
private:
	GPIO_InitTypeDef init;
	GPIO_TypeDef* type;
public:
	gpio(GPIO_TypeDef* type, uint32_t pin, uint32_t mode, uint32_t pull,
	     uint32_t speed, uint32_t alternate);
	gpio(GPIO_TypeDef* type, uint32_t pin, uint32_t mode, uint32_t pull,
	     uint32_t speed);
	gpio(GPIO_TypeDef* type, uint32_t pin, uint32_t mode);
	~gpio();
	void Init();
	void SetHI();
	void SetLO();
};

#endif
