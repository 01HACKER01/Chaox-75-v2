#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "keycode.h"
#include "keymap.h"
#include "matrix.h"
#include "usb_hid.h"

LOG_MODULE_REGISTER(chaox75_matrix, LOG_LEVEL_INF);

#define MATRIX_NODE DT_PATH(zephyr_user)

#define DEBOUNCE_MS 10

static const struct gpio_dt_spec row_pins[MATRIX_ROWS] = {
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, row_gpios, 0),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, row_gpios, 1),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, row_gpios, 2),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, row_gpios, 3),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, row_gpios, 4),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, row_gpios, 5),
};

static const struct gpio_dt_spec col_pins[MATRIX_COLS] = {
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 0),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 1),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 2),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 3),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 4),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 5),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 6),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 7),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 8),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 9),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 10),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 11),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 12),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 13),
    GPIO_DT_SPEC_GET_BY_IDX(MATRIX_NODE, col_gpios, 14),
};

struct key_state {
    bool raw;
    bool stable;
    bool reported;
    int64_t raw_changed_at;
    uint8_t active_code;
};

static struct key_state keys[MATRIX_ROWS][MATRIX_COLS];

static bool is_media_key(uint8_t code)
{
    return code == KEY_MEDIA_PLAY_PAUSE ||
           code == KEY_MEDIA_NEXT ||
           code == KEY_MEDIA_PREVIOUS ||
           code == KEY_MEDIA_MUTE ||
           code == KEY_MEDIA_VOLUME_UP ||
           code == KEY_MEDIA_VOLUME_DOWN;
}

int chaox75_matrix_init(void)
{
    for (int row = 0; row < MATRIX_ROWS; row++) {
        if (!gpio_is_ready_dt(&row_pins[row])) {
            LOG_ERR("Row %d GPIO controller is not ready", row);
            return -ENODEV;
        }

        int ret = gpio_pin_configure_dt(
            &row_pins[row], GPIO_OUTPUT_INACTIVE);

        if (ret < 0) {
            LOG_ERR("Failed to configure row %d: %d", row, ret);
            return ret;
        }
    }

    for (int col = 0; col < MATRIX_COLS; col++) {
        if (!gpio_is_ready_dt(&col_pins[col])) {
            LOG_ERR("Column %d GPIO controller is not ready", col);
            return -ENODEV;
        }

        int ret = gpio_pin_configure_dt(
            &col_pins[col], GPIO_INPUT | GPIO_PULL_DOWN);

        if (ret < 0) {
            LOG_ERR("Failed to configure column %d: %d", col, ret);
            return ret;
        }
    }

    return 0;
}

int chaox75_matrix_scan(
    bool current_state[MATRIX_ROWS][MATRIX_COLS])
{
    for (int row = 0; row < MATRIX_ROWS; row++) {
        int ret = gpio_pin_set_dt(&row_pins[row], 1);

        if (ret < 0) {
            LOG_ERR("Failed to activate row %d: %d", row, ret);
            return ret;
        }

        k_busy_wait(10);

        for (int col = 0; col < MATRIX_COLS; col++) {
            int value = gpio_pin_get_dt(&col_pins[col]);

            if (value < 0) {
                (void)gpio_pin_set_dt(&row_pins[row], 0);
                LOG_ERR("Failed to read column %d: %d", col, value);
                return value;
            }

            current_state[row][col] = (value != 0);
        }

        ret = gpio_pin_set_dt(&row_pins[row], 0);

        if (ret < 0) {
            LOG_ERR("Failed to deactivate row %d: %d", row, ret);
            return ret;
        }
    }

    return 0;
}

