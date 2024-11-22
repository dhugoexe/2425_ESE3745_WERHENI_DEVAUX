/*
 * shell.h
 *
 *  Created on: Oct 1, 2023
 *      Author: nicolas
 */

#ifndef INC_MYLIBS_SHELL_H_
#define INC_MYLIBS_SHELL_H_

#define UART_RX_BUFFER_SIZE 1
#define UART_TX_BUFFER_SIZE 64
#define CMD_BUFFER_SIZE 64
#define MAX_ARGS 9
#define ASCII_LF 0x0A			// LF = line feed, saut de ligne
#define ASCII_CR 0x0D			// CR = carriage return, retour chariot
#define ASCII_BACK 0x08			// BACK = Backspace

#define START_BYTE 0xA5
#define START_SCAN 0x60
#define STOP_SCAN 0x65
#define DEVICE_INFO 0x90
#define HEALTH_STATUS 0x91
#define ADC_VREF_MV 3300           // 3.3V en millivolts
#define ADC_RESOLUTION 4096        // 12-bit resolution
#define SHUNT_RESISTANCE_MOHM 100  // 0.1Ω = 100mΩ
#define GAIN_AMPLIFIER 20
#define ADC_BUF_SIZE 16

// Constantes pour éviter les calculs flottants
#define ADC_VREF_MV 3300    // 3.3V en millivolts
#define ADC_RESOLUTION 4096  // 12-bit resolution

void Shell_Init(void);

typedef struct {
	uint16_t adc_raw;
	uint32_t voltage_mv;
	uint32_t current_ma;
} Current_Measure_t;


#endif /* INC_MYLIBS_SHELL_H_ */
