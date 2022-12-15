/**
    \file    main.c
    \brief   led spark with systick

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
    \version 2022-03-09, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include <stdio.h>

#include "gd32f4xx.h"
#include "main.h"

#include "clk.h"
#include "exmc_sdram.h"
#include "systick.h"
#include "tftlcd.h"

#include "lvgl.h"

/**
    \brief    toggle the led every 500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_spark(void)
{
    static __IO uint32_t timingdelaylocal = 0U;

    if (timingdelaylocal) {

        // if(timingdelaylocal < 500U) {
        //     gd_eval_led_on(LED1);
        // } else {
        //     gd_eval_led_off(LED1);
        // }

        timingdelaylocal--;
    } else {
        timingdelaylocal = 100U;
    }
}

/**
    \brief    main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    rcu_init();
    systick_config();
    exmc_synchronous_dynamic_ram_init(EXMC_SDRAM_DEVICE0);
    // sdram_memcheck(EXMC_SDRAM_DEVICE0);

    // uint8_t ptr[32];
    // uint8_t ptr_read[600];
    // memset(ptr, 0xcc, sizeof(ptr));
    // memset(ptr_read, 0, sizeof(ptr_read));
    // sdram_writebuffer_8(EXMC_SDRAM_DEVICE0, ptr, 0, sizeof(ptr));
    // sdram_readbuffer_8(EXMC_SDRAM_DEVICE0, ptr_read, 0, sizeof(ptr_read));
    // memset(ptr, 0xAA, sizeof(ptr));
    // memset(ptr_read, 0, sizeof(ptr_read));
    // sdram_writebuffer_16(EXMC_SDRAM_DEVICE0, (uint16_t *)ptr, 0, sizeof(ptr) / sizeof(uint16_t));
    // sdram_readbuffer_16(EXMC_SDRAM_DEVICE0, (uint16_t *)ptr_read, 0, sizeof(ptr_read) / sizeof(uint16_t));

    lcd_init();
    lcd_layer_init(LCD_LAYER_BACKGROUND);
    lcd_layer_enable(LCD_LAYER_BACKGROUND);
    lcd_layer_set(LCD_LAYER_BACKGROUND);
    lcd_clear(lcd_get_color_value(lcd_color_black));// black
    // sdram_set_16(EXMC_SDRAM_DEVICE0, 0x0000, 0, 800 * 480);
    delay_1ms(1000);
    lcd_clear(lcd_get_color_value(lcd_color_white));// white
    // sdram_set_16(EXMC_SDRAM_DEVICE0, 0xffff, 0, 800 * 480);
    delay_1ms(1000);
    lcd_clear(lcd_get_color_value(lcd_color_blue));// blue
    // sdram_set_16(EXMC_SDRAM_DEVICE0, 0x001f, 0, 800 * 480);
    delay_1ms(1000);
    lcd_clear(lcd_get_color_value(lcd_color_green));// green
    // sdram_set_16(EXMC_SDRAM_DEVICE0, 0x07e0, 0, 800 * 480);
    delay_1ms(1000);
    lcd_clear(lcd_get_color_value(lcd_color_red));// red
    // sdram_set_16(EXMC_SDRAM_DEVICE0, 0xf800, 0, 800 * 480);
    delay_1ms(1000);

    lcd_line_draw(0, 50, 400, LCD_LINEDIR_HORIZONTAL);
    lcd_rectangle_draw(100, 100, 100, 200);
    lcd_rectangle_fill(400, 100, 200, 100);
    lcd_font_set(&font_6x11);
    lcd_char_display(300, 200, '!');
    lcd_string_display(300, 220, " !\"#$%'()*+,-.0123456789:;<=>?@");
    lcd_string_display(300, 240, "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`");
    lcd_string_display(300, 260, "abcdefghijklmnopqrstuvwxyz{|}~");

    // lcd_layer_init(LCD_LAYER_FOREGROUND);
    // lcd_layer_enable(LCD_LAYER_FOREGROUND);
    // lcd_layer_set(LCD_LAYER_FOREGROUND);
    // lcd_clear(lcd_get_color_value(lcd_color_black));// black
    // delay_1ms(1000);
    // lcd_clear(lcd_get_color_value(lcd_color_white));// white
    // delay_1ms(1000);
    // lcd_clear(lcd_get_color_value(lcd_color_blue));// blue
    // delay_1ms(1000);
    // lcd_clear(lcd_get_color_value(lcd_color_green));// green
    // delay_1ms(1000);
    // lcd_clear(lcd_get_color_value(lcd_color_red));// red
    // delay_1ms(1000);

    while (1) {
        led_spark();
        delay_1ms(10);
    }
}
