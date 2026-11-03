#ifndef CHAOX75_MATRIX_H
#define CHAOX75_MATRIX_H

#include <stdbool.h>
#include <stdint.h>

#define MATRIX_ROWS 6
#define MATRIX_COLS 15

int chaox75_matrix_init(void);

int chaox75_matrix_scan(
    bool current_state[MATRIX_ROWS][MATRIX_COLS]);

void chaox75_matrix_process(
    const bool current_state[MATRIX_ROWS][MATRIX_COLS]);

#endif /* CHAOX75_MATRIX_H */