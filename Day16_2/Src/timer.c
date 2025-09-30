/*
 * timer.c
 *
 *  Created on: Sep 29, 2025
 *      Author: Sunbeam
 */

#include "timer.h"

int GC = 0;
void TimerInit(uint32_t ms) {
	// Enable Timer APB clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	// Set the Prescalar
	TIM7->PSC = PR - 1;
	// calculate number of clocks to count
	uint32_t cnt = (FPCLK / 1000) * ms / PR;
	// set max count in ARR
	TIM7->ARR = cnt - 1;
	// start counting from zero
	TIM7->CNT = 0;
	// enable the timer interrupt in peripheral
	TIM7->DIER |= TIM_DIER_UIE;
	// enable the timer interrupt in NVIC
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	// enable timer clock (mandatory) and set APRE (optional)
	TIM7->CR1 |= TIM_CR1_CEN;
}

// implement Interrupt Handler for TIM6
void TIM6_DAC_IRQHandler(void) {
	// check if TIM7 interrupt occurred.
	if(TIM7->SR & TIM_SR_UIF) {
		// Increment Global Count
		GC++;
		// Clear/Ack TIM7 interrupt
		TIM7->SR &= ~TIM_SR_UIF;
	}
}


