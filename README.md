# Chaox-75-v2
<img width="410" height="329" alt="image" src="https://github.com/user-attachments/assets/0e184ab3-a807-4555-9e2d-b27b0a48235e" />
<img width="1010" height="632" alt="image" src="https://github.com/user-attachments/assets/887b0fd3-700c-49f0-ab09-b6765a803e89" />
<img width="800" height="305" alt="image" src="https://github.com/user-attachments/assets/7e0d5e57-19a0-49bf-9f55-15ddec867746" />
<img width="802" height="293" alt="image" src="https://github.com/user-attachments/assets/051067fd-8080-456e-9e36-00cb07725982" />
<img width="761" height="333" alt="image" src="https://github.com/user-attachments/assets/e66547de-7d9d-4594-8712-abd92a6a7974" />




## Keyboard
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

## BOM

| Part | Qty | Approx. USD | Link |
| -------------------------------------- | -- | ------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Custom 82-key Keyboard PCB (5 pcs) | 1 | ~$10–25 | [JLCPCB](https://jlcpcb.com/) |
| 1.5mm Anodized Aluminum Switch Plate | 1 | ~$12–22 | [JLCPCB CNC](https://jlcpcb.com/cnc-machining) |
| 3D Printed Keyboard Case (Top & Bottom) | 1 | ~$10–20 | [JLCPCB 3D Print](https://jlcpcb.com/3d-printing) |
| MX mechanical switches (90 pcs) | 1 | ~$18–35 | [Alibaba MX switches](https://www.alibaba.com/trade/search?SearchText=mechanical+keyboard+switches+90pcs) |
| PBT keycap set (82-key / 75%) | 1 | ~$15–30 | [Alibaba PBT keycaps](https://www.alibaba.com/trade/search?SearchText=PBT+keycaps+set) |
| Kailh MX hotswap sockets (100 pcs) | 1 | ~$6–10 | [LCSC Kailh sockets](https://www.lcsc.com/search?q=kailh%20socket) |
| SK6812MINI-E reverse-mount RGB LEDs (100 pcs) | 1 | ~$8–14 | [LCSC SK6812MINI-E](https://www.lcsc.com/search?q=SK6812MINI-E) |
| 1N4148W SOD-123 diodes (100 pcs) | 1 | ~$1–2 | [LCSC 1N4148W](https://www.lcsc.com/search?q=1N4148W) |
| PCB-mount screw-in stabilizers | 1 | ~$6–12 | [Alibaba PCB stabilizers](https://www.alibaba.com/trade/search?SearchText=PCB+mount+stabilizers) |
| EC11 rotary encoder with metal knob | 1 | ~$1–2 | [LCSC EC11 encoder](https://www.lcsc.com/search?q=EC11%20encoder) |
| 0.91" 128×32 I2C OLED display | 1 | ~$1.50–3 | [LCSC 0.91 OLED](https://www.lcsc.com/search?q=0.91%20inch%20OLED%20I2C) |
| 3.7V 2500–3000mAh LiPo battery (JST-PH 2.0) | 1 | ~$5–9 | [Alibaba LiPo battery](https://www.alibaba.com/trade/search?SearchText=3.7V+3000mAh+lipo+battery+JST) |
| Poron plate foam & switch pads | 1 | ~$3–6 | [Alibaba keyboard poron foam](https://www.alibaba.com/trade/search?SearchText=poron+foam+keyboard) |
| M2 heat-set brass inserts (20 pcs) | 1 | ~$1–3 | [Alibaba M2 heat-set inserts](https://www.alibaba.com/trade/search?SearchText=M2+heat+set+inserts) |
| M2 hex socket screws (20 pcs) | 1 | ~$1–2 | [Alibaba M2 screws](https://www.alibaba.com/trade/search?SearchText=M2+hex+screws) |
| Silicone rubber anti-slip feet (4 pcs) | 1 | ~$0.50–1.50 | [Alibaba silicone bumpons](https://www.alibaba.com/trade/search?SearchText=silicone+bumpons) |
| Braided USB-A → USB-C cable | 1 | ~$2–4 | [Alibaba USB-C cable](https://www.alibaba.com/trade/search?SearchText=braided+usb-c+cable) |
| Custom 2.4GHz USB Dongle (PCB + components) | 1 | ~$8–12 | [JLCPCB](https://jlcpcb.com/) |

### 💰 Estimated total

**Parts: About $110–210 USD**

**Estimated shipping: ~$25–45 USD**

**Total including shipping: About $135–255 USD**, depending heavily on the seller, quantity and shipping.

**CHAOX-75**  
*Custom keyboard firmware, built from the ground up.*

## Thank you very much 

