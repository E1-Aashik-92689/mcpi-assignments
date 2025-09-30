#include "systick.h"
uint32_t xTicks = 0;
void systick_init(void)
{
	SysTick_Config(SystemCoreClock/1000);
}//SystemCoreClock = 16000000

void SysTick_DelayMs(uint32_t ms)
{
	uint32_t cur = xTicks;
	uint32_t wait = cur+ms;
	while(xTicks < wait);

}
void SysTick_Handler(void)
{
	xTicks++;
}
