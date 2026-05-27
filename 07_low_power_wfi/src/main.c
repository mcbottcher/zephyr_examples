#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(my_led), gpios);

/* Called from the system workqueue after the gpio-keys debounce delay */
static void button_handler(struct input_event *evt, void *user_data)
{
	ARG_UNUSED(user_data);

	/* gpio-keys always configures GPIO_INT_EDGE_BOTH (see input_gpio_keys.c),
	 * so this callback fires on both press (value=1) and release (value=0).
	 * The value=1 guard below discards the release wake-up in software.
	 * For better low power performance, write a custom driver that configures
	 * a single edge interrupt so the CPU is not woken on release at all. */
	if (evt->code == INPUT_KEY_0 && evt->value == 1) {
		gpio_pin_toggle_dt(&led);
		printk("LED toggled\n");
	}
}

/* NULL as first arg means this receives events from all input devices. */
INPUT_CALLBACK_DEFINE(NULL, button_handler, NULL);

int main(void)
{
	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	/* Setup done — main exits. The gpio-keys driver owns the ISR and debounce.
	 * button_handler is called from the system workqueue on each press.
	 * The idle thread runs WFI whenever no other thread is scheduled. */
	return 0;
}
