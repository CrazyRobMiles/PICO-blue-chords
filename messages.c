#include <stdio.h>
#include "ssd1306.h"
#include "image.h"
#include "acme_5_outlines_font.h"
#include "bubblesstandard_font.h"
#include "crackers_font.h"
#include "BMSPA_font.h"

void setup_gpios(void)
{
    i2c_init(i2c1, 400000);
    gpio_set_function(18, GPIO_FUNC_I2C);
    gpio_set_function(19, GPIO_FUNC_I2C);
    gpio_pull_up(18);
    gpio_pull_up(19);
}

#define SLEEPTIME 25

ssd1306_t disp;

#define BUFFER_SIZE 100

char current_message[BUFFER_SIZE];
char bluetooth_status[BUFFER_SIZE];
char shift_status[BUFFER_SIZE];

void render_messages()
{
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0, 16, 2, current_message);
    ssd1306_draw_string(&disp, 2, 0, 1, bluetooth_status);
    ssd1306_draw_string(&disp, 2, 48, 1, shift_status);
    ssd1306_show(&disp);
}

int message_pos = 0;
#define MESSAGE_LIMIT 8

void display_message(const char *s)
{
    snprintf(current_message, BUFFER_SIZE, s);
    render_messages();
}

void add_character_to_message(char ch)
{
    if(ch<32 || ch>126){
        return;
    }

    if (message_pos < MESSAGE_LIMIT)
    {
        current_message[message_pos] = ch;
        message_pos++;
    }
    else
    {
        for (int i = 0; i < MESSAGE_LIMIT; i++ )
        {
            current_message[i] = current_message[i+1];
        }
        current_message[message_pos] = ch;
    }
    render_messages();
}

void clear_message(){
    for(int i=0;i<BUFFER_SIZE;i++){
        current_message[i] = ' ';
    }
    message_pos=0;
    render_messages();
}

void display_bluetooth_status(const char *s)
{
    snprintf(bluetooth_status, BUFFER_SIZE, s);
    render_messages();
}

void display_shift_status(const char *s)
{
    snprintf(shift_status, BUFFER_SIZE, s);
    render_messages();
}

void init_messages()
{
    setup_gpios();

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, i2c1);
    ssd1306_clear(&disp);
    display_message("");
    display_bluetooth_status("");
    display_shift_status("");
}
