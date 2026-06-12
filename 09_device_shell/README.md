# 09 Device Shell

Blinks an LED at a configurable period. A custom shell command lets you change the blink rate at runtime. `CONFIG_DEVICE_SHELL` also enables the built-in `device list` command to inspect all registered devices.

## Build, Flash, and Run

```bash
export ZEPHYR_BASE=~/zephyrproject/zephyr
west build --pristine --board stm32f3_disco .
west flash
```

Connect to the shell over UART (default 115200 baud):

```bash
minicom -D /dev/ttyACM0 -b 115200
```

## Shell Backend

`CONFIG_SHELL_BACKEND_SERIAL=y` enables the UART shell backend. The UART it uses is determined by the `zephyr,shell-uart` chosen node in the DTS. The STM32F3 disco board DTS already sets this to `usart1`, so no overlay change is needed. To use a different UART, override it in the board overlay:

```dts
/ {
    chosen {
        zephyr,shell-uart = &usart2;
    };
};
```

## Shell Commands

| Command | Description |
|---------|-------------|
| `led period <ms>` | Set the LED blink period in milliseconds |
| `device list` | List all registered devices and their status |

## Example Session

```
uart:~$ led period 250
LED blink period set to 250 ms
uart:~$ device list
devices:
- clock@40021000 (READY)
- ...
```
