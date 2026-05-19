#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/dfu/mcuboot.h>

static const int32_t sleep_time_ms = 1000;

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(my_led), gpios);

int main(void)
{
	/* Mark this image as confirmed so MCUboot won't revert it on next reset */
	boot_write_img_confirmed();

	// Make sure that the GPIO was initialized
	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	// Set the GPIO as output
	int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	if (ret < 0) {
		return 0;
	}

	int state = 0;

	// Do forever
	while (1) {

		// Change the state of the pin and print
		state = !state;

		// Set pin state
		ret = gpio_pin_set_dt(&led, state);
		if (ret < 0) {
			return 0;
		}

		// Sleep
		k_msleep(sleep_time_ms);
	}

	return 0;
}
