/*
 * timer.c
 *
 *  Created on: Sep 29, 2025
 *      Author: Sunbeam
 */

#include "timer.h"

void TimerInit(void) {
	// Enable Timer APB clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	// Set the Prescalar
	TIM7->PSC = PR - 1;
}

void TimerDelayMs(uint32_t ms) {
	// calculate number of clocks to count
	uint32_t cnt = (FPCLK / 1000) * ms / PR;
	// set max count in ARR
	TIM7->ARR = cnt - 1;
	// start counting from zero
	TIM7->CNT = 0;
	// enable timer clock (mandatory) and set APRE (optional)
	TIM7->CR1 |= TIM_CR1_CEN;
	// wait for update event SR -- UIF flag
	while(!(TIM7->SR & TIM_SR_UIF));
	// clear update flag in SR
	TIM7->SR &= ~TIM_SR_UIF;
	// stop timer clock
	TIM7->CR1 &= ~TIM_CR1_CEN;
}

