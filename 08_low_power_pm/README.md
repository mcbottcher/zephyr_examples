# 08 Low Power PM

Button-driven LED toggle using a GPIO interrupt, the system workqueue, and Zephyr's PM subsystem. The main thread exits after setup. A custom `led_indicator` device driver uses PM device actions to reflect the awake/idle state via the blue LED.

## How it works

1. Blue LED turns on at boot (the `led_indicator` driver initialises its GPIO active)
2. Button press fires a GPIO interrupt → ISR submits work to the system workqueue
3. Work handler toggles the red LED, then adds a PM policy latency constraint (`pm_policy_latency_request_add`) to signal that the system should stay awake, and explicitly resumes the `led_indicator` device (`PM_DEVICE_ACTION_RESUME` → blue LED on)
4. A 5-second delayable work item is (re)scheduled on every press
5. When the timer fires, the latency constraint is removed and the `led_indicator` device is explicitly suspended (`PM_DEVICE_ACTION_SUSPEND` → blue LED off)

## LED meaning

| LED | Meaning |
|-----|---------|
| Blue (PE8) | On while the awake constraint is active |
| Red (PE9) | Toggled on each button press |

## Device PM

The `led_indicator` driver (`modules/led_indicator/drivers/led_indicator/led_indicator.c`) is a minimal custom driver that exposes only PM device actions:

- `PM_DEVICE_ACTION_RESUME` → sets the status GPIO high (LED on)
- `PM_DEVICE_ACTION_SUSPEND` → sets the status GPIO low (LED off)

Because system PM (`CONFIG_PM`) is **not available on STM32F3** (see below), the application calls `pm_device_action_run()` directly to drive the LED state rather than relying on the PM subsystem to call it automatically.

## Out-of-tree module

The `led_indicator` driver lives in `modules/led_indicator/` and is registered as a Zephyr out-of-tree module via `ZEPHYR_EXTRA_MODULES` in `CMakeLists.txt`. The module layout is:

```
modules/led_indicator/
├── CMakeLists.txt
├── Kconfig
├── drivers/led_indicator/
│   ├── led_indicator.c       # driver implementation
│   ├── led_indicator.h       # LED_INDICATOR_DEV macro
│   ├── CMakeLists.txt
│   └── Kconfig               # CONFIG_LED_INDICATOR
├── dts/bindings/
│   └── example,led-indicator.yaml  # DTS compatible string + status-gpios property
└── zephyr/module.yaml        # tells Zephyr where to find cmake/kconfig/dts roots
```

The DTS node is declared in `boards/stm32f3_disco.overlay` using the `example,led-indicator` compatible, which ties `status-gpios` to PE8 (blue LED). `CONFIG_LED_INDICATOR=y` in `prj.conf` enables the driver.

## CONFIG_PM on STM32F3

`CONFIG_PM` depends on `HAS_PM`, which must be selected by the SoC Kconfig. The STM32F3 family (`soc/st/stm32/stm32f3x/Kconfig`) does not select `HAS_PM` — there is no `power.c` for STM32F3 and no `zephyr,power-state` nodes in its DTS. As a result, setting `CONFIG_PM=y` in `prj.conf` is silently ignored by Kconfig. STM32 families that do support system PM include F1, F4, G0, L4, and U5.

## Build, Flash, and Run

```bash
export ZEPHYR_BASE=~/zephyrproject/zephyr
west build --pristine --board stm32f3_disco .
west flash
```
