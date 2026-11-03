#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/usb/class/usbd_hid.h>
#include <zephyr/logging/log.h>

#include "keycode.h"
#include "keymap.h"
#include "usb_hid.h"

LOG_MODULE_REGISTER(chaox75_usb, LOG_LEVEL_INF);

#define USB_CONFIG_ATTRIBUTES 0
#define USB_MAX_POWER_UNITS   125

#define KEYBOARD_REPORT_ID    1
#define CONSUMER_REPORT_ID    2

#define KEYBOARD_REPORT_SIZE  9
#define CONSUMER_REPORT_SIZE  2

enum {
    HID_MODIFIER_CTRL  = 1U << 0,
    HID_MODIFIER_SHIFT = 1U << 1,
    HID_MODIFIER_ALT   = 1U << 2,
    HID_MODIFIER_GUI   = 1U << 3,
    HID_MODIFIER_RIGHT_SHIFT = 1U << 5,
};

enum {
    REPORT_ID = 0,
    REPORT_MODIFIER,
    REPORT_RESERVED,
    REPORT_KEY_1,
    REPORT_KEY_2,
    REPORT_KEY_3,
    REPORT_KEY_4,
    REPORT_KEY_5,
    REPORT_KEY_6,
};

enum {
    MEDIA_PLAY_PAUSE = 1U << 0,
    MEDIA_NEXT       = 1U << 1,
    MEDIA_PREVIOUS   = 1U << 2,
    MEDIA_MUTE       = 1U << 3,
    MEDIA_VOLUME_UP  = 1U << 4,
    MEDIA_VOLUME_DOWN = 1U << 5,
};

static const uint8_t hid_report_desc[] = {
    /* Keyboard report: report ID 1. */
    0x05, 0x01,       /* Usage Page (Generic Desktop) */
    0x09, 0x06,       /* Usage (Keyboard) */
    0xA1, 0x01,       /* Collection (Application) */
    0x85, KEYBOARD_REPORT_ID,

    0x05, 0x07,       /* Usage Page (Keyboard/Keypad) */
    0x19, 0xE0,       /* Usage Minimum (Left Control) */
    0x29, 0xE7,       /* Usage Maximum (Right GUI) */
    0x15, 0x00,       /* Logical Minimum (0) */
    0x25, 0x01,       /* Logical Maximum (1) */
    0x75, 0x01,       /* Report Size (1) */
    0x95, 0x08,       /* Report Count (8) */
    0x81, 0x02,       /* Input (Data, Variable, Absolute) */

    0x75, 0x08,       /* Report Size (8) */
    0x95, 0x01,       /* Report Count (1) */
    0x81, 0x01,       /* Input (Constant) */

    0x05, 0x08,       /* Usage Page (LEDs) */
    0x19, 0x01,       /* Usage Minimum (Num Lock) */
    0x29, 0x05,       /* Usage Maximum (Kana) */
    0x75, 0x01,       /* Report Size (1) */
    0x95, 0x05,       /* Report Count (5) */
    0x91, 0x02,       /* Output (Data, Variable, Absolute) */
    0x75, 0x03,       /* Report Size (3) */
    0x95, 0x01,       /* Report Count (1) */
    0x91, 0x01,       /* Output (Constant) */

    0x05, 0x07,       /* Usage Page (Keyboard/Keypad) */
    0x19, 0x00,       /* Usage Minimum (Reserved) */
    0x29, 0x65,       /* Usage Maximum (Keyboard Application) */
    0x15, 0x00,       /* Logical Minimum (0) */
    0x25, 0x65,       /* Logical Maximum (101) */
    0x75, 0x08,       /* Report Size (8) */
    0x95, 0x06,       /* Report Count (6) */
    0x81, 0x00,       /* Input (Data, Array, Absolute) */
    0xC0,             /* End Collection */

    /* Consumer Control report: report ID 2. */
    0x05, 0x0C,       /* Usage Page (Consumer) */
    0x09, 0x01,       /* Usage (Consumer Control) */
    0xA1, 0x01,       /* Collection (Application) */
    0x85, CONSUMER_REPORT_ID,

    0x09, 0xCD,       /* Usage (Play/Pause) */
    0x09, 0xB5,       /* Usage (Scan Next Track) */
    0x09, 0xB6,       /* Usage (Scan Previous Track) */
    0x09, 0xE2,       /* Usage (Mute) */
    0x09, 0xE9,       /* Usage (Volume Increment) */
    0x09, 0xEA,       /* Usage (Volume Decrement) */

    0x15, 0x00,       /* Logical Minimum (0) */
    0x25, 0x01,       /* Logical Maximum (1) */
    0x75, 0x01,       /* Report Size (1) */
    0x95, 0x06,       /* Report Count (6) */
    0x81, 0x02,       /* Input (Data, Variable, Absolute) */

    0x75, 0x02,       /* Report Size (2) */
    0x95, 0x01,       /* Report Count (1) */
    0x81, 0x01,       /* Input (Constant padding) */
    0xC0,             /* End Collection */
};

