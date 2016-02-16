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
#include "stdio.h"

#define BUFFER_SIZE	64
#define TIMEOUT		10000
#define VREF		3.3F

static UART_HandleTypeDef hUART;
static ADC_HandleTypeDef hADC;

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

	/* Configure GPIO alternate function for RX and TX pins */
	gpioInitStructure.Pin = DISCOVERY_COM1_TX_PIN;
	gpioInitStructure.Mode = GPIO_MODE_AF_PP;
	gpioInitStructure.Alternate = DISCOVERY_COM1_TX_AF;
	gpioInitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DISCOVERY_COM1_TX_GPIO_PORT, &gpioInitStructure);
}

/****************************************************/
/***                HAL_ADC_MspInit               ***/
/****************************************************/
/* Note: This function is called by the HAL library */
/****************************************************/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hADC) {
	GPIO_InitTypeDef gpioInitStructure;

	/* Enable ADC interface clock */
	__HAL_RCC_ADC1_CLK_ENABLE();

	/* Enable clock for GPIOA */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* Configure GPIO PA0 pin */
	gpioInitStructure.Pin = GPIO_PIN_0;
	gpioInitStructure.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &gpioInitStructure);
}

/*****************/
/*** UART_init ***/
/*****************/
static void UART_init() {
	hUART.Instance = DISCOVERY_COM1;
	hUART.Init.BaudRate = 9600;
	hUART.Init.WordLength = UART_WORDLENGTH_8B;
	hUART.Init.StopBits = UART_STOPBITS_1;
	hUART.Init.Parity = UART_PARITY_NONE;
	hUART.Init.Mode = UART_MODE_TX;
	hUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&hUART);
}

/****************/
/*** ADC_init ***/
/****************/
static void ADC_init() {
	ADC_ChannelConfTypeDef channelConfStruct;

	hADC.Instance = ADC1;
	hADC.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hADC.Init.Resolution = ADC_RESOLUTION_12B;
	hADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hADC.Init.ScanConvMode = DISABLE;
	hADC.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hADC.Init.ContinuousConvMode = DISABLE;
	hADC.Init.DMAContinuousRequests = DISABLE;
	hADC.Init.NbrOfConversion = 1;
	hADC.Init.DiscontinuousConvMode = DISABLE;
	hADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	HAL_ADC_Init(&hADC);

	channelConfStruct.Channel = ADC_CHANNEL_0;
	channelConfStruct.Rank = 1;
	channelConfStruct.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	channelConfStruct.Offset = 0;
	HAL_ADC_ConfigChannel(&hADC, &channelConfStruct);
}

/**************************/
/*** UART_write_voltage ***/
/**************************/
static void UART_write_voltage(float voltage) {
	static char buffer[BUFFER_SIZE];
	int n;

	n = snprintf(buffer, BUFFER_SIZE, "\r%-0.2fV", voltage);
	HAL_UART_Transmit(&hUART, (uint8_t *)buffer, n, TIMEOUT);
}

/************/
/*** Main ***/
/************/
int main() {
	float voltage = 0;

	/*** Init HAL library ***/
	HAL_Init();

	/*** Init UART ***/
	UART_init();

	/*** Init ADC ***/
	ADC_init();

	/*** Do forever ***/
	for(;;) {
		HAL_ADC_Start(&hADC);
		if (HAL_ADC_PollForConversion(&hADC, TIMEOUT) == HAL_OK) {
			voltage = (float)HAL_ADC_GetValue(&hADC) / 4095 * VREF;
			UART_write_voltage(voltage);
		}
		HAL_ADC_Stop(&hADC);

		HAL_Delay(1000);
	}
}
