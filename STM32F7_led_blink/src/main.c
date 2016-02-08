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

/**************/
/*** led_on ***/
/**************/
static void led_on() {
	HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
}

/***************/
/*** led_off ***/
/***************/
static void led_off() {
	HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
}

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

	led_off();
}

/*************/
/*** pause ***/
/*************/
static void pause() {
	unsigned int i;

	for (i = 0; i < 1000000; i++)
		;
}

/************/
/*** main ***/
/************/
int main(void) {
	/* Init LED GPIO */
	init_led();

	for (;;) {
		/* Switch on the led */
		led_on();
		pause();

		/* Switch off the led */
		led_off();
		pause();
	}

	/* NotReached */
	return 0;
}
