#define DT_DRV_COMPAT example_led_indicator

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/pm/device.h>

struct led_indicator_config {
	struct gpio_dt_spec status_gpio;
};

static int led_indicator_pm_action(const struct device *dev, enum pm_device_action action)
{
	const struct led_indicator_config *cfg = dev->config;

	switch (action) {
	case PM_DEVICE_ACTION_RESUME:
		return gpio_pin_set_dt(&cfg->status_gpio, 1);
	case PM_DEVICE_ACTION_SUSPEND:
		return gpio_pin_set_dt(&cfg->status_gpio, 0);
	default:
		return -ENOTSUP;
	}
}

static int led_indicator_init(const struct device *dev)
{
	const struct led_indicator_config *cfg = dev->config;

	if (!gpio_is_ready_dt(&cfg->status_gpio)) {
		return -ENODEV;
	}

	return gpio_pin_configure_dt(&cfg->status_gpio, GPIO_OUTPUT_ACTIVE);
}

#define LED_INDICATOR_DEFINE(inst)                                                                 \
	static const struct led_indicator_config led_indicator_cfg_##inst = {                      \
		.status_gpio = GPIO_DT_SPEC_INST_GET(inst, status_gpios),                          \
	};                                                                                         \
                                                                                                   \
	PM_DEVICE_DT_INST_DEFINE(inst, led_indicator_pm_action);                                   \
                                                                                                   \
	DEVICE_DT_INST_DEFINE(inst, led_indicator_init, PM_DEVICE_DT_INST_GET(inst), NULL,         \
			      &led_indicator_cfg_##inst, POST_KERNEL,                              \
			      CONFIG_KERNEL_INIT_PRIORITY_DEVICE, NULL);

DT_INST_FOREACH_STATUS_OKAY(LED_INDICATOR_DEFINE)
