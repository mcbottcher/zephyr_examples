# 03 Dictionary Logging

Demonstrates Zephyr's dictionary-based logging, where log messages are encoded as compact IDs on the device and decoded into human-readable strings on the host using a dictionary file. This reduces the work the device needs to do at log time.

## Build, Flash, and Run

```bash
export ZEPHYR_BASE=~/zephyrproject/zephyr
west build --pristine --board stm32f3_disco .
west flash
python -m serial.tools.miniterm /dev/ttyACM0 115200
```

Or to run in QEMU:

```bash
west build --pristine -b qemu_x86 .
west build -t run
```

## Interesting Files

| File | Description |
|------|-------------|
| `build/zephyr/log_dictionary.json` | The dictionary used to decode log messages on the host |
