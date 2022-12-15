#include <string.h>

#include "exmc_sdram.h"
#include "gd32f4xx_tli.h"
#include "systick.h"
#include "tftlcd.h"

// #define TFT43IN480272PIX
// #define TFT50IN480272PIX
#define TFT50IN800480PIX
// #define TFT70IN800480PIX

/* Physical LCD timings */
#ifdef TFT43IN480272PIX
#define XSIZE_PHYS 480// LCD水平分辨率
#define YSIZE_PHYS 272// LCD垂直分辨率
// 福瑞达
#define DCLK 12// DCLK Frequency
#define HSW  1 // HSYNC width水平同步宽度 行同步信号宽度
#define HBP  43// HSYNC back porch水平后沿周期 水平同步信号开始到行有效数据开始之间的相素时钟（CLK）个数
#define HFP  2 // HSYNC front porch水平前沿周期
#define VSW  1 // VSYNC width垂直同步宽度 垂直同步信号宽度
#define VBP  12// VSYNC back porch垂直后沿周期 垂直同步信号后，无效行的个数
#define VFP  1 // VSYNC front porch垂直前沿周期
#elif defined TFT50IN480272PIX
#define XSIZE_PHYS 480// LCD水平分辨率
#define YSIZE_PHYS 272// LCD垂直分辨率
// 海创通科技 福瑞达 三元晶
#define DCLK       9  // DCLK Frequency
#define HSW        4  // HSYNC width水平同步宽度 行同步信号宽度
#define HBP        40// HSYNC back porch水平后沿周期 水平同步信号开始到行有效数据开始之间的相素时钟（CLK）个数
#define HFP        4 // HSYNC front porch水平前沿周期
#define VSW        3 // VSYNC width垂直同步宽度 垂直同步信号宽度
#define VBP        5 // VSYNC back porch垂直后沿周期 垂直同步信号后，无效行的个数
#define VFP        2 // VSYNC front porch垂直前沿周期
// 佳显电子
#define DCLK       9 // DCLK Frequency
#define HSW        2 // HSYNC width水平同步宽度 行同步信号宽度
#define HBP        4// HSYNC back porch水平后沿周期 水平同步信号开始到行有效数据开始之间的相素时钟（CLK）个数
#define HFP        2// HSYNC front porch水平前沿周期
#define VSW        1// VSYNC width垂直同步宽度 垂直同步信号宽度
#define VBP        2// VSYNC back porch垂直后沿周期 垂直同步信号后，无效行的个数
#define VFP        1// VSYNC front porch垂直前沿周期
#elif defined TFT50IN800480PIX
#define XSIZE_PHYS 800// LCD水平分辨率
#define YSIZE_PHYS 480// LCD垂直分辨率
// 海创通科技
//  #define DCLK       33 // DCLK Frequency
//  #define HSW        1  // HSYNC width水平同步宽度 行同步信号宽度
//  #define HBP        46// HSYNC back porch水平后沿周期 水平同步信号开始到行有效数据开始之间的相素时钟（CLK）个数
//  #define HFP        16// HSYNC front porch水平前沿周期
//  #define VSW        1 // VSYNC width垂直同步宽度 垂直同步信号宽度
//  #define VBP        23// VSYNC back porch垂直后沿周期 垂直同步信号后，无效行的个数
//  #define VFP        7 // VSYNC front porch垂直前沿周期
// 大显1
//  #define DCLK       37// DCLK Frequency
//  #define HSW        3 // HSYNC width水平同步宽度 行同步信号宽度
//  #define HBP        40// HSYNC back porch水平后沿周期 水平同步信号开始到行有效数据开始之间的相素时钟（CLK）个数
//  #define HFP        40// HSYNC front porch水平前沿周期
//  #define VSW        3 // VSYNC width垂直同步宽度 垂直同步信号宽度
//  #define VBP        29// VSYNC back porch垂直后沿周期 垂直同步信号后，无效行的个数
//  #define VFP        13// VSYNC front porch垂直前沿周期
// 大显2
#define DCLK       37 // DCLK Frequency
#define HSW        3  // HSYNC width水平同步宽度 行同步信号宽度
#define HBP        100// HSYNC back porch水平后沿周期 水平同步信号开始到行有效数据开始之间的相素时钟（CLK）个数
#define HFP        6  // HSYNC front porch水平前沿周期
#define VSW        3  // VSYNC width垂直同步宽度 垂直同步信号宽度
#define VBP        20 // VSYNC back porch垂直后沿周期 垂直同步信号后，无效行的个数
#define VFP        6  // VSYNC front porch垂直前沿周期
#elif defined TFT70IN800480PIX
#define XSIZE_PHYS 800// LCD水平分辨率
#define YSIZE_PHYS 480// LCD垂直分辨率
// 思坦德 群创光电
#define DCLK       33 // DCLK Frequency
#define HSW        1  // HSYNC width水平同步宽度 行同步信号宽度
#define HBP        46// HSYNC back porch水平后沿周期 水平同步信号开始到行有效数据开始之间的相素时钟（CLK）个数
#define HFP        16// HSYNC front porch水平前沿周期
#define VSW        1 // VSYNC width垂直同步宽度 垂直同步信号宽度
#define VBP        23// VSYNC back porch垂直后沿周期 垂直同步信号后，无效行的个数
#define VFP        7 // VSYNC front porch垂直前沿周期
#else
#error "The TFT LCD type has not been defined"
#endif

