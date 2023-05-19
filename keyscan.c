#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "messages.h"
#include "keyscan.h"

#ifdef LEFT_HANDED_KEYBOARD

#define LITTLE_KEY_GPIO 15
#define RING_KEY_GPIO 14
#define MIDDLE_KEY_GPIO 13
#define INDEX_KEY_GPIO 12
#define THUMB_KEY_GPIO 11
#define CONTROL_KEY_GPIO 10

#else

#define LITTLE_KEY_GPIO 10
#define RING_KEY_GPIO 11
#define MIDDLE_KEY_GPIO 12
#define INDEX_KEY_GPIO 13
#define THUMB_KEY_GPIO 14
#define CONTROL_KEY_GPIO 15

#endif

enum keyboard_states
{
    UPPER_CASE_KEYS,
    LOWER_CASE_KEYS,
    NUMBER_KEYS,
    SYMBOL_KEYS
};

enum keyboard_states keyboard_state;

struct key
{
    int gpio;
    bool state;
    char *name;
    int change_counter;
    int bit_number;
};

struct key keys[] =
    {
        {LITTLE_KEY_GPIO, false, "little key", 0, 0},
        {RING_KEY_GPIO, false, "ring key", 0, 1},
        {MIDDLE_KEY_GPIO, false, "middle key", 0, 2},
        {INDEX_KEY_GPIO, false, "index key", 0, 3},
        {THUMB_KEY_GPIO, false, "thumb", 0, 4},
        {CONTROL_KEY_GPIO, false, "control", 0, 5}};

#define DEBOUNCE_COUNT 1
#define KEY_PRESSED false

struct key_decode
{
    int scan;
    char ch;
};

struct key_decode lower_case_text_decode[] = {
    {12, 'a'},
    {56, 'b'},
    {10, 'c'},
    {14, 'd'},
    {4, 'e'},
    {30, 'f'},
    {48, 'g'},
    {34, 'h'},
    {6, 'i'},
    {50, 'j'},
    {18, 'k'},
    {38, 'l'},
    {60, 'm'},
    {24, 'n'},
    {8, 'o'},
    {62, 'p'},
    {40, 'q'},
    {22, 'r'},
    {16, 's'},
    {20, 't'},
    {32, 'u'},
    {36, 'v'},
    {54, 'w'},
    {58, 'x'},
    {26, 'y'},
    {42, 'z'},
    {2, ' '},
    {52, ','},
    {28, '.'},
    {0, ' '}};

struct key_decode upper_case_text_decode[] = {
    {12, 'A'},
    {56, 'B'},
    {10, 'C'},
    {14, 'D'},
    {4, 'E'},
    {30, 'F'},
    {48, 'G'},
    {34, 'H'},
    {6, 'I'},
    {50, 'J'},
    {18, 'K'},
    {38, 'L'},
    {60, 'M'},
    {24, 'N'},
    {8, 'O'},
    {62, 'P'},
    {40, 'Q'},
    {22, 'R'},
    {16, 'S'},
    {20, 'T'},
    {32, 'U'},
    {36, 'V'},
    {54, 'W'},
    {58, 'X'},
    {26, 'Y'},
    {42, 'Z'},
    {2, ' '},
    {52, ','},
    {28, '.'},
    {0, ' '}};

struct key_decode num_decode[] = {
    {8, '0'},
    {2, '1'},
    {6, '2'},
    {14, '3'},
    {30, '4'},
    {62, '5'},
    {32, '6'},
    {48, '7'},
    {56, '8'},
    {60, '9'},
    {52, '}'},
    {28, '.'},
    {0, ' '}};

struct key_decode sym_decode[] = {
    {2, ' '},
    {52, ','},
    {28, '.'},
    {54, ','},
    {50, ';'},
    {42, '%'},
    {4, '='},
    {22, '&'},
    {14, '('},
    {56, ')'},
    {16, '$'},
    {12, '@'},
    {62, '+'},
    {34, '#'},
    {24, '-'},
    {58, '!'},
    {26, '?'},
    {18, '/'},
    {30, '{'},
    {60, '}'},
    {10, '['},
    {40, ']'},
    {36, '\\'},
    {20, '*'},
    {6, '<'},
    {48, '>'},
    {0, ' '}};

