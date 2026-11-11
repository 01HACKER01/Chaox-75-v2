# CHAOX-75 simple firmware (USB + Bluetooth starter)

This is the first ZMK-based source project for the CHAOX-75 nRF52840 keyboard. It includes a 6x15 GPIO matrix map extracted from the supplied KiCad PCB and a basic keymap.

## Build
1. Create a GitHub repository named `CHAOX-75-Firmware`.
2. Upload the contents of this folder to the repository root.
3. Open **Actions** and run **Build ZMK firmware** (or push a commit).
4. Download the build artifact from the completed workflow.

## Hardware / safety notes
- The ZMK shield currently uses the `nrf52840dk_nrf52840` build target as a starting point; the actual board has a bare nRF52840, so validate the final build and bootloader/UF2 or SWD flashing method before connecting power or flashing.
- `diode-direction = "col2row"` is a provisional setting and must be checked against the actual diode orientation. If the matrix does not scan correctly, do not ship this build.
- This first version targets basic USB HID and Bluetooth LE keyboard support. It does **not** yet implement RGB effects, battery measurement, configuration/macro features, or the custom 2.4 GHz keyboard-to-dongle protocol.
- The PCB uses SK6812MINI-E LEDs on P0.26; RGB will be added after basic matrix bring-up.
- Do not advertise this as production-ready until tested on the assembled hardware.
