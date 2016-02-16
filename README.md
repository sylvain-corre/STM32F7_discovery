# STM32F7_discovery
Examples for the STM32F7 discovery board

This project aims at providing some examples of code for the STM32F7 discovery board (see http://www.st.com/stm32f7-discovery).
The examples have been developped using AC6 System Workbench (see http://www.openstm32.org/), and the corresponding project files are included.

The following examples are provided:
  - STM32F7_led_blink: This example shows how to make the green led blink
  - STM32F7_UART_test: This example shows how to configure and use the UART. It asks for a name and then display a welcome
  						message on the serial console (using the virtual COM port through the STLink USB port).
  - STM32F7_ADC_test: This example shows how to use the ADC. It outputs the voltage of the A0 Arduino connector pin
  						to the serial console (using the virtual COM port through the STLink USB port).
