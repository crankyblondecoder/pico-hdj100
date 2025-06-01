#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/regs/intctrl.h"
#include "pico/time.h"

/** The GPIO pin used for . */
#define _GPIO 1

/**
 * Program for Pi Pico that does Toyota Landcruiser HDJ100 specific things.
 */
int main()
{
	// Make sure disabled interrupts wake up WFE via the SEVONPEND flag of the SCR register.
	// See "M0PLUS: SCR Register" in RP2040 Datasheet.
	// SEVONPEND needs to be set. This allows WFE to wake up on pending interrupt even if disabled.
	uint32_t* scrRegAddr = (uint32_t*)(PPB_BASE + 0xed10);
	*scrRegAddr |= 0x10;

	// Must happen for serial stdout to work.
	stdio_init_all();

	// Push GPIO pin 23 (SMPS Mode) high to disable power saving, which causes noise in the ADC.
	gpio_init(23);
	gpio_set_dir(23, GPIO_OUT);
	gpio_put(23, true);

	// Main processing loop. Used for user interaction.
	while(1)
	{
	}
}