UDC_STATIC_BUF_DEFINE(keyboard_report, KEYBOARD_REPORT_SIZE);
UDC_STATIC_BUF_DEFINE(consumer_report, CONSUMER_REPORT_SIZE);

USBD_DEVICE_DEFINE(
    chaox75_usbd,
    DEVICE_DT_GET(DT_NODELABEL(zephyr_udc0)),
    CONFIG_CHAOX75_USB_VID,
    CONFIG_CHAOX75_USB_PID
);

USBD_DESC_LANG_DEFINE(chaox75_lang);
USBD_DESC_MANUFACTURER_DEFINE(
    chaox75_mfr,
    CONFIG_CHAOX75_USB_MANUFACTURER
);
USBD_DESC_PRODUCT_DEFINE(
    chaox75_product,
    CONFIG_CHAOX75_USB_PRODUCT
);
USBD_DESC_CONFIG_DEFINE(
    chaox75_cfg_desc,
    "CHAOX-75 Configuration"
);

USBD_CONFIGURATION_DEFINE(
    chaox75_fs_config,
    USB_CONFIG_ATTRIBUTES,
    USB_MAX_POWER_UNITS,
    &chaox75_cfg_desc
);

static const struct device *hid_device;
static struct usbd_context *usb_context = &chaox75_usbd;
static bool hid_ready;

static void hid_iface_ready(const struct device *dev, bool ready)
{
    ARG_UNUSED(dev);

    hid_ready = ready;
    LOG_INF("USB HID interface %s", ready ? "ready" : "not ready");
}

static int hid_get_report(const struct device *dev,
                          uint8_t type,
                          uint8_t id,
                          uint16_t len,
                          uint8_t *buf)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(type);
    ARG_UNUSED(id);
    ARG_UNUSED(len);
    ARG_UNUSED(buf);

    return -ENOTSUP;
}

static int hid_set_report(const struct device *dev,
                          uint8_t type,
                          uint8_t id,
                          uint16_t len,
                          const uint8_t *buf)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(type);
    ARG_UNUSED(id);
    ARG_UNUSED(len);
    ARG_UNUSED(buf);

    return -ENOTSUP;
}

static void hid_set_idle(const struct device *dev,
                         uint8_t id,
                         uint32_t duration)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(id);
    ARG_UNUSED(duration);
}

static uint32_t hid_get_idle(const struct device *dev, uint8_t id)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(id);

    return 0;
}

static void hid_set_protocol(const struct device *dev, uint8_t protocol)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(protocol);
}

static void hid_output_report(const struct device *dev,
                              uint16_t len,
                              const uint8_t *buf)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(len);
    ARG_UNUSED(buf);
}

static const struct hid_device_ops hid_ops = {
    .iface_ready = hid_iface_ready,
    .get_report = hid_get_report,
    .set_report = hid_set_report,
    .set_idle = hid_set_idle,
    .get_idle = hid_get_idle,
    .set_protocol = hid_set_protocol,
    .output_report = hid_output_report,
};

static void usb_message_callback(struct usbd_context *ctx,
                                 const struct usbd_msg *msg)
{
    LOG_INF("USB message: %s", usbd_msg_type_string(msg->type));

    if (!usbd_can_detect_vbus(ctx)) {
        return;
    }

    if (msg->type == USBD_MSG_VBUS_READY) {
        int ret = usbd_enable(ctx);

        if (ret < 0) {
            LOG_ERR("Failed to enable USB: %d", ret);
        }
    } else if (msg->type == USBD_MSG_VBUS_REMOVED) {
        int ret = usbd_disable(ctx);

        if (ret < 0) {
            LOG_ERR("Failed to disable USB: %d", ret);
        }

        hid_ready = false;
    }
}

static int usb_add_descriptor(struct usbd_desc_node *desc)
{
    int ret = usbd_add_descriptor(usb_context, desc);

    if (ret < 0) {
        LOG_ERR("Failed to add USB descriptor: %d", ret);
    }

    return ret;
}

