# zephyr_examples

A collection of Zephyr examples I used while learning Zephyr, stored here to refer back to.

All examples have been tested on the STM32F3 Discovery board. If you use a different board you will need to update the board name and any board-specific overlay files.

## Examples

| # | Description |
|---|-------------|
| 01 | Blinks an LED |
| 02 | Blinks an LED using the LED class |
| 03 | Demonstrates dictionary logging — a way to construct log messages on the host rather than on the device, reducing logging overhead at the cost of post-processing on the host side |
| 04 | Uses the on-board STM32F3 Discovery accelerometer with an I2C data-ready interrupt; maps the accelerometer output to the on-board LEDs |
| 05 | Example firmware for use with MCUboot (no firmware update mechanism) |
| 06 | Firmware for use with MCUboot that supports firmware update over UART |

## Instructions for using STM32F3 Discovery with WSL2

I used the STM32F3 Discovery board with WSL2 for these examples. Some extra setup is required to connect the board to WSL2 — see the guide here:

[WSL2 USB device setup](https://mcbottcher.github.io/my_docs/wsl.html)
