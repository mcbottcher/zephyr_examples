#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define LED_INDICATOR_BLUE   DEVICE_DT_GET(DT_NODELABEL(led_indicator_blue))
#define LED_INDICATOR_ORANGE DEVICE_DT_GET(DT_NODELABEL(led_indicator_orange))

int main(void)
{
	if (!device_is_ready(LED_INDICATOR_BLUE) || !device_is_ready(LED_INDICATOR_ORANGE)) {
		printk("LED indicator device not ready\n");
		return 0;
	}

	printk("PM shell example ready — both LEDs are ON.\n");
	printk("Use 'device list' to see device names.\n");
	printk("Use 'pm device suspend led_indicator_blue' to turn blue LED off.\n");
	printk("Use 'pm device suspend led_indicator_orange' to turn orange LED off.\n");
	printk("Use 'pm device resume <name>' to turn a LED back on.\n");

	return 0;
}