// calculate frame buffer szie
#define LAYER0_PIXEL_FORMAT        LAYER_PPF_RGB565
#define LAYER0_PIXEL_BYTES         2// note: layer packeted pixel format
#define LAYER0_FRAME_BUFFER        SDRAM_DEVICE0_ADDR
#define LAYER0_FRAME_BUFFER_OFFSET ((uint32_t)(XSIZE_PHYS * YSIZE_PHYS * LAYER0_PIXEL_BYTES))
#define LAYER1_PIXEL_FORMAT        LAYER_PPF_RGB565
#define LAYER1_PIXEL_BYTES         2// note: layer packeted pixel format
#define LAYER1_FRAME_BUFFER        (LAYER0_FRAME_BUFFER + LAYER0_FRAME_BUFFER_OFFSET)
#define LAYER1_FRAME_BUFFER_OFFSET ((uint32_t)(XSIZE_PHYS * YSIZE_PHYS * LAYER1_PIXEL_BYTES))

static const font_t *current_font      = &font_6x8;
static uint32_t      current_textcolor = 0xffff;
static uint32_t      current_backcolor = 0x0000;
static uint32_t      current_layer     = LCD_LAYER_BACKGROUND;

/**
    \brief      initialize the LCD GPIO and TLI
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lcd_init(void)
{
    tli_parameter_struct tli_init_struct;

    tli_layer_disable(LAYER0);
    tli_layer_disable(LAYER1);

    /* configure HSYNC(PC6), VSYNC(PA4), PCLK(PG7), DE(PF10) */
    gpio_af_set(GPIOC, GPIO_AF_14, GPIO_PIN_6);// HSYNC
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_4);// VSYNC
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);

    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_7);// PCLK
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

    gpio_af_set(GPIOF, GPIO_AF_14, GPIO_PIN_10);// DE
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* configure LCD_R7(PG6), LCD_R6(PB1), LCD_R5(PA12), LCD_R4(PA11), LCD_R3(PB0),
                 LCD_G7(PD3), LCD_G6(PC7), LCD_G5(PB11), LCD_G4(PB10), LCD_G3(PG10), LCD_G2(PA6),
                 LCD_B7(PB9), LCD_B6(PB8), LCD_B5(PA3), LCD_B4(PG12), LCD_B3(PG11) */
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_6);// R7
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_0 | GPIO_PIN_1);// R3 R6
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);

    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_11 | GPIO_PIN_12);// R4 R5
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11 | GPIO_PIN_12);

    gpio_af_set(GPIOD, GPIO_AF_14, GPIO_PIN_3);// G7
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    gpio_af_set(GPIOC, GPIO_AF_14, GPIO_PIN_7);// G6
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

    gpio_af_set(GPIOB, GPIO_AF_14, GPIO_PIN_10 | GPIO_PIN_11);// G4 G5
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10 | GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_11);

    gpio_af_set(GPIOG, GPIO_AF_9, GPIO_PIN_10);// G3
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_6);// G2
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    gpio_af_set(GPIOB, GPIO_AF_14, GPIO_PIN_8 | GPIO_PIN_9);// B6 B7
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8 | GPIO_PIN_9);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_9);

    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_3);// B5
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_11);// B3
    gpio_af_set(GPIOG, GPIO_AF_9, GPIO_PIN_12); // B4
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_12);

    /* configure BL(PC8) DIP(PC9)*/
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_8 | GPIO_PIN_9);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_9);
    gpio_bit_reset(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
    delay_1ms(10);
    gpio_bit_set(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);

    /* TLI initialization */
    tli_init_struct.signalpolarity_hs      = TLI_HSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_vs      = TLI_VSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_de      = TLI_DE_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_pixelck = TLI_PIXEL_CLOCK_TLI;

    /* LCD display timing configuration */
    tli_init_struct.synpsz_hpsz   = HSW - 1;
    tli_init_struct.synpsz_vpsz   = VSW - 1;
    tli_init_struct.backpsz_hbpsz = HBP - 1;
    tli_init_struct.backpsz_vbpsz = VBP - 1;
    tli_init_struct.activesz_hasz = XSIZE_PHYS + HBP - 1;
    tli_init_struct.activesz_vasz = YSIZE_PHYS + VBP - 1;
    tli_init_struct.totalsz_htsz  = XSIZE_PHYS + HBP + HFP - 1;
    tli_init_struct.totalsz_vtsz  = YSIZE_PHYS + VBP + VFP - 1;

    /* LCD background color configure, 采用RGB三色，全为0是黑色*/
    tli_init_struct.backcolor_red   = 0x00;
    tli_init_struct.backcolor_green = 0x00;
    tli_init_struct.backcolor_blue  = 0x00;
    tli_init(&tli_init_struct);
    tli_enable();
}

