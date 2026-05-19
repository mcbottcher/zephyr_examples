# 05 Boot App (MCUboot)

A signed firmware image for use with MCUboot. It does not include a mechanism for receiving firmware updates — its purpose is to verify the boot process works. Once MCUboot and this firmware are both flashed, the LED should blink on boot.

You can monitor the MCUboot serial output with:

```bash
python -m serial.tools.miniterm /dev/ttyACM0 115200
```

## Build, Flash, and Run

Flash MCUboot first if it is not already on the device:

```bash
# Clone the MCUboot repo, then build and flash the bootloader
cd mcuboot/boot/zephyr
west build --pristine --board stm32f3_disco .
west flash
```

Then build and flash the application:

```bash
export ZEPHYR_BASE=~/zephyrproject/zephyr
west build --pristine --board stm32f3_disco .
west flash
```

## Interesting Files

| File | Description |
|------|-------------|
| `build/zephyr/runners.yaml` | Defines the runners available to `west flash` |
| `boards/st/stm32f3_disco/board.cmake` | Default flashing settings for the board (STM32CubeProgrammer) |

Run `west flash -- --help` to see the help message for the default flashing tool.

## Running with Signed Firmware

By default, MCUboot uses a built-in test key to sign images. To use your own key:

```bash
imgtool keygen --key mcuboot-signing-key.pem --type rsa-2048
```

The `.pem` file is a private key. Point both the MCUboot build and the application build at it via Kconfig:

```
CONFIG_MCUBOOT_SIGNATURE_KEY_FILE="/home/user/mcuboot-signing-key.pem"
CONFIG_BOOT_SIGNATURE_TYPE_RSA=y   # must match the key type
```

- **MCUboot build** — derives and embeds the public key, which MCUboot uses to verify signed images.
- **Application build** — uses the private key to sign the firmware image before flashing.
