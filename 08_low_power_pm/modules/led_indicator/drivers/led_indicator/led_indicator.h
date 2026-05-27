#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H

#include <zephyr/device.h>

#define LED_INDICATOR_DEV DEVICE_DT_GET(DT_NODELABEL(led_indicator_0))

#endif /* LED_INDICATOR_H */
