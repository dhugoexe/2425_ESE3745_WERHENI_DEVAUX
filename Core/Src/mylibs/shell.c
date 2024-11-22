/*
 * shell.c
 *
 *  Created on: Oct 1, 2023
 *      Author: nicolas
 */
#include "usart.h"
#include "mylibs/shell.h"
#include <stdio.h>
#include <string.h>
#include "tim.h"
#include "adc.h"

uint8_t prompt[]="user@Nucleo-STM32G474RET6>>";
uint8_t started[]=
		"\r\n*-----------------------------*"
		"\r\n| Welcome on Nucleo-STM32G474 |"
		"\r\n*-----------------------------*"
		"\r\n";
uint8_t newline[]="\r\n";
uint8_t backspace[]="\b \b";
uint8_t cmdNotFound[]="Command not found\r\n";
uint8_t brian[]="Brian is in the kitchen\r\n";
uint8_t uartRxReceived;
uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];
uint16_t speed_value;
uint8_t speed[50];

char	 	cmdBuffer[CMD_BUFFER_SIZE];
int 		idx_cmd;
char* 		argv[MAX_ARGS];
int		 	argc = 0;
char*		token;
int 		newCmdReady = 0;
static int currentSpeed =50;


#define MAX_SPEED 99
#define MIN_SPEED 1
#define DUTY_CYCLE_50 50

extern Current_Measure_t currentMeasure;
uint8_t conversionComplete =0;

float offset_current=2.5268;

static volatile uint16_t adc_buffer[ADC_BUF_SIZE];
static volatile uint16_t adcValue = 0;

int setSpeed(uint8_t speed)
{
	if (speed >= currentSpeed)
	{
		while(currentSpeed != speed)
		{
			htim1.Instance->CCR1=(4250-1)*currentSpeed/100;
			htim1.Instance->CCR2=(4250-1)*(100-currentSpeed)/100;
			currentSpeed++;
			HAL_Delay(100);

		}
	}
	else if (speed <= currentSpeed)
	{
		while(currentSpeed != speed)
		{
			htim1.Instance->CCR1=(4250-1)*currentSpeed/100;
			htim1.Instance->CCR2=(4250-1)*(100-currentSpeed)/100;
			currentSpeed--;
			HAL_Delay(100);
		}
	}
	return currentSpeed;

}

void setPWM(int speed)
{
	if (speed >= currentSpeed)
	{
		for (int i = currentSpeed; i < speed; i++)
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, i);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 100 - i);
			HAL_Delay(50);
		}
	}
	else
	{
		for (int i = currentSpeed; i > speed; i--)
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, i);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 100 - i);
			HAL_Delay(50);
		}
	}
}


void Shell_Init(void){
	memset(argv, NULL, MAX_ARGS*sizeof(char*));
	memset(cmdBuffer, NULL, CMD_BUFFER_SIZE*sizeof(char));
	memset(uartRxBuffer, NULL, UART_RX_BUFFER_SIZE*sizeof(char));
	memset(uartTxBuffer, NULL, UART_TX_BUFFER_SIZE*sizeof(char));

	HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
	HAL_UART_Transmit(&huart2, started, strlen((char *)started), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, prompt, strlen((char *)prompt), HAL_MAX_DELAY);
}

