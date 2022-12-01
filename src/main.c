/* Standard libraries */
#include <stdio.h>

/* Hardware support */
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>

/* Logging */
#define LOGGER_LOCAL_LEVEL (eLOGGER_Level_Info)
#include <logger.h>

/* Declare prototype for systick handler */
void sys_tick_handler(void);

/* Define logger tag */
static const char* TAG = "main";

/* Millisecond counter */
volatile uint32_t uiMillis;

void sys_tick_handler(void)
{
	uiMillis++;
}

static void msleep(uint32_t uiDelay)
{
	uint32_t uiWake = uiMillis + uiDelay;
	while (uiWake > uiMillis);
}

int main(void)
{
	/* Set PLL to 72Mhz....full speed ahead! */
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* 72MHz / 8 => 9000000 counts per second */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

	/* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
	/* SysTick interrupt every N clock pulses: set reload to N-1 */
	systick_set_reload(9000 - 1);

	/* Enable interrupt and start counting */
	systick_interrupt_enable();
	systick_counter_enable();

	/* Welcome user */
	printf("Welcome from printf!\n");
	LOGGER_W(TAG, "Welcome from LOGGER!");

	/* Show off our counting skills */
	for (int i = 0; /* no condition */ ; i++)
	{
		LOGGER_I(TAG, "Value: %i", i);
		msleep(500);
	}
}
