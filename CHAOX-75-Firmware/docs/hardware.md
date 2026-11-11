# CHAOX-75 hardware notes

Extracted from the supplied KiCad PCB/schematic files:

| Function | PCB net / part |
|---|---|
| MCU | U4, Nordic nRF52840 |
| Matrix rows | R0, R1, R2, R3, R4, R5 |
| Matrix columns | C0 through C14 |
| Addressable RGB data | LED-DATA |
| Battery supply net | VBAT |
| Charger | MCP73831-2-OT |
| USB | VBUS, D+, D- |
| Rotary encoder signals | P0.14, P0.15, P0.16 are present as nets; verify exact encoder mapping from schematic |

This table lists net names, not the final Zephyr GPIO specifiers. Do not substitute the net labels for GPIO port/pin numbers.
