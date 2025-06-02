#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/regs/intctrl.h"
#include "pico/time.h"

// Shutter (venturi) sequence for rotation away from limit switch.
// This is looking at the gear and the rotation is clockwise. The plug is facing you (attachment clip is down).
// Each full coil has a resistance of 38ohms.
// Step 1: coil 1 - 1 (Top left contact)
// Step 2: coil 2 - 1 (Bottom left contact)
// Step 3: coil 1 - 2 (Top right contact)
// Step 4: coil 2 - 2 (Bottom right conact)

/** The GPIO pin used for shutter (venturi) unipolar stepper - coil 1 - section 1. */
#define SHUTTER_COIL_1_1_GPIO 2

/** The GPIO pin used for shutter (venturi) unipolar stepper - coil 1 - section 2. */
#define SHUTTER_COIL_1_2_GPIO 3

/** The GPIO pin used for shutter (venturi) unipolar stepper - coil 2 - section 1. */
#define SHUTTER_COIL_2_1_GPIO 4

/** The GPIO pin used for shutter (venturi) unipolar stepper - coil 2 - section 2. */
#define SHUTTER_COIL_2_2_GPIO 5

/** State of shutter coil 1 - section 1. Energised is true. */
bool shutter_coil_1_1;

/** State of shutter coil 1 - section 2. Energised is true. */
bool shutter_coil_1_2;

/** State of shutter coil 2 - section 1. Energised is true. */
bool shutter_coil_2_1;

/** State of shutter coil 2 - section 2. Energised is true. */
bool shutter_coil_2_2;

/**
 * Program for Pi Pico that does Toyota Landcruiser HDJ100 specific things.
 * Mechanism 1: Emulate shutter (venturi) stepper motor limit switch so shutter can be removed.
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

	// Setup shutter stepper motor state reading.

	gpio_init(SHUTTER_COIL_1_1_GPIO);
	gpio_set_dir(SHUTTER_COIL_1_1_GPIO, GPIO_IN);
	shutter_coil_1_1 = gpio_get(SHUTTER_COIL_1_1_GPIO);

	gpio_init(SHUTTER_COIL_1_2_GPIO);
	gpio_set_dir(SHUTTER_COIL_1_2_GPIO, GPIO_IN);
	shutter_coil_1_2 = gpio_get(SHUTTER_COIL_1_2_GPIO);

	gpio_init(SHUTTER_COIL_2_1_GPIO);
	gpio_set_dir(SHUTTER_COIL_2_1_GPIO, GPIO_IN);
	shutter_coil_2_1 = gpio_get(SHUTTER_COIL_2_1_GPIO);

	gpio_init(SHUTTER_COIL_2_2_GPIO);
	gpio_set_dir(SHUTTER_COIL_2_2_GPIO, GPIO_IN);
	shutter_coil_2_2 = gpio_get(SHUTTER_COIL_2_2_GPIO);

	// Main processing loop.
	while(1)
	{
		bool prev_shutter_coil_1_1 = shutter_coil_1_1;
		bool prev_shutter_coil_1_2 = shutter_coil_1_2;
		bool prev_shutter_coil_2_1 = shutter_coil_2_1;
		bool prev_shutter_coil_2_2 = shutter_coil_2_2;

		shutter_coil_1_1 = gpio_get(SHUTTER_COIL_1_1_GPIO);
		shutter_coil_1_2 = gpio_get(SHUTTER_COIL_1_2_GPIO);
		shutter_coil_2_1 = gpio_get(SHUTTER_COIL_2_1_GPIO);
		shutter_coil_2_2 = gpio_get(SHUTTER_COIL_2_2_GPIO);

		if(prev_shutter_coil_1_1 != shutter_coil_1_1 ||
			prev_shutter_coil_1_2 != shutter_coil_1_2 ||
			prev_shutter_coil_2_1 != shutter_coil_2_1 ||
			prev_shutter_coil_2_2 != shutter_coil_2_2)
		{
			printf("%u - %u - %u - %u", shutter_coil_1_1, shutter_coil_1_2, shutter_coil_2_1, shutter_coil_2_2);
		}
	}
}
