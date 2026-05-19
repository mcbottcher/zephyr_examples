#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>

// Settings
static const int32_t sleep_time_ms = 1000;

static const struct led_dt_spec my_led = LED_DT_SPEC_GET(DT_ALIAS(my_led));

int main(void)
{
	// Make sure that the LED driver GPIO was initialized
	if (!led_is_ready_dt(&my_led)) {
		return 0;
	}

	// This shouldn't work since led_blink api is not registered by the gpio-leds driver
	// only set brightness is
	// int ret = led_blink(my_led.dev, my_led.index, sleep_time_ms, sleep_time_ms);

	int state = 0;

	while (1) {

		led_off(my_led.dev, my_led.index);
		k_msleep(sleep_time_ms);

		led_on(my_led.dev, my_led.index);
		k_msleep(sleep_time_ms);
	}

	return 0;
}