/**
    \brief      initialize TLI layer0 or layer1
    \param[in]  layer: LCD layer
      \arg        LCD_LAYER_BACKGROUND
      \arg        LCD_LAYER_FOREGROUND
    \param[out] none
    \retval     none
*/
void lcd_layer_init(uint32_t layer)
{
    tli_layer_parameter_struct tli_layer_init_struct;

    /* TLI layer configuration */
    tli_layer_init_struct.layer_window_leftpos   = HBP;
    tli_layer_init_struct.layer_window_rightpos  = XSIZE_PHYS + HBP - 1;
    tli_layer_init_struct.layer_window_toppos    = VBP;
    tli_layer_init_struct.layer_window_bottompos = YSIZE_PHYS + VBP - 1;
    tli_layer_init_struct.layer_sa               = 0xff;// 恒定 Alpha,可用于计算混合因子 0为全不透
    tli_layer_init_struct.layer_default_blue     = 0x00;
    tli_layer_init_struct.layer_default_green    = 0x00;
    tli_layer_init_struct.layer_default_red      = 0x00;
    tli_layer_init_struct.layer_default_alpha    = 0x00;
    tli_layer_init_struct.layer_acf1             = LAYER_ACF1_PASA;
    tli_layer_init_struct.layer_acf2             = LAYER_ACF2_PASA;

    tli_layer_init_struct.layer_frame_total_line_number = YSIZE_PHYS;
    if (LCD_LAYER_BACKGROUND == layer) {
        /* TLI layer0 configuration */
        tli_layer_init_struct.layer_ppf                     = LAYER0_PIXEL_FORMAT;
        tli_layer_init_struct.layer_frame_line_length       = ((XSIZE_PHYS * LAYER0_PIXEL_BYTES) + 3);
        tli_layer_init_struct.layer_frame_buf_stride_offset = (XSIZE_PHYS * LAYER0_PIXEL_BYTES);
        tli_layer_init_struct.layer_frame_bufaddr           = LAYER0_FRAME_BUFFER;
        tli_layer_init(LAYER0, &tli_layer_init_struct);
    } else if (LCD_LAYER_FOREGROUND == layer) {
        /* TLI layer1 configuration */
        tli_layer_init_struct.layer_ppf                     = LAYER1_PIXEL_FORMAT;
        tli_layer_init_struct.layer_frame_line_length       = ((XSIZE_PHYS * LAYER1_PIXEL_BYTES) + 3);
        tli_layer_init_struct.layer_frame_buf_stride_offset = (XSIZE_PHYS * LAYER1_PIXEL_BYTES);
        tli_layer_init_struct.layer_frame_bufaddr           = LAYER1_FRAME_BUFFER;
        tli_layer_init(LAYER1, &tli_layer_init_struct);
    }
}

/**
    \brief      enable the LCD layer0 or layer1
    \param[in]  layer: LCD layer
      \arg        LCD_LAYER_BACKGROUND
      \arg        LCD_LAYER_FOREGROUND
    \param[out] none
    \retval     none
*/
void lcd_layer_enable(uint32_t layer)
{
    if (LCD_LAYER_BACKGROUND == layer) {
        tli_layer_enable(LAYER0);
    } else if (LCD_LAYER_FOREGROUND == layer) {
        tli_layer_enable(LAYER1);
    }
    tli_reload_config(TLI_FRAME_BLANK_RELOAD_EN);
}

/**
    \brief      disable the LCD layer0 or layer1
    \param[in]  layer: LCD layer
      \arg        LCD_LAYER_BACKGROUND
      \arg        LCD_LAYER_FOREGROUND
    \param[out] none
    \retval     none
*/
void lcd_layer_disable(uint32_t layer)
{
    if (LCD_LAYER_BACKGROUND == layer) {
        tli_layer_disable(LAYER0);
    } else if (LCD_LAYER_FOREGROUND == layer) {
        tli_layer_disable(LAYER1);
    }
    tli_reload_config(TLI_FRAME_BLANK_RELOAD_EN);
}

/**
    \brief      set the LCD layer
    \param[in]  layer: LCD layer
      \arg        LCD_LAYER_BACKGROUND
      \arg        LCD_LAYER_FOREGROUND
    \param[out] none
    \retval     none
*/
void lcd_layer_set(uint32_t layer)
{
    if (LCD_LAYER_BACKGROUND != layer && LCD_LAYER_FOREGROUND != layer)
        return;
    current_layer = layer;
}

/**
    \brief      get the LCD layer
    \param[in]  none
    \param[out] none
    \retval     layer: LCD layer
      \arg        LCD_LAYER_BACKGROUND
      \arg        LCD_LAYER_FOREGROUND
*/
uint32_t lcd_layer_get(void)
{
    return current_layer;
}

