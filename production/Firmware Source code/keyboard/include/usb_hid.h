#ifndef CHAOX75_USB_HID_H
#define CHAOX75_USB_HID_H

#include <stdbool.h>
#include <stdint.h>

int chaox75_usb_hid_init(void);
int chaox75_usb_hid_send_key(uint8_t keycode, bool pressed);
int chaox75_usb_hid_send_media(uint8_t keycode, bool pressed);

#endif /* CHAOX75_USB_HID_H */