void chaox75_matrix_process(
    const bool current_state[MATRIX_ROWS][MATRIX_COLS])
{
    int64_t now = k_uptime_get();
    bool fn_active = false;

    /* Pass 1: Update debounced states for all switches. */
    for (int row = 0; row < MATRIX_ROWS; row++) {
        for (int col = 0; col < MATRIX_COLS; col++) {
            struct key_state *key = &keys[row][col];
            bool reading = current_state[row][col];

            if (reading != key->raw) {
                key->raw = reading;
                key->raw_changed_at = now;
            }

            if (key->stable != key->raw &&
                now - key->raw_changed_at >= DEBOUNCE_MS) {
                key->stable = key->raw;
            }

            if (key->stable && keymap[row][col] == KEY_FN) {
                fn_active = true;
            }
        }
    }

    /* Pass 2: Process transitions using the current Fn state. */
    for (int row = 0; row < MATRIX_ROWS; row++) {
        for (int col = 0; col < MATRIX_COLS; col++) {
            struct key_state *key = &keys[row][col];

            if (key->stable == key->reported) {
                continue;
            }

            key->reported = key->stable;

            uint8_t base_code = keymap[row][col];

            /* Fn is a layer switch, not a USB keyboard key. */
            if (base_code == KEY_NONE || base_code == KEY_FN) {
                continue;
            }

            if (key->stable) {
                uint8_t code = base_code;

                if (fn_active) {
                    switch (base_code) {
                    case KEY_W:         code = KEY_UP;        break;
                    case KEY_A:         code = KEY_LEFT;      break;
                    case KEY_S:         code = KEY_DOWN;      break;
                    case KEY_D:         code = KEY_RIGHT;     break;
                    case KEY_1:         code = KEY_F1;        break;
                    case KEY_2:         code = KEY_F2;        break;
                    case KEY_3:         code = KEY_F3;        break;
                    case KEY_4:         code = KEY_F4;        break;
                    case KEY_5:         code = KEY_F5;        break;
                    case KEY_6:         code = KEY_F6;        break;
                    case KEY_7:         code = KEY_F7;        break;
                    case KEY_8:         code = KEY_F8;        break;
                    case KEY_9:         code = KEY_F9;        break;
                    case KEY_0:         code = KEY_F10;       break;
                    case KEY_Q:         code = KEY_HOME;      break;
                    case KEY_E:         code = KEY_END;       break;
                    case KEY_R:         code = KEY_PAGE_UP;   break;
                    case KEY_F:         code = KEY_PAGE_DOWN; break;
                    case KEY_BACKSPACE: code = KEY_DELETE;    break;
                    case KEY_TAB:       code = KEY_INSERT;    break;
                    case KEY_Z:         code = KEY_MEDIA_PLAY_PAUSE;  break;
                    case KEY_X:         code = KEY_MEDIA_PREVIOUS;    break;
                    case KEY_C:         code = KEY_MEDIA_NEXT;        break;
                    case KEY_V:         code = KEY_MEDIA_MUTE;        break;
                    case KEY_G:         code = KEY_MEDIA_VOLUME_DOWN; break;
                    case KEY_B:         code = KEY_MEDIA_VOLUME_UP;   break;
                    default: break;
                    }
                }

                key->active_code = code;

                int ret = is_media_key(code)
                    ? chaox75_usb_hid_send_media(code, true)
                    : chaox75_usb_hid_send_key(code, true);

                if (ret < 0) {
                    LOG_ERR("Key press failed at R%d C%d: %d",
                            row, col, ret);
                }

                LOG_INF("Key R%d C%d code %u pressed",
                        row, col, (unsigned int)code);
            } else {
                uint8_t code = key->active_code;

                if (code != KEY_NONE) {
                    int ret = is_media_key(code)
                        ? chaox75_usb_hid_send_media(code, false)
                        : chaox75_usb_hid_send_key(code, false);

                    if (ret < 0) {
                        LOG_ERR("Key release failed at R%d C%d: %d",
                                row, col, ret);
                    }

                    LOG_INF("Key R%d C%d code %u released",
                            row, col, (unsigned int)code);
                }

                key->active_code = KEY_NONE;
            }
        }
    }
}
