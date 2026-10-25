# Chaox-75-v2
<img width="410" height="329" alt="image" src="https://github.com/user-attachments/assets/0e184ab3-a807-4555-9e2d-b27b0a48235e" />
<img width="1010" height="632" alt="image" src="https://github.com/user-attachments/assets/887b0fd3-700c-49f0-ab09-b6765a803e89" />
<img width="800" height="305" alt="image" src="https://github.com/user-attachments/assets/7e0d5e57-19a0-49bf-9f55-15ddec867746" />
<img width="802" height="293" alt="image" src="https://github.com/user-attachments/assets/051067fd-8080-456e-9e36-00cb07725982" />
<img width="761" height="333" alt="image" src="https://github.com/user-attachments/assets/e66547de-7d9d-4594-8712-abd92a6a7974" />


#keyboard
so this keyboard hacs trimode connectivity 2.4GHz wired and buetooth 
i have made 1 more keyboard which was version 1 but this is version 2 i havent shipped that keyboard cause i havent made any firmware yetr cause i dont know how to make the firmware so we will also ship that keyboard soon. also this keyboard has 82 mx cherry hotswappable switches and 1 rotaary encoder it also has 1 oled display and the micro controller is the nRF52840.   

# CHAOX-75 Firmware — Pre-Release

Firmware for the CHAOX-75 keyboard, built using C and Zephyr RTOS with the nRF52840 platform.

This pre-release includes the keyboard matrix handling, key mappings, USB HID support, and function-layer shortcuts.

## What's included

- Keyboard matrix handling (6 × 15)
- Standard keyboard input over USB HID
- Fn-layer key mappings
- Media controls
- Firmware binaries for flashing
- SHA-256 checksums for file verification

## Fn Layer

The Fn key gives access to additional shortcuts.

| Key | Function |
|---|---|
| W | Up Arrow |
| A | Left Arrow |
| S | Down Arrow |
| D | Right Arrow |
| 1–0 | F1–F10 |
| Q | Home |
| E | End |
| R | Page Up |
| F | Page Down |
| Backspace | Delete |
| Tab | Insert |
| Z | Play/Pause |
| X | Previous Track |
| C | Next Track |
| V | Mute |
| G | Volume Down |
| B | Volume Up |

## Release Files

The `release/CHAOX-75/` directory contains the following files:

| File | Description |
|---|---|
| `zephyr.hex` | Firmware image in HEX format |
| `zephyr.bin` | Firmware image in binary format |
| `zephyr.elf` | ELF build output |
| `SHA256SUMS.txt` | SHA-256 checksums for verifying the firmware files |

The release ZIP contains the firmware files and checksum list together.

## Flashing

1. Connect the nRF52840 board to your computer.
2. Put the board into its supported programming or bootloader mode.
3. Flash `zephyr.hex` using your preferred compatible flashing tool.
4. Wait for flashing to finish.
5. Restart the board and check keyboard input.

Use the flashing method appropriate for your specific nRF52840 board. The development-kit firmware configuration may need to be adapted if you're using custom CHAOX-75 hardware.

## Built With

- **Language:** C
- **RTOS:** Zephyr
- **Platform:** Nordic nRF52840
- **SDK:** nRF Connect SDK 3.4.1
- **Build system:** CMake and west

## Project Structure

```text
keyboard/
├── CMakeLists.txt
├── kconfig
├── nrf52840dk_nrf52840.overlay
├── prj.conf
├── include/
│   ├── keycode.h
│   ├── matrix.h
│   └── usb_hid.h
└── src/
    ├── keycode.c
    ├── keymap.h
    ├── main.c
    ├── matrix.c
    └── usb_hid.c

release/
└── CHAOX-75/
    ├── zephyr.hex
    ├── zephyr.bin
    ├── zephyr.elf
    └── SHA256SUMS.txt
```

## Checksums

Use the values in `SHA256SUMS.txt` to verify that your firmware files match the release package.

On Windows PowerShell, you can calculate a file's SHA-256 hash with:

```powershell
Get-FileHash .\zephyr.hex -Algorithm SHA256
```

Compare the resulting hash with the corresponding entry in `SHA256SUMS.txt`.

## Status

This is a pre-release build of the CHAOX-75 firmware. It includes the current keyboard functionality and is intended for early use and feedback while development continues.

---

**CHAOX-75**  
*Custom keyboard firmware, built from the ground up.*

##thank you very much 