/**
    \brief      get the LCD color data with specified lcd_color_enum color
    \param[in]  color: LCD color, lcd_color_enum
      \arg        lcd_color_white
      \arg        lcd_color_black
      \arg        lcd_color_grey
      \arg        lcd_color_blue
      \arg        lcd_color_blue2
      \arg        lcd_color_red
      \arg        lcd_color_magenta
      \arg        lcd_color_green
      \arg        lcd_color_cyan
      \arg        lcd_color_yellow
    \param[out] color: LCD color, data
      \arg        LCD_COLOR_WHITE
      \arg        LCD_COLOR_BLACK
      \arg        LCD_COLOR_GREY
      \arg        LCD_COLOR_BLUE
      \arg        LCD_COLOR_BLUE2
      \arg        LCD_COLOR_RED
      \arg        LCD_COLOR_MAGENTA
      \arg        LCD_COLOR_GREEN
      \arg        LCD_COLOR_CYAN
      \arg        LCD_COLOR_YELLOW
    \retval     none
*/
uint32_t lcd_get_color_value(lcd_color_enum color)
{

    uint32_t frame = current_layer == LCD_LAYER_BACKGROUND ? LAYER0_FRAME_BUFFER : LAYER1_FRAME_BUFFER;

    switch (frame) {
    case LAYER_PPF_ARGB8888:
        switch (color) {
        case lcd_color_white:
            return 0xFFFFFFFF;
            break;
        case lcd_color_grey:
            return 0xFFBEBEBE;
            break;
        case lcd_color_blue:
            return 0xFF0000FF;
            break;
        case lcd_color_blue2:
            return 0xFF4169E1;
            break;
        case lcd_color_red:
            return 0xFFFF0000;
            break;
        case lcd_color_magenta:
            return 0xFFFF00FF;
            break;
        case lcd_color_green:
            return 0xFF00FF00;
            break;
        case lcd_color_cyan:
            return 0xFF00FFFF;
            break;
        case lcd_color_yellow:
            return 0xFFFFFF00;
            break;
        default:
            // lcd_color_black
            return 0xFF000000;
            break;
        }
        break;
    case LAYER_PPF_ARGB1555:
        switch (color) {
        case lcd_color_white:
            return 0xFFFF;
            break;
        case lcd_color_grey:
            return 0xFB9C;
            break;
        case lcd_color_blue:
        case lcd_color_blue2:
            return 0x801F;
            break;
        case lcd_color_red:
            return 0xFC00;
            break;
        case lcd_color_magenta:
            return 0xFC1F;
            break;
        case lcd_color_green:
            return 0x83E0;
            break;
        case lcd_color_cyan:
            return 0x83FF;
            break;
        case lcd_color_yellow:
            return 0xFFE0;
            break;
        default:
            // lcd_color_black
            return 0x8000;
            break;
        }
        break;
    case LAYER_PPF_ARGB4444:
        switch (color) {
        case lcd_color_white:
            return 0xFFFF;
            break;
        case lcd_color_grey:
            return 0xFFEE;
            break;
        case lcd_color_blue:
        case lcd_color_blue2:
            return 0xF00F;
            break;
        case lcd_color_red:
            return 0xFF00;
            break;
        case lcd_color_magenta:
            return 0xFF0F;
            break;
        case lcd_color_green:
            return 0xF0F0;
            break;
        case lcd_color_cyan:
            return 0xF0FF;
            break;
        case lcd_color_yellow:
            return 0xFFF0;
            break;
        default:
            // lcd_color_black
            return 0xF000;
            break;
        }
        break;
    default:
        // LAYER_PPF_RGB565
        switch (color) {
        case lcd_color_white:
            return 0xFFFF;
            break;
        case lcd_color_grey:
            return 0xF7DE;
            break;
        case lcd_color_blue:
            return 0x001F;
            break;
        case lcd_color_blue2:
            return 0x051F;
            break;
        case lcd_color_red:
            return 0xF800;
            break;
        case lcd_color_magenta:
            return 0xF81F;
            break;
        case lcd_color_green:
            return 0x07E0;
            break;
        case lcd_color_cyan:
            return 0x7FFF;
            break;
        case lcd_color_yellow:
            return 0xFFE0;
            break;
        default:
            // lcd_color_black
            return 0x0000;
            break;
        }
        break;
    }
}

/**
    \brief      clear the LCD with specified color
    \param[in]  color: LCD color, value
      \arg        LCD_COLOR_WHITE
      \arg        LCD_COLOR_BLACK
      \arg        LCD_COLOR_GREY
      \arg        LCD_COLOR_BLUE
      \arg        LCD_COLOR_BLUE2
      \arg        LCD_COLOR_RED
      \arg        LCD_COLOR_MAGENTA
      \arg        LCD_COLOR_GREEN
      \arg        LCD_COLOR_CYAN
      \arg        LCD_COLOR_YELLOW
    \param[out] none
    \retval     none
*/
void lcd_clear(uint32_t color)
{
    uint32_t color_tmp      = 0;
    uint8_t  color_step     = 0;
    uint8_t  color_size     = 0;// byte size
    uint8_t  color_size_tmp = 0;

    uint32_t format = current_layer == LCD_LAYER_BACKGROUND ? LAYER0_PIXEL_FORMAT : LAYER1_PIXEL_FORMAT;
    uint32_t frame  = current_layer == LCD_LAYER_BACKGROUND ? LAYER0_FRAME_BUFFER : LAYER1_FRAME_BUFFER;
    uint32_t frame_size =
        current_layer == LCD_LAYER_BACKGROUND ? LAYER0_FRAME_BUFFER_OFFSET : LAYER1_FRAME_BUFFER_OFFSET;

    switch (format) {
    case LAYER_PPF_ARGB8888:
        color_size = 4;
        break;

    case LAYER_PPF_RGB888:
        color_size = 3;
        break;

    case LAYER_PPF_RGB565 ... LAYER_PPF_ARGB4444:
    case LAYER_PPF_AL88:
        color_size = 2;
        break;

    case LAYER_PPF_L8 ... LAYER_PPF_AL44:
        color_size = 1;
        break;

    default:
        color_size = 2;
        break;
    }

    for (uint32_t index = 0; index < frame_size;) {
        color_tmp      = color;
        color_size_tmp = color_size;

        while (color_size_tmp) {
            if (color_size_tmp > 1) {
                color_step = 2;

                *(uint16_t *)(frame + (index)) = (uint16_t)(color_tmp);
            } else {
                color_step = 1;

                *(uint8_t *)(frame + (index)) = (uint8_t)(color_tmp);
            }
            index += color_step;
            color_tmp >>= color_step * 8;
            color_size_tmp -= color_step;
        }
    }
}

