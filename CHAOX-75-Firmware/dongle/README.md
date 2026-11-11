# USB dongle image

Target: separate nRF52840 dongle PCB.

This image must implement the matching 2.4 GHz receiver/protocol and expose received key events to the host as USB HID. Do not use a generic BLE receiver as a substitute. The protocol, pairing, encryption/replay protection, packet format, retransmission, and USB HID interface must be designed and tested together with the keyboard image.
