/*
 * timer.c
 *
 *  Created on: Sep 29, 2025
 *      Author: Sunbeam
 */

#include "timer.h"
int flag = 0;
void TimerInit(uint32_t ms) {
	// Enable Timer APB clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	// Set the Prescalar
	TIM9->PSC = PR - 1;
	// calculate number of clocks to count
	uint32_t cnt = (FPCLK / 1000) * ms / PR;
	// set max count in ARR
	TIM9->ARR = cnt - 1;
	// start counting from zero
	TIM9->CNT = 0;
	// enable the timer interrupt in peripheral
	TIM9->DIER |= TIM_DIER_UIE;
	// enable the timer interrupt in NVIC
	NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
	// enable timer clock (mandatory) and set APRE (optional)
	TIM9->CR1 |= TIM_CR1_CEN;
}

// implement Interrupt Handler for TIM6
void TIM1_BRK_TIM9_IRQHandler(void) {
	// check if TIM6 interrupt occurred.
	if(TIM9->SR & TIM_SR_UIF) {

		flag = 1;
		// Clear/Ack TIM6 interrupt
		TIM9->SR &= ~TIM_SR_UIF;
	}
}