/**
    \brief      set the text font
    \param[in]  font: the text font
    \param[out] none
    \retval     none
*/
void lcd_font_set(const font_t *font)
{
    current_font = font;
}

/**
    \brief      get the text font
    \param[in]  none
    \param[out] none
    \retval     the text font
*/
const font_t *lcd_font_get(void)
{
    return current_font;
}

/**
    \brief      set the text color
    \param[in]  color: LCD color
      \arg        LCD_COLOR_WHITE
      \arg        LCD_COLOR_BLACK
      \arg        LCD_COLOR_GREY
      \arg        LCD_COLOR_BLUE
      \arg        LCD_COLOR_BLUE2
      \arg        LCD_COLOR_RED
      \arg        LCD_COLOR_MAGENTA
      \arg        LCD_COLOR_GREEN
      \arg        LCD_COLOR_CYAN
      \arg        LCD_COLOR_YELLOW
    \param[out] none
    \retval     none
*/
void lcd_text_color_set(uint32_t color)
{
    current_textcolor = color;
}

/**
    \brief      get the current text color
    \param[in]  none
    \param[out] none
    \retval     LCD color
      \arg        LCD_COLOR_WHITE
      \arg        LCD_COLOR_BLACK
      \arg        LCD_COLOR_GREY
      \arg        LCD_COLOR_BLUE
      \arg        LCD_COLOR_BLUE2
      \arg        LCD_COLOR_RED
      \arg        LCD_COLOR_MAGENTA
      \arg        LCD_COLOR_GREEN
      \arg        LCD_COLOR_CYAN
      \arg        LCD_COLOR_YELLOW
*/
uint16_t lcd_text_color_get(void)
{
    return current_textcolor;
}

/**
    \brief      set the background color
    \param[in]  color: LCD color
      \arg        LCD_COLOR_WHITE
      \arg        LCD_COLOR_BLACK
      \arg        LCD_COLOR_GREY
      \arg        LCD_COLOR_BLUE
      \arg        LCD_COLOR_BLUE2
      \arg        LCD_COLOR_RED
      \arg        LCD_COLOR_MAGENTA
      \arg        LCD_COLOR_GREEN
      \arg        LCD_COLOR_CYAN
      \arg        LCD_COLOR_YELLOW
    \param[out] none
    \retval     none
*/
void lcd_background_color_set(uint32_t color)
{
    current_backcolor = color;
}

/**
    \brief      get the current background color
    \param[in]  none
    \param[out] none
    \retval     LCD color
      \arg        LCD_COLOR_WHITE
      \arg        LCD_COLOR_BLACK
      \arg        LCD_COLOR_GREY
      \arg        LCD_COLOR_BLUE
      \arg        LCD_COLOR_BLUE2
      \arg        LCD_COLOR_RED
      \arg        LCD_COLOR_MAGENTA
      \arg        LCD_COLOR_GREEN
      \arg        LCD_COLOR_CYAN
      \arg        LCD_COLOR_YELLOW
*/
uint16_t lcd_background_color_get(void)
{
    return current_backcolor;
}

// /**
//     \brief      set point with the specified position and color
//     \param[in]  xpos: position of x
//     \param[in]  ypos: position of y
//     \param[in]  color: LCD color
//       \arg        LCD_COLOR_WHITE
//       \arg        LCD_COLOR_BLACK
//       \arg        LCD_COLOR_GREY
//       \arg        LCD_COLOR_BLUE
//       \arg        LCD_COLOR_BLUE2
//       \arg        LCD_COLOR_RED
//       \arg        LCD_COLOR_MAGENTA
//       \arg        LCD_COLOR_GREEN
//       \arg        LCD_COLOR_CYAN
//       \arg        LCD_COLOR_YELLOW
//     \param[out] none
//     \retval     none
// */
// void lcd_point_set(uint16_t xpos, uint16_t ypos, uint32_t color)
// {
//     //    *(__IO uint16_t*)(current_framebuffer + 4*((XSIZE_PHYS*ypos) + xpos)) = (uint16_t)color;
//     //    *(__IO uint16_t*)(current_framebuffer + 4*((XSIZE_PHYS*ypos) + xpos) + 2) = (uint16_t)(color >> 16);
//     *(__IO uint16_t *)(current_framebuffer + 3 * ((XSIZE_PHYS * ypos) + xpos))    = (uint16_t)color;
//     *(__IO uint8_t *)(current_framebuffer + 3 * ((XSIZE_PHYS * ypos) + xpos) + 2) = (uint8_t)(color >> 16);
//     //    *(__IO uint16_t*)(current_framebuffer + 4*((XSIZE_PHYS*ypos) + xpos)) = (uint16_t)color;
//     //    *(__IO uint16_t*)(current_framebuffer + 4*((XSIZE_PHYS*ypos) + xpos) + 2) = (uint16_t)(color >> 16);
// }

