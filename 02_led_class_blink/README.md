# 02 LED Class Blink

Blinks an LED using Zephyr's LED class API instead of raw GPIO.

## Build, Flash, and Run

```bash
export ZEPHYR_BASE=~/zephyrproject/zephyr
west build --pristine --board stm32f3_disco .
west flash
```

## Interesting Files

| File | Description |
|------|-------------|
| `build/zephyr/zephyr.dts` | The fully resolved device tree, with all includes and overlays applied |
| `build/zephyr/include/generated/zephyr/devicetree_generated.h` | The generated C macros for the device tree, used by things like `DT_ALIAS` |

## LED Class Notes

Using the LED class is convenient because you don't need to manually configure the GPIO as an output — that is handled during board initialisation by the LED driver. It also abstracts away active-high vs. active-low polarity, so the LED class functions work the same regardless of how the GPIO is wired.

That said, there is no toggle function, so you have to track LED state yourself. The LED class also appears to be more oriented towards dedicated LED driver hardware than towards GPIOs driving LEDs directly.
