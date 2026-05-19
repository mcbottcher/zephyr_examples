# 06 Boot App 2 (MCUboot + UART OTA)

Builds on [05_boot_app](../05_boot_app/README.md) by adding a firmware update mechanism. Updates are performed over UART using the MCUmgr SMP protocol.

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

The `.pem` file is a private key. The build system derives the public key from this file and embeds it in the bootloader for verifying signed images.

## Performing a Firmware Update over UART (MCUmgr SMP)

The application runs an SMP server on UART (`usart1` / `/dev/ttyACM0`). Use [`mcumgr-client`](https://github.com/vouch-opensource/mcumgr-client) to upload a new image to slot 1 and trigger MCUboot to swap it in.

```bash
# 1. Build the new firmware image (e.g. change the LED colour in the overlay first)
west build --pristine --board stm32f3_disco .

# 2. Upload the signed image to slot 1
mcumgr-client -d /dev/ttyACM0 upload build/zephyr/zephyr.signed.bin

# If the upload hangs, try setting explicit timing/MTU options:
mcumgr-client -d /dev/ttyACM0 upload -b 115200 --mtu 128 --initial_timeout 10 --subsequent_timeout 5000 build/zephyr/zephyr.signed.bin

# 3. List images to get the hash of the newly uploaded image
mcumgr-client -d /dev/ttyACM0 list

# 4. Mark the new image as pending (MCUboot will swap on next reset)
mcumgr-client -d /dev/ttyACM0 test <hash of new firmware image>

# 5. Reset — MCUboot swaps slot 0 and slot 1, new image boots
mcumgr-client -d /dev/ttyACM0 reset
```

The new image calls `boot_write_img_confirmed()` on startup to make the update permanent. Without this call, MCUboot would revert to the previous image on the next reset. You can test this by removing that call and observing the revert behaviour.