// /**
//     \brief      get point value with the specified position
//     \param[in]  xpos: position of x
//     \param[in]  ypos: position of y
//     \param[out] none
//     \retval     value of point(x, y)
// */
// uint32_t lcd_point_get(uint16_t xpos, uint16_t ypos)
// {
//     uint32_t temp_color = 0;
//     //    temp_color = (*(__IO uint16_t*)(current_framebuffer + 4*((XSIZE_PHYS*ypos) + xpos) + 2) << 16);
//     //    temp_color |= *(__IO uint16_t*)(current_framebuffer + 4*((XSIZE_PHYS*ypos) + xpos));
//     temp_color = (*(__IO uint8_t *)(current_framebuffer + 3 * ((XSIZE_PHYS * ypos) + xpos) + 2) << 16);
//     temp_color |= *(__IO uint16_t *)(current_framebuffer + 3 * ((XSIZE_PHYS * ypos) + xpos));
//     return temp_color;
// }

/**
    \brief      set a pixel
    \param[in]  x: x position
    \param[in]  y: y position
    \param[out] none
    \retval     none
*/
static void pixel_set_backcolor(int16_t x, int16_t y)
{
    if (x < 0 || x > (XSIZE_PHYS - 1) || y < 0 || y > (YSIZE_PHYS - 1)) {
        return;
    }

    uint32_t format = current_layer == LCD_LAYER_BACKGROUND ? LAYER0_PIXEL_FORMAT : LAYER1_PIXEL_FORMAT;
    uint32_t frame  = current_layer == LCD_LAYER_BACKGROUND ? LAYER0_FRAME_BUFFER : LAYER1_FRAME_BUFFER;

    switch (format) {
    case LAYER_PPF_ARGB8888:
        *(uint16_t *)(frame + (y * XSIZE_PHYS + x) * sizeof(uint32_t)) = (uint16_t)(current_backcolor);
        *(uint16_t *)(frame + (y * XSIZE_PHYS + x) * sizeof(uint32_t) + sizeof(uint16_t)) =
            (uint16_t)(current_backcolor >> 16);
        break;

    case LAYER_PPF_RGB888:
        *(uint16_t *)(frame + (y * XSIZE_PHYS + x) * (sizeof(uint16_t) + sizeof(uint8_t))) =
            (uint16_t)(current_backcolor);
        *(uint8_t *)(frame + (y * XSIZE_PHYS + x) * (sizeof(uint16_t) + sizeof(uint8_t)) + sizeof(uint16_t)) =
            (uint8_t)(current_backcolor >> 16);
        break;

    case LAYER_PPF_RGB565 ... LAYER_PPF_ARGB4444:
    case LAYER_PPF_AL88:
        *(uint16_t *)(frame + (y * XSIZE_PHYS + x) * sizeof(uint16_t)) = (uint16_t)(current_backcolor);
        break;

    case LAYER_PPF_L8 ... LAYER_PPF_AL44:
        *(uint8_t *)(frame + (y * XSIZE_PHYS + x) * sizeof(uint8_t)) = (uint8_t)(current_backcolor);
        break;

    default:
        *(uint16_t *)(frame + (y * XSIZE_PHYS + x) * sizeof(uint16_t)) = (uint16_t)(current_backcolor);
        break;
    }
} /**
     \brief      set a pixel
     \param[in]  x: x position
     \param[in]  y: y position
     \param[out] none
     \retval     none
 */
static void pixel_set_textcolor(int16_t x, int16_t y)
{
    if (x < 0 || x > (XSIZE_PHYS - 1) || y < 0 || y > (YSIZE_PHYS - 1)) {
        return;
    }

    uint32_t format = current_layer == LCD_LAYER_BACKGROUND ? LAYER0_PIXEL_FORMAT : LAYER1_PIXEL_FORMAT;
    uint32_t frame  = current_layer == LCD_LAYER_BACKGROUND ? LAYER0_FRAME_BUFFER : LAYER1_FRAME_BUFFER;

    switch (format) {
    case LAYER_PPF_ARGB8888:
        *(uint16_t *)(frame + (y * XSIZE_PHYS + x) * sizeof(uint32_t)) = (uint16_t)(current_textcolor);
        *(uint16_t *)(frame + (y * XSIZE_PHYS + x) * sizeof(uint32_t) + sizeof(uint16_t)) =
            (uint16_t)(current_textcolor >> 16);
        break;

    case LAYER_PPF_RGB888:
        *(uint16_t *)(frame + (y * XSIZE_PHYS + x) * (sizeof(uint16_t) + sizeof(uint8_t))) =
            (uint16_t)(current_textcolor);
        *(uint8_t *)(frame + (y * XSIZE_PHYS + x) * (sizeof(uint16_t) + sizeof(uint8_t)) + sizeof(uint16_t)) =
            (uint8_t)(current_textcolor >> 16);
        break;

    case LAYER_PPF_RGB565 ... LAYER_PPF_ARGB4444:
    case LAYER_PPF_AL88:
        *(uint16_t *)(frame + (y * XSIZE_PHYS + x) * sizeof(uint16_t)) = (uint16_t)(current_textcolor);
        break;

    case LAYER_PPF_L8 ... LAYER_PPF_AL44:
        *(uint8_t *)(frame + (y * XSIZE_PHYS + x) * sizeof(uint8_t)) = (uint8_t)(current_textcolor);
        break;

    default:
        *(uint16_t *)(frame + (y * XSIZE_PHYS + x) * sizeof(uint16_t)) = (uint16_t)(current_textcolor);
        break;
    }
}

