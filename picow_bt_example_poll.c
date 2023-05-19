/**
  * 
 * By Rob Miles 2023
 * 
 * Based on examples:
 * 
* Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "btstack_run_loop.h"
#include "pico/stdlib.h"
#include "picow_bt_example_common.h"


int main()
{
    stdio_init_all();
    int res = picow_bt_example_init();
    if (res)
    {
        return -1;
    }

    pico_blue_chords_main();
    btstack_run_loop_execute();
}
