/*
 * Accelerometer tilt LED indicator
 * STM32F3DISCO - Zephyr port of FreeRTOS version
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define TILT_THRESHOLD 2

/* LED GPIO specs from devicetree - compass directions */
static const struct gpio_dt_spec leds[] = {
	GPIO_DT_SPEC_GET(DT_ALIAS(led_n), gpios),  /* N  - LD3 PE9  */
	GPIO_DT_SPEC_GET(DT_ALIAS(led_ne), gpios), /* NE - LD5 PE10 */
	GPIO_DT_SPEC_GET(DT_ALIAS(led_e), gpios),  /* E  - LD7 PE11 */
	GPIO_DT_SPEC_GET(DT_ALIAS(led_se), gpios), /* SE - LD9 PE12 */
	GPIO_DT_SPEC_GET(DT_ALIAS(led_s), gpios),  /* S  - LD10 PE13 */
	GPIO_DT_SPEC_GET(DT_ALIAS(led_sw), gpios), /* SW - LD8 PE14 */
	GPIO_DT_SPEC_GET(DT_ALIAS(led_w), gpios),  /* W  - LD6 PE15 */
	GPIO_DT_SPEC_GET(DT_ALIAS(led_nw), gpios), /* NW - LD4 PE8  */
};

static void drdy_handler(const struct device *dev, const struct sensor_trigger *trig)
{
	struct sensor_value accel[3];

	sensor_sample_fetch(dev);
	sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);

	/* Use integer part of sensor_value for threshold comparison */
	int16_t ax = (int16_t)accel[0].val1;
	int16_t ay = -(int16_t)accel[1].val1;
	int16_t absx = ax < 0 ? -ax : ax;
	int16_t absy = ay < 0 ? -ay : ay;
	int16_t mag = absx > absy ? absx : absy;

	/* Turn all LEDs off */
	for (int i = 0; i < ARRAY_SIZE(leds); i++) {
		gpio_pin_set_dt(&leds[i], 0);
	}

	if (mag < TILT_THRESHOLD) {
		return;
	}

	/* Same sector logic as FreeRTOS version */
	int sector;
	if (absx <= absy / 2 && ay > 0) {
		sector = 0; /* N  */
	} else if (absy <= absx / 2 && ax > 0) {
		sector = 2; /* E  */
	} else if (absx <= absy / 2 && ay < 0) {
		sector = 4; /* S  */
	} else if (absy <= absx / 2 && ax < 0) {
		sector = 6; /* W  */
	} else if (ax > 0 && ay > 0) {
		sector = 1; /* NE */
	} else if (ax > 0 && ay < 0) {
		sector = 3; /* SE */
	} else if (ax < 0 && ay < 0) {
		sector = 5; /* SW */
	} else {
		sector = 7; /* NW */
	}

	gpio_pin_set_dt(&leds[sector], 1);
}

int main(void)
{
	const struct device *accel = DEVICE_DT_GET(DT_ALIAS(accel0));

	/* Initialise LED GPIOs */
	for (int i = 0; i < ARRAY_SIZE(leds); i++) {
		gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT_INACTIVE);
	}

	/* Register DRDY trigger */
	struct sensor_trigger trig = {
		.type = SENSOR_TRIG_DATA_READY,
		.chan = SENSOR_CHAN_ACCEL_XYZ,
	};
	sensor_trigger_set(accel, &trig, drdy_handler);

	return 0;
}