/**
    \brief      draw a line on LCD
    \param[in]  xpos: position of x
    \param[in]  ypos: position of y
    \param[in]  length: length of line
    \param[in]  line_direction: direction of line
      \arg        LCD_LINEDIR_HORIZONTAL
      \arg        LCD_LINEDIR_VERTICAL
    \param[out] none
    \retval     none
*/
void lcd_line_draw(uint16_t xpos, uint16_t ypos, uint16_t length, uint8_t line_direction)
{
    if (LCD_LINEDIR_HORIZONTAL == line_direction) {
        uint16_t x;
        for (x = xpos; x < xpos + length; x++) {
            pixel_set_textcolor(x, ypos);
        }
    } else {
        uint16_t y;
        for (y = ypos; y < ypos + length; y++) {
            pixel_set_textcolor(xpos, y);
        }
    }
}

/**
    \brief      draw a rectangle on LCD
    \param[in]  xpos: position of x
    \param[in]  ypos: position of y
    \param[in]  width: width of rectangle
    \param[in]  height: height of rectangle
    \param[out] none
    \retval     none
*/
void lcd_rectangle_draw(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height)
{
    /* draw horizontal lines */
    lcd_line_draw(xpos, ypos, width, LCD_LINEDIR_HORIZONTAL);
    lcd_line_draw(xpos, (ypos + height - 1), width, LCD_LINEDIR_HORIZONTAL);

    /* draw vertical lines */
    lcd_line_draw(xpos, ypos, height, LCD_LINEDIR_VERTICAL);
    lcd_line_draw((xpos + width - 1), ypos, height, LCD_LINEDIR_VERTICAL);
}

/**
    \brief      fill the whole rectangle
    \param[in]  xpos: position of x
    \param[in]  ypos: position of y
    \param[in]  width: width of the rectangle
    \param[in]  height: height of the rectangle
    \param[out] none
    \retval     none
*/
void lcd_rectangle_fill(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height)
{
    uint16_t x, y;

    for (x = xpos; x < xpos + width; x++) {
        for (y = ypos; y < ypos + height; y++) {
            pixel_set_textcolor(x, y);
        }
    }
}

// /**
//     \brief      draw a circle on LCD using Bresenham algorithm
//     \param[in]  xpos: position of x
//     \param[in]  ypos: position of y
//     \param[in]  radius: radius of the circle
//     \param[out] none
//     \retval     none
// */
// void lcd_circle_draw(uint16_t xpos, uint16_t ypos, uint16_t radius)
// {
//     int x, y, e;
//     e = 3 - 2 * radius;
//     x = 0;
//     y = radius;

//     /* set four pixel (x-r, y), (x+r, y), (x, y-r), (x, y-r) */
//     pixel_set_textcolor(-radius + xpos, ypos);
//     pixel_set_textcolor(radius + xpos, ypos);
//     pixel_set_textcolor(xpos, -radius + ypos);
//     pixel_set_textcolor(xpos, radius + ypos);

//     while (x <= y) {
//         if (e < 0) {
//             /* choose the right of the current pixel as the next pixel */
//             e = e + 4 * x + 6;
//             x++;
//         } else {
//             /* choose the right bottom of the current pixel as the next pixel */
//             e = e + 4 * (x - y) + 10;
//             x++;
//             y--;
//         }
//         pixel_set_textcolor(x + xpos, y + ypos);
//         pixel_set_textcolor(-x + xpos, y + ypos);
//         pixel_set_textcolor(-x + xpos, -y + ypos);
//         pixel_set_textcolor(x + xpos, -y + ypos);
//         pixel_set_textcolor(y + xpos, x + ypos);
//         pixel_set_textcolor(-y + xpos, x + ypos);
//         pixel_set_textcolor(-y + xpos, -x + ypos);
//         pixel_set_textcolor(y + xpos, -x + ypos);
//     }
// }

// /**
//     \brief      set plot point of ellipse
//     \param[in]  center_x: x position of ellipse center
//     \param[in]  center_y: y position of ellipse center
//     \param[in]  x: x value
//     \param[in]  y: y value
//     \param[out] none
//     \retval     none
// */
// static void plotpoint_set(int center_x, int center_y, int x, int y)
// {
//     pixel_set(center_x + x, center_y + y);
//     pixel_set(center_x - x, center_y + y);
//     pixel_set(center_x + x, center_y - y);
//     pixel_set(center_x - x, center_y - y);
// }

