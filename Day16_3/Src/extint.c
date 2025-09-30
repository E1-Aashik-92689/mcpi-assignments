/*
 * extint.c
 *
 *  Created on: Sep 16, 2025
 *      Author: sunbeam
 */

#include "stm32f4xx.h"

volatile int ext_flag = 0;

void extint_init(void)
{
	//1. Enable clock for GPIOA
	RCC->AHB1ENR |= BV(0);

	//2. Configure GPIOA0 as input
	GPIOA->MODER &= ~(BV(0) | BV(1));
	GPIOA->PUPDR &= ~(BV(0) | BV(1));

	//3. Configure external interrupt EXTI0 into system configuration (stm)
	SYSCFG->EXTICR[0] &= ~(BV(0) | BV(1) | BV(2) | BV(3));

	//4. Configure external interrupt EXTI0 into external circuit
	EXTI->IMR |= BV(0);
	EXTI->RTSR |= BV(0);

	//5. Configure external interrupt EXTI0 into NVIC
	NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void)
{
	//1. clear pending bit of EXTI0 in PR
	EXTI->PR |= BV(0);
	//2. take action
	ext_flag = 1;

}









