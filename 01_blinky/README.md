# 01 Blinky

A minimal example that blinks an LED using GPIO.

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