char find_char(char scan, struct key_decode *decode)
{
    while (decode->scan > 0)
    {
        if (scan == decode->scan)
        {
            return decode->ch;
        }
        decode++;
    }
    // return 0 if we reach the end without finding a character
    return 0;
}

char key_bits = 0;
char assembling_bits = false;

int poll_gpio()
{
    char result = 0;

    for (int i = 0; i < sizeof(keys) / sizeof(struct key); i++)
    {

        int bit_mask = 1 << keys[i].bit_number;
        int gpio = keys[i].gpio;
        int in_state = gpio_get(gpio);
        if (keys[i].state == in_state)
        {
            keys[i].change_counter = 0;
        }
        else
        {
            keys[i].change_counter++;
            if (keys[i].change_counter >= DEBOUNCE_COUNT)
            {
                // Got a proper key state change
                // clear the debounce counter
                keys[i].change_counter = 0;
                keys[i].state = in_state;
                if (in_state == KEY_PRESSED)
                {
                    // key has been pressed
                    if (key_bits == 0)
                    {
                        // first press of a key
                        assembling_bits = true;
                    }
                    key_bits = key_bits + bit_mask;
                }
                else
                {
                    // key has been released
                    int bits = key_bits;
                    key_bits = key_bits - bit_mask;
                    if (assembling_bits)
                    {
                        // first key lifted which is part of a character
                        // decode it
                        result = bits;
                        assembling_bits = false;
                    }
                }
            }
        }
    }
    return result;
}

bool old_state;

void set_keyboard_state(enum keyboard_states state)
{

    keyboard_state = state;

    switch (keyboard_state)
    {
    case UPPER_CASE_KEYS:
        display_shift_status("CAPS");
        break;
    case LOWER_CASE_KEYS:
        display_shift_status("lower");
        break;
    case NUMBER_KEYS:
        display_shift_status("NUMB");
        break;
    case SYMBOL_KEYS:
        display_shift_status("SYM");
        break;
    }
}

void toggle_caps_lock()
{
    if (keyboard_state == UPPER_CASE_KEYS)
    {
        set_keyboard_state(LOWER_CASE_KEYS);
    }
    else
    {
        if (keyboard_state == LOWER_CASE_KEYS)
        {
            set_keyboard_state(UPPER_CASE_KEYS);
        }
    }
}

#define CHAR_RETURN '\n'
#define CHAR_ESCAPE 27
#define CHAR_TAB '\t'
#define CHAR_BACKSPACE 0x7f

int scan_keys()
{
    int bits = poll_gpio();

    if (bits == 0)
    {
        return 0;
    }

    char ch = 0;

    // decode the bits into a character
    switch (keyboard_state)
    {
    case UPPER_CASE_KEYS:
        ch = find_char(bits, upper_case_text_decode);
        break;
    case LOWER_CASE_KEYS:
        ch = find_char(bits, lower_case_text_decode);
        break;
    case NUMBER_KEYS:
        ch = find_char(bits, num_decode);
        break;
    case SYMBOL_KEYS:
        ch = find_char(bits, sym_decode);
        break;
    }

    if (ch == 0)
    {
        switch (bits)
        {
        case 1:
            toggle_caps_lock();
            break;
        case 13:
            ch = CHAR_BACKSPACE;
            break;
        case 17:
            set_keyboard_state(SYMBOL_KEYS);
            break;
        case 21:
            set_keyboard_state(LOWER_CASE_KEYS);
            break;
        case 25:
            set_keyboard_state(NUMBER_KEYS);
            break;
        case 44:
            ch = CHAR_RETURN;
            break;
        }
    }

    add_character_to_message(ch);

    return ch;
}

int init_keyscan()
{
    printf("starting keyscan");

    for (int i = 0; i < sizeof(keys) / sizeof(struct key); i++)
    {
        int bit_mask = 1 << keys[i].bit_number;
        int gpio = keys[i].gpio;
        gpio_set_dir(i, gpio);
        gpio_pull_up(gpio);
        int in_state = gpio_get(i);
        keys[i].state = in_state;
        if (in_state == KEY_PRESSED)
        {
            key_bits = key_bits + bit_mask;
        }
    }

    clear_message();

    set_keyboard_state(LOWER_CASE_KEYS);

    return 0;
}
