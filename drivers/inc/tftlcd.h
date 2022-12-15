/*!
    \file  tftlcd.h
    \brief the header file of LCD
*/

#ifndef TFTLCD_H
#define TFTLCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#include "gd32f4xx.h"
#include "font.h"

#define LCD_LAYER_BACKGROUND 0x0000
#define LCD_LAYER_FOREGROUND 0x0001

typedef enum {
    lcd_color_white = 0,
    lcd_color_black,
    lcd_color_grey,
    lcd_color_blue,
    lcd_color_blue2,
    lcd_color_red,
    lcd_color_magenta,
    lcd_color_green,
    lcd_color_cyan,
    lcd_color_yellow,
} lcd_color_enum;

/* definitions of lines depending on the chosen fonts */
#define LINE(x) ((x) * (((font_t *)lcd_font_get())->Height))

#define LCD_LINE_0  LINE(0)
#define LCD_LINE_1  LINE(1)
#define LCD_LINE_2  LINE(2)
#define LCD_LINE_3  LINE(3)
#define LCD_LINE_4  LINE(4)
#define LCD_LINE_5  LINE(5)
#define LCD_LINE_6  LINE(6)
#define LCD_LINE_7  LINE(7)
#define LCD_LINE_8  LINE(8)
#define LCD_LINE_9  LINE(9)
#define LCD_LINE_10 LINE(10)
#define LCD_LINE_11 LINE(11)
#define LCD_LINE_12 LINE(12)
#define LCD_LINE_13 LINE(13)
#define LCD_LINE_14 LINE(14)
#define LCD_LINE_15 LINE(15)
#define LCD_LINE_16 LINE(16)
#define LCD_LINE_17 LINE(17)
#define LCD_LINE_18 LINE(18)
#define LCD_LINE_19 LINE(19)
#define LCD_LINE_20 LINE(20)
#define LCD_LINE_21 LINE(21)
#define LCD_LINE_22 LINE(22)
#define LCD_LINE_23 LINE(23)
#define LCD_LINE_24 LINE(24)
#define LCD_LINE_25 LINE(25)
#define LCD_LINE_26 LINE(26)
#define LCD_LINE_27 LINE(27)
#define LCD_LINE_28 LINE(28)
#define LCD_LINE_29 LINE(29)
#define LCD_LINE_30 LINE(30)
#define LCD_LINE_31 LINE(31)
#define LCD_LINE_32 LINE(32)
#define LCD_LINE_33 LINE(33)
#define LCD_LINE_34 LINE(34)
#define LCD_LINE_35 LINE(35)
#define LCD_LINE_36 LINE(36)
#define LCD_LINE_37 LINE(37)
#define LCD_LINE_38 LINE(38)
#define LCD_LINE_39 LINE(39)
#define LCD_LINE_40 LINE(40)
#define LCD_LINE_41 LINE(41)
#define LCD_LINE_42 LINE(42)
#define LCD_LINE_43 LINE(43)
#define LCD_LINE_44 LINE(44)
#define LCD_LINE_45 LINE(45)
#define LCD_LINE_46 LINE(46)
#define LCD_LINE_47 LINE(47)
#define LCD_LINE_48 LINE(48)
#define LCD_LINE_49 LINE(49)
#define LCD_LINE_50 LINE(50)
#define LCD_LINE_51 LINE(51)
#define LCD_LINE_52 LINE(52)
#define LCD_LINE_53 LINE(53)
#define LCD_LINE_54 LINE(54)
#define LCD_LINE_55 LINE(55)
#define LCD_LINE_56 LINE(56)
#define LCD_LINE_57 LINE(57)
#define LCD_LINE_58 LINE(58)
#define LCD_LINE_59 LINE(59)

#define LCD_LINEDIR_HORIZONTAL 0x0000
#define LCD_LINEDIR_VERTICAL   0x0001

// #define LCD_PIXEL_WIDTH  ((uint16_t)480)
// #define LCD_PIXEL_HEIGHT ((uint16_t)272)

#define LCD_DEFAULT_FONT Font16x24

#define HorizontalScreen 1 /* Set it to 0 and VerticalScreen to 1 can change screen into vertical */
#define VerticalScreen   0 /* NOT USED IN GD32F450 */

typedef struct {
    int16_t x;
    int16_t y;
} point_struct, *ptr_point;

/* initialize the LCD GPIO and TLI */
void lcd_init(void);
/* initialize TLI layer0 or layer1 */
void lcd_layer_init(uint32_t layer);
/*enable the LCD layer0 or layer1*/
void lcd_layer_enable(uint32_t layer);
/*disable the LCD layer0 or layer1*/
void lcd_layer_disable(uint32_t layer);
/*set the LCD layer*/
void lcd_layer_set(uint32_t layer);
/*get the LCD layer*/
uint32_t lcd_layer_get(void);

/* get the LCD color data with specified lcd_color_enum color */
uint32_t lcd_get_color_value(lcd_color_enum color);
/* clear the LCD with specified color */
void lcd_clear(uint32_t color);
/* set the text font */
void lcd_font_set(const font_t *font);
/* get the text font */
const font_t *lcd_font_get(void);
/* set the text color */
void lcd_text_color_set(uint32_t color);
/* get the current text color */
uint16_t lcd_text_color_get(void);
/* set the background color */
void lcd_background_color_set(uint32_t color);
/* get the current background color */
uint16_t lcd_background_color_get(void);

/* draw a line on LCD */
void lcd_line_draw(uint16_t xpos, uint16_t ypos, uint16_t length, uint8_t line_direction);
/* draw a rectangle on LCD */
void lcd_rectangle_draw(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height);
/* fill the whole rectangle */
void lcd_rectangle_fill(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height);
/* draw a circle on LCD using Bresenham algorithm */
void lcd_circle_draw(uint16_t xpos, uint16_t ypos, uint16_t radius);
/* draw a ellipse on LCD using the midpoint ellipse algorithm */
void lcd_ellipse_draw(uint16_t xpos, uint16_t ypos, uint16_t axis1, uint16_t axis2);

/* display the character on LCD */
void lcd_char_display(uint16_t line, uint16_t column, char ascii);
/* display the string on LCD */
void lcd_string_display(uint16_t xpos, uint16_t ypos, const char *ptr);
/* display the vertical string on LCD */
void lcd_vertical_string_display(uint16_t xpos, uint16_t ypos, const char *ptr);

#ifdef __cplusplus
}
#endif

#endif /* TFTLCD_H */
