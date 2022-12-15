/*
 *  font.h -- `Soft' font definitions
 *
 *  Created 1995 by Geert Uytterhoeven
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 */

#ifndef _VIDEO_FONT_H
#define _VIDEO_FONT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define FONT_EXTRA_WORDS 4

/* Max. length for the name of a predefined font */
#define MAX_FONT_NAME 32

struct font_data {
    unsigned int        extra[FONT_EXTRA_WORDS];
    const unsigned char data[];
};

typedef struct font_desc {
    int          idx;
    const char  *name;
    unsigned int width, height;
    unsigned int first_char_id;
    unsigned int charcount;
    const void  *data;
    int          pref;
} font_t;

#define FONT6x8_IDX   0
#define FONT6x10_IDX  1
#define FONT6x11_IDX  2
#define FONT7x14_IDX  3
#define FONT8x8_IDX   4
#define FONT8x16_IDX  5
#define FONT10x18_IDX 6
#define MINI4x6_IDX   7
#define ACORN8x8_IDX  8
#define PEARL8x8_IDX  9
#define SUN8x16_IDX   10
#define SUN12x22_IDX  11
#define TER16x32_IDX  12

extern const font_t font_6x8;
extern const font_t font_6x10;
extern const font_t font_6x11;
extern const font_t font_7x14;
extern const font_t font_8x8;
extern const font_t font_10x18;
extern const font_t font_acorn_8x8;
extern const font_t font_mini_4x6;
extern const font_t font_pearl_8x8;
extern const font_t font_sun_8x16;
extern const font_t font_sun_12x22;
extern const font_t font_ter_16x32;

// /* Extra word getters */
// #define REFCOUNT(fd)   (((int *)(fd))[-1])
// #define FNTSIZE(fd)    (((int *)(fd))[-2])
// #define FNTCHARCNT(fd) (((int *)(fd))[-3])
// #define FNTSUM(fd)     (((int *)(fd))[-4])

#ifdef __cplusplus
}
#endif

#endif /* _VIDEO_FONT_H */
