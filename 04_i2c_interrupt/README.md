# 04 I2C Interrupt

Uses the on-board STM32F3 Discovery accelerometer (LIS2DH / LSM303DLHC) with its data-ready interrupt to signal when new data is available on the I2C bus. The program reads the accelerometer and lights the on-board LED that is closest to the ground — spin the board to see the LEDs respond.

## Build, Flash, and Run

```bash
export ZEPHYR_BASE=~/zephyrproject/zephyr
west build --pristine --board stm32f3_disco .
west build -t menuconfig   # optional: review/adjust Kconfig options
west flash
```

## Interesting Notes

The board file declares the accelerometer with two compatible strings:

```
compatible = "st,lis2dh", "st,lsm303dlhc-accel";
```

Two things to note:

1. In `dts/bindings/sensor/st,lis2dh-i2c.yaml`, the bindings point to a common YAML file that specifies interrupt options — but notably not which sources trigger the interrupts.
2. In `zephyr/drivers/sensor/st/lis2dh/Kconfig`, the Kconfig options for the driver also do not specify interrupt sources.

In general, hardware configuration belongs in the device tree and software configuration belongs in Kconfig.

## Binding

Located at `zephyr/dts/bindings/sensor/st,lis2dh.yaml`. The `"st,lis2dh"` compatible string takes priority over `"st,lsm303dlhc-accel"` because it is listed first.

## Interrupt Pin Routing

The driver maps two logical signals to the physical `irq-gpios` pins. By default:

- **INT1** (`irq-gpios[0]`) → Data Ready (DRDY)
- **INT2** (`irq-gpios[1]`) → any-motion / other events

Adding `anym-on-int1` to the DT node swaps these, putting any-motion on INT1 and disabling DRDY. This is useful for boards with only one interrupt line wired. Since both pins are wired on the STM32F3 Discovery this property is not needed.

`anym-on-int1` is a boolean property — it is `false` by default when absent from the node, and `true` simply by being present (it is never assigned a value).

## Thread Options

The driver supports two trigger modes:

- **Global work queue** — the interrupt handler runs in the shared system work queue.
- **Own thread** — the driver creates a dedicated thread for the callback, which is faster but requires its own stack.
