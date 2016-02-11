/**
 ******************************************************************************
 * @file    main.c
 * @author  Sylvain Corré
 * @version V1.0
 * @date    10/02/2016
 * @brief   UART test. The used UART is connected to the ST/LINK USB port acting
 * 			as a virtual COM port. A serial terminal software (e.g. Putty)
 * 			can be used to interact with the UART. On Linux, the device shows
 * 			up as /dev/ttyACM0.
 ******************************************************************************
 */

#include "stm32f7xx.h"
#include "stm32746g_discovery.h"
#include "string.h"

#define BUFFER_SIZE	64
#define TIMEOUT		10000

/****************************************************/
/***               HAL_UART_MspInit               ***/
/****************************************************/
/* Note: This function is called by the HAL library */
/****************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *hUART) {
	GPIO_InitTypeDef gpioInitStructure;

	/* Enable USART1 interface clock */
	DISCOVERY_COM1_CLK_ENABLE();

	/* Enable clock for UART GPIOs (RX and TX pins) */
	DISCOVERY_COM1_TX_GPIO_CLK_ENABLE();
	DISCOVERY_COM1_RX_GPIO_CLK_ENABLE();

	/* Configure GPIO alternate function for RX and TX pins */
	gpioInitStructure.Pin = DISCOVERY_COM1_TX_PIN;
	gpioInitStructure.Mode = GPIO_MODE_AF_PP;
	gpioInitStructure.Alternate = DISCOVERY_COM1_TX_AF;
	gpioInitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DISCOVERY_COM1_TX_GPIO_PORT, &gpioInitStructure);

	gpioInitStructure.Pin = DISCOVERY_COM1_RX_PIN;
	gpioInitStructure.Mode = GPIO_MODE_AF_PP;
	gpioInitStructure.Alternate = DISCOVERY_COM1_RX_AF;
	gpioInitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DISCOVERY_COM1_RX_GPIO_PORT, &gpioInitStructure);
}

/*****************/
/*** UART_init ***/
/*****************/
static void UART_init(UART_HandleTypeDef *hUART) {
	hUART->Instance = DISCOVERY_COM1;
	hUART->Init.BaudRate = 9600;
	hUART->Init.WordLength = UART_WORDLENGTH_8B;
	hUART->Init.StopBits = UART_STOPBITS_1;
	hUART->Init.Parity = UART_PARITY_NONE;
	hUART->Init.Mode = UART_MODE_TX_RX;
	hUART->Init.HwFlowCtl = UART_HWCONTROL_NONE;

	HAL_UART_Init(hUART);
}

/******************/
/*** UART_write ***/
/******************/
static void UART_write(UART_HandleTypeDef *hUART, const char *msg) {
	HAL_UART_Transmit(hUART, (uint8_t *)msg, strlen(msg), TIMEOUT);
}

/*****************/
/*** UART_read ***/
/*****************/
static unsigned int UART_read(UART_HandleTypeDef *hUART, char *buffer, unsigned int size) {
	unsigned int num_chars;
	char c;

	for (num_chars = 0; num_chars < size - 1; ) {
		if (HAL_UART_Receive(hUART, (uint8_t *)&c, 1, TIMEOUT) == HAL_OK) {
			HAL_UART_Transmit(hUART, (uint8_t *)&c, 1, TIMEOUT);

			if (c == '\r') {
				break;
			} else if (c != '\n') {
				buffer[num_chars++] = c;
			}
		}
	}

	buffer[num_chars] = '\0';

	return num_chars;
}

/************/
/*** Main ***/
/************/
int main() {
	UART_HandleTypeDef hUART;
	char buffer[BUFFER_SIZE];

	/*** Init HAL library ***/
	HAL_Init();

	/*** Init UART ***/
	UART_init(&hUART);

	/*** Do forever ***/
	for(;;) {
		/* Send message */
		UART_write(&hUART, "Please enter your name: ");

		/* Read name */
		if (UART_read(&hUART, buffer, BUFFER_SIZE)) {
			/* Write welcome message */
			UART_write(&hUART, "\r\nHello ");
			UART_write(&hUART, buffer);
			UART_write(&hUART, " !\r\n");
		}
	}
}