void Shell_Loop(void){
	if(uartRxReceived){
		switch(uartRxBuffer[0]){
		case ASCII_CR: // Nouvelle ligne, instruction à traiter
			HAL_UART_Transmit(&huart2, newline, sizeof(newline), HAL_MAX_DELAY);
			cmdBuffer[idx_cmd] = '\0';
			argc = 0;
			token = strtok(cmdBuffer, " ");
			while(token!=NULL){
				argv[argc++] = token;
				token = strtok(NULL, " ");
			}
			idx_cmd = 0;
			newCmdReady = 1;
			break;
		case ASCII_BACK: // Suppression du dernier caractère
			cmdBuffer[idx_cmd--] = '\0';
			HAL_UART_Transmit(&huart2, backspace, sizeof(backspace), HAL_MAX_DELAY);
			break;

		default: // Nouveau caractère
			cmdBuffer[idx_cmd++] = uartRxBuffer[0];
			HAL_UART_Transmit(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
		}
		uartRxReceived = 0;
	}

	if(newCmdReady){
		if(strcmp(argv[0],"WhereisBrian?")==0){
			HAL_UART_Transmit(&huart2, brian, sizeof(brian), HAL_MAX_DELAY);
		}
		else if(strcmp(argv[0],"help")==0){
			int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Print all available functions here\r\n");
			HAL_UART_Transmit(&huart2, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
		}

		else if (strcmp(argv[0],"speed")==0)
		{

			speed_value=atoi(argv[1]);

			if (speed_value>MAX_SPEED)
			{
				speed_value=MAX_SPEED;
			}
			else if (speed_value<MIN_SPEED)
			{
				speed_value=MIN_SPEED;
			}


			currentSpeed = setSpeed(speed_value);
			//setPWM(speed_value);
			//htim1.Instance->CCR1=(4250-1)*speed_value/100;
			//htim1.Instance->CCR2=(4250-1)*(100-speed_value)/100;
			sprintf(speed,"Speed is changed to %i \r\n",speed_value);
			HAL_UART_Transmit(&huart2, speed, sizeof(speed), HAL_MAX_DELAY);
		}
		else if (strcmp(argv[0],"stop")==0)
		{

			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
			HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
			//HAL_TIM_Base_Stop_IT(&htim1);
		}
		else if (strcmp(argv[0],"start")==0)
		{

			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
			//HAL_TIM_Base_Stop_IT(&htim1);

			htim1.Instance->CCR1=(4250-1)*DUTY_CYCLE_50/100;
			htim1.Instance->CCR2=(4250-1)*DUTY_CYCLE_50/100;

			currentSpeed = 50;
		}
		else if (strcmp(argv[0],"current")==0)
		{
			/* Partie polling
			HAL_ADC_Start(&hadc1);
			if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY ) == HAL_OK) {
				adcValue = HAL_ADC_GetValue(&hadc1); // Lit la valeur de l'ADC
				float u = (3.3*adcValue)/ADC_RESOLUTION;
				float i=(u-1.65)/0.05;

				sprintf(uartTxBuffer,"Current: %.2f \r\n", i);
				HAL_UART_Transmit(&huart2, uartTxBuffer, sizeof(uartTxBuffer), HAL_MAX_DELAY);
			}
			*/


			sprintf(uartTxBuffer,"Calculating current... \r\n");
			HAL_UART_Transmit(&huart2, uartTxBuffer, sizeof(uartTxBuffer), HAL_MAX_DELAY);
			memset(uartRxBuffer, NULL, UART_RX_BUFFER_SIZE*sizeof(char));

			//HAL_TIM_Base_Start(&htim1);
			HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
			memset(adc_buffer, NULL, ADC_BUF_SIZE*sizeof(char));
			if (HAL_OK != HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, ADC_BUF_SIZE))
			{
				sprintf(uartTxBuffer,"HAL_ERROR \r\n");
				HAL_UART_Transmit(&huart2, uartTxBuffer, sizeof(uartTxBuffer), HAL_MAX_DELAY);
				Error_Handler();
			}else{
				sprintf(uartTxBuffer,"HAL_OK \r\n");
				HAL_UART_Transmit(&huart2, uartTxBuffer, sizeof(uartTxBuffer), HAL_MAX_DELAY);
			}

			while(!conversionComplete){}
			sprintf(uartTxBuffer,"Fin while \r\n");
							HAL_UART_Transmit(&huart2, uartTxBuffer, sizeof(uartTxBuffer), HAL_MAX_DELAY);
			float u = (3.3*adcValue)/ADC_RESOLUTION;
			float i=(u-1.65)/0.05;
			memset(uartRxBuffer, NULL, UART_RX_BUFFER_SIZE*sizeof(char));
			sprintf(uartTxBuffer,"current: %.3f \r\n", i);
			HAL_UART_Transmit(&huart2, uartTxBuffer, sizeof(uartTxBuffer), HAL_MAX_DELAY);
			conversionComplete = 0;




		}


		else{
			HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
		}
		HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);
		newCmdReady = 0;
	}
}

/*
void Encoder_Init(void)
{

    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);

    lastCounter = __HAL_TIM_GET_COUNTER(&htim1);
    lastTime = HAL_GetTick();
}

float Encoder_GetFilteredSpeed(void)
{

    float raw_speed = Encoder_GetSpeed();

    speed_buffer[buffer_index] = raw_speed;
    buffer_index = (buffer_index + 1) % FILTER_SIZE;

    float sum = 0;
    for(uint8_t i = 0; i < FILTER_SIZE; i++)
    {
        sum += speed_buffer[i];
    }

    return sum / FILTER_SIZE;
}
*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart){
	uartRxReceived = 1;
	HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1)
	{
		sprintf(uartTxBuffer,"CC \r\n");
		HAL_UART_Transmit(&huart2, uartTxBuffer, sizeof(uartTxBuffer), HAL_MAX_DELAY);
		uint32_t sum = 0;
		for (uint16_t i = 0; i < ADC_BUF_SIZE; i++)
		{
		sum += adc_buffer[i];
		}
		adcValue = sum / ADC_BUF_SIZE;

		conversionComplete = 1;
		HAL_ADC_Stop_DMA(&hadc1);
	}
}

