#include <stdbool.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "keymap.h"
#include "matrix.h"
#include "usb_hid.h"

LOG_MODULE_REGISTER(chaox75, LOG_LEVEL_INF);

#define SCAN_INTERVAL_MS 5

int main(void)
{
    bool current_state[MATRIX_ROWS][MATRIX_COLS] = {0};

    LOG_INF("CHAOX-75 keyboard starting");

    int ret = chaox75_matrix_init();

    if (ret < 0) {
        LOG_ERR("Matrix initialization failed: %d", ret);
        return ret;
    }

    LOG_INF("Matrix initialized: %d rows, %d columns",
            MATRIX_ROWS, MATRIX_COLS);

    ret = chaox75_usb_hid_init();

    if (ret < 0) {
        LOG_ERR("USB HID initialization failed: %d", ret);
        return ret;
    }

    LOG_INF("CHAOX-75 keyboard ready");

    while (true) {
        ret = chaox75_matrix_scan(current_state);

        if (ret == 0) {
            chaox75_matrix_process(current_state);
        } else {
            LOG_ERR("Matrix scan failed: %d", ret);
        }

        k_sleep(K_MSEC(SCAN_INTERVAL_MS));
    }

    return 0;
}