// /**
//     \brief      draw a ellipse on LCD using the midpoint ellipse algorithm
//     \param[in]  xpos: x position of ellipse center
//     \param[in]  ypos: y position of ellipse center
//     \param[in]  axis1: major axis
//     \param[in]  axis2: minor axis
//     \param[out] none
//     \retval     none
// */
// void lcd_ellipse_draw(uint16_t xpos, uint16_t ypos, uint16_t axis1, uint16_t axis2)
// {
//     int sq_axis1 = axis1 * axis1, sq_axis2 = axis2 * axis2;
//     int p;
//     int x = 0, y = axis2;
//     int px = 0, py = 2 * sq_axis1 * y;
//     /* draw four points on the long and short axis of the ellipse */
//     plotpoint_set(xpos, ypos, x, y);
//     /* calculate the initial value in area 1 */
//     p = (int)((sq_axis2 - (sq_axis1 * axis2) + (0.25 * sq_axis1)));
//     while (px < py) {
//         ++x;
//         px += 2 * sq_axis2;
//         if (p < 0) {
//             p += sq_axis2 + px;
//         } else {
//             --y;
//             py -= 2 * sq_axis1;
//             p += sq_axis2 + px - py;
//         }
//         plotpoint_set(xpos, ypos, x, y);
//     }
//     /* calculate the initial value with the last point calculated in the area 1 */
//     p = (int)((sq_axis2 * (x + 0.5) * (x + 0.5) + sq_axis1 * (y - 1) * (y - 1) - sq_axis1 * sq_axis2));
//     while (y > 0) {
//         --y;
//         py -= 2 * sq_axis1;
//         if (p > 0) {
//             p += sq_axis1 - py;
//         } else {
//             ++x;
//             px += 2 * sq_axis2;
//             p += sq_axis1 - py + px;
//         }
//         plotpoint_set(xpos, ypos, x, y);
//     }
// }

/**
    \brief      draw the character on LCD
    \param[in]  xpos: position of x
    \param[in]  ypos: position of y
    \param[in]  c: a pointer to the character
    \param[out] none
    \retval     none
*/
static void lcd_char_draw(uint16_t xpos, uint16_t ypos, const char *c)
{
    // uint8_t  *c_8   = (uint8_t *)c;
    // uint16_t *c_16  = (uint16_t *)c;
    // uint16_t index = 0;
    // uint16_t x = 0, y = 0;
    uint8_t  counter    = 0;
    uint32_t width_byte = current_font->width / 8 + ((current_font->width % 8) == 0 ? 0 : 1);

    for (uint16_t index = 0; index < current_font->height; index++) {
        counter = 0;
        for (uint16_t byte = 0; byte < width_byte; byte++) {
            for (uint8_t i = 0; i < 8; i++) {
                if ((c[index * width_byte + byte] << i) & 0x80)
                    pixel_set_textcolor(xpos + counter, ypos + index);
                else
                    pixel_set_backcolor(xpos + counter, ypos + index);
                counter++;
                if (counter >= current_font->width)
                    break;
            }
        }
    }
}

/**
    \brief      display the character on LCD
    \param[in]  line: line to display the character
      \arg        LCD_LINE_x(x=0..39 for 480*272)
    \param[in]  column: column address
    \param[in]  ascii: character ascii code(from 0x20 to 0x7E)
    \param[out] none
    \retval     none
*/
void lcd_char_display(uint16_t line, uint16_t column, char ascii)
{
    if (ascii < current_font->first_char_id || ascii > (current_font->first_char_id + current_font->charcount - 1))
        return;

    uint16_t width_byte = current_font->width / 8 + ((current_font->width % 8) == 0 ? 0 : 1);
    lcd_char_draw(line, column,
                  current_font->data + (ascii - current_font->first_char_id) * (width_byte * current_font->height));
}

/**
    \brief      display the string on LCD
    \param[in]  xpos: line to display the character
    \param[in]  ypos: column to display the character
    \param[in]  ptr: a pointer to the string
    \param[out] none
    \retval     none
*/
void lcd_string_display(uint16_t xpos, uint16_t ypos, const char *ptr)
{
    uint32_t index = 0;

    while (*ptr != 0) {
        /* display character on LCD */
        lcd_char_display(xpos + (current_font->width * index), ypos, *ptr);
        index++;
        ptr++;
    }
}

/**
    \brief      display the vertical string on LCD
    \param[in]  stringline: line to display the character
      \arg        LCD_LINE_x(x=0..39 for 480*272)
    \param[in]  offset: offset value
    \param[in]  ptr: a pointer to the string
    \param[out] none
    \retval     none
*/
void lcd_vertical_string_display(uint16_t xpos, uint16_t ypos, const char *ptr)
{
    uint32_t index = 0;

    while (*ptr != 0) {
        /* display character on LCD */
        lcd_char_display(xpos, ypos + (current_font->height * index), *ptr);
        index++;
        ptr++;
    }
}
