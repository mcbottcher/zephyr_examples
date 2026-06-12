#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>

#define DEFAULT_PERIOD_MS 1000

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(my_led), gpios);

static atomic_t blink_period_ms = ATOMIC_INIT(DEFAULT_PERIOD_MS);

static int cmd_led_period(const struct shell *sh, size_t argc, char **argv)
{
	char *end;
	long period = strtol(argv[1], &end, 10);

	if (*end != '\0' || period <= 0 || period > INT32_MAX) {
		shell_error(sh, "period must be a positive integer (ms)");
		return -EINVAL;
	}

	atomic_set(&blink_period_ms, (atomic_val_t)period);
	shell_print(sh, "LED blink period set to %ld ms", period);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(led_cmds,
			       SHELL_CMD_ARG(period, NULL,
					     "Set LED blink period in ms: period <ms>",
					     cmd_led_period, 2, 0),
			       SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(led, &led_cmds, "LED commands", NULL);

int main(void)
{
	if (!gpio_is_ready_dt(&led)) {
		printk("LED GPIO not ready\n");
		return 0;
	}

	int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		printk("Failed to configure LED GPIO\n");
		return 0;
	}

	printk("Device shell example ready. Default blink period: %d ms\n", DEFAULT_PERIOD_MS);
	printk("Use 'led period <ms>' to change the blink rate.\n");
	printk("Use 'device list' to list all devices.\n");

	while (1) {
		gpio_pin_toggle_dt(&led);
		/* atomic_get ensures the compiler always reads the value from memory each iteration
		 */
		k_msleep(atomic_get(&blink_period_ms));
	}

	return 0;
}
