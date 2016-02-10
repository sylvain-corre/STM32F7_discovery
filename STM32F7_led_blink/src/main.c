/**
 ******************************************************************************
 * @file    main.c
 * @author  Sylvain Corré
 * @version V1.0
 * @date    07/02/2016
 * @brief   Blink led example.
 ******************************************************************************
 */

#include "stm32f7xx.h"
#include "stm32746g_discovery.h"

/****************/
/*** init_led ***/
/****************/
static void init_led() {
	GPIO_InitTypeDef gpioInitStructure;

	LED1_GPIO_CLK_ENABLE();

	gpioInitStructure.Pin = LED1_PIN;
	gpioInitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitStructure.Pull = GPIO_PULLUP;
	gpioInitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(LED1_GPIO_PORT, &gpioInitStructure);

	/* Switch off led */
	HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
}

/************/
/*** main ***/
/************/
int main(void) {
	/*** Init ***/
	HAL_Init();

	/* Init LED GPIO */
	init_led();

	for (;;) {
		/* Switch led state */
		HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);

		/* Wait 1s */
		HAL_Delay(1000);
	}

	/* NotReached */
	return 0;
}
