# 10 PM Shell

Two custom `led_indicator` devices controlled from the shell using Zephyr's built-in PM device shell commands. Both LEDs turn on at boot. Suspend and resume them individually to observe device PM in action.

## LEDs

| Device | LED | Pin |
|--------|-----|-----|
| `led_indicator_blue` | Blue (LD4) | PE8 |
| `led_indicator_orange` | Orange (LD5) | PE10 |

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

## Shell Commands

| Command | Description |
|---------|-------------|
| `pm device suspend <name>` | Suspend a device (LED off) |
| `pm device resume <name>` | Resume a device (LED on) |
| `device list` | List all registered devices and their status |

## Example Session

```
uart:~$ pm device suspend led_indicator_blue
uart:~$ pm device suspend led_indicator_orange
uart:~$ pm device resume led_indicator_blue
uart:~$ device list
devices:
- led_indicator_blue (READY)
- led_indicator_orange (SUSPENDED)
- ...
```
