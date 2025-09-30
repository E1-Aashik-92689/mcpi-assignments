/*
 * timer.c
 *
 *  Created on: Apr 4, 2025
 *      Author: admin
 */

#include "timer.h"

void TimerPwmInit(void)
{
	// config PA8 as TIM8 CH1
	// enable PC clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// set PC6 mode as Alt Fn (10)
	GPIOA->MODER |= BV(2 * 8 + 1);
	GPIOA->MODER &= ~BV(2 * 8);
	// disable pull-up and pull-down regrs
	GPIOA->PUPDR &= ~(BV(2 * 8) | BV(2 * 8 + 1));
	// set alt fn "3" as TIM8
	GPIOA->AFR[1] |= BV(0);

	// PWM config
	// 0. Enable TIM1 clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	// 1. Setup PWM clock (10 KHz to 100 KHz). Program the period (ARR) and the duty cycle (CCR) respectively in ARR and CCRx registers.
	//    - PCLK = 16MHz, PSC = 16 --> TCLK = 1MHz
	//    - ARR = 100 => PWM output freq = TCLK / ARR = 1 MHz / 100 = 10 KHz
	//    - CCR = 50 => 50% Duty Cycle
	TIM1->PSC = TIM_PR - 1;
	TIM1->ARR = 100 - 1;
	TIM1->CCR1 = 0;
	// 2. Configure the output pin:
	//    - Select the output mode by writing CCS bits (00 = output) in CCMRx register.
	//    - Select the polarity by writing the CCxP bit (0 = active high) in CCER register.
	TIM1->CCMR1 &= ~(TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC1S_1);
	TIM1->CCER &= ~TIM_CCER_CC1P;
	// 3. Select the PWM mode (PWM1 or PWM2) by writing OCxM bits in CCMRx register.
	//    - The PWM mode can be selected independently on each channel (one PWM per OCx output) by writing 110 (PWM mode 1) or 111 (PWM mode 2) in the TIMx_CCMRx register.
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // PWM Mode1
	// 4. Set the preload bit in CCMRx register and the ARPE bit in the CR1 register.
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE;
	TIM1->CR1 |= TIM_CR1_ARPE;
	// 5. Select the counting mode in CR1:
	//    - PWM edge-aligned mode (00): the counter must be configured up-counting or down-counting.
	//    - PWM center aligned mode: the counter mode must be center aligned counting mode (CMS bits different from '00').
	TIM1->CR1 &= ~(TIM_CR1_CMS_0 | TIM_CR1_CMS_1);
	// 6. Enable the capture compare in CCER.
	TIM1->CCER |= TIM_CCER_CC1E;
	// 7. Enable main output in BDTR and Enable the counter.
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM1->CR1 |= TIM_CR1_CEN;
}