int chaox75_usb_hid_init(void)
{
    int ret;

    keyboard_report[REPORT_ID] = KEYBOARD_REPORT_ID;
    consumer_report[0] = CONSUMER_REPORT_ID;

    hid_device = DEVICE_DT_GET_ONE(zephyr_hid_device);

    if (!device_is_ready(hid_device)) {
        LOG_ERR("USB HID device is not ready");
        return -ENODEV;
    }

    ret = hid_device_register(hid_device,
                              hid_report_desc,
                              sizeof(hid_report_desc),
                              &hid_ops);
    if (ret < 0) {
        LOG_ERR("Failed to register HID device: %d", ret);
        return ret;
    }

    ret = usb_add_descriptor(&chaox75_lang);
    if (ret < 0) {
        return ret;
    }

    ret = usb_add_descriptor(&chaox75_mfr);
    if (ret < 0) {
        return ret;
    }

    ret = usb_add_descriptor(&chaox75_product);
    if (ret < 0) {
        return ret;
    }

    ret = usbd_add_configuration(usb_context,
                                 USBD_SPEED_FS,
                                 &chaox75_fs_config);
    if (ret < 0) {
        LOG_ERR("Failed to add USB configuration: %d", ret);
        return ret;
    }

    static const char *const class_blocklist[] = {
        "dfu_dfu",
        NULL,
    };

    ret = usbd_register_all_classes(usb_context,
                                    USBD_SPEED_FS,
                                    1,
                                    class_blocklist);
    if (ret < 0) {
        LOG_ERR("Failed to register USB classes: %d", ret);
        return ret;
    }

    usbd_self_powered(usb_context, false);

    ret = usbd_msg_register_cb(usb_context, usb_message_callback);
    if (ret < 0) {
        LOG_ERR("Failed to register USB callback: %d", ret);
        return ret;
    }

    ret = usbd_init(usb_context);
    if (ret < 0) {
        LOG_ERR("Failed to initialize USB device: %d", ret);
        return ret;
    }

    if (!usbd_can_detect_vbus(usb_context)) {
        ret = usbd_enable(usb_context);
        if (ret < 0) {
            LOG_ERR("Failed to enable USB device: %d", ret);
            return ret;
        }
    }

    LOG_INF("CHAOX-75 USB HID initialized");
    return 0;
}

static int submit_keyboard_report(void)
{
    return hid_device_submit_report(hid_device,
                                    KEYBOARD_REPORT_SIZE,
                                    keyboard_report);
}

static int submit_consumer_report(void)
{
    return hid_device_submit_report(hid_device,
                                    CONSUMER_REPORT_SIZE,
                                    consumer_report);
}

int chaox75_usb_hid_send_key(uint8_t keycode, bool pressed)
{
    if (!hid_ready) {
        return -EAGAIN;
    }

    if (keycode == KEY_NONE || keycode == KEY_FN) {
        return -EINVAL;
    }

    uint8_t modifier = 0;
    switch (keycode) {
    case KEY_CTRL:        modifier = HID_MODIFIER_CTRL; break;
    case KEY_SHIFT:       modifier = HID_MODIFIER_SHIFT; break;
    case KEY_ALT:         modifier = HID_MODIFIER_ALT; break;
    case KEY_GUI:         modifier = HID_MODIFIER_GUI; break;
    case KEY_RIGHT_SHIFT: modifier = HID_MODIFIER_RIGHT_SHIFT; break;
    default: break;
    }

    if (modifier != 0) {
        if (pressed) {
            keyboard_report[REPORT_MODIFIER] |= modifier;
        } else {
            keyboard_report[REPORT_MODIFIER] &= (uint8_t)~modifier;
        }
        return submit_keyboard_report();
    }

    uint8_t usage_code = chaox75_keycode_to_hid(keycode);

    if (usage_code == 0) {
        return -EINVAL;
    }

    if (pressed) {
        for (int i = REPORT_KEY_1; i <= REPORT_KEY_6; i++) {
            if (keyboard_report[i] == usage_code) {
                return submit_keyboard_report();
            }
        }

        for (int i = REPORT_KEY_1; i <= REPORT_KEY_6; i++) {
            if (keyboard_report[i] == 0) {
                keyboard_report[i] = usage_code;
                return submit_keyboard_report();
            }
        }

        return -ENOSPC;
    }

    for (int i = REPORT_KEY_1; i <= REPORT_KEY_6; i++) {
        if (keyboard_report[i] == usage_code) {
            keyboard_report[i] = 0;
            break;
        }
    }

    return submit_keyboard_report();
}

int chaox75_usb_hid_send_media(uint8_t keycode, bool pressed)
{
    if (!hid_ready) {
        return -EAGAIN;
    }

    uint8_t mask;

    switch (keycode) {
    case KEY_MEDIA_PLAY_PAUSE:
        mask = MEDIA_PLAY_PAUSE;
        break;
    case KEY_MEDIA_NEXT:
        mask = MEDIA_NEXT;
        break;
    case KEY_MEDIA_PREVIOUS:
        mask = MEDIA_PREVIOUS;
        break;
    case KEY_MEDIA_MUTE:
        mask = MEDIA_MUTE;
        break;
    case KEY_MEDIA_VOLUME_UP:
        mask = MEDIA_VOLUME_UP;
        break;
    case KEY_MEDIA_VOLUME_DOWN:
        mask = MEDIA_VOLUME_DOWN;
        break;
    default:
        return -EINVAL;
    }

    if (pressed) {
        consumer_report[1] |= mask;
    } else {
        consumer_report[1] &= (uint8_t)~mask;
    }

    return submit_consumer_report();
}
