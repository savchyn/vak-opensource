/*
 * Demo of LED 8x32 matrix (with max7219 controller).
 *
 * Copyright (C) 2018 Serge Vakulenko
 * Based on ch341 sources by Pluto Yang (yangyj.ee@gmail.com).
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "max7219.h"

#define WIDTH   32
#define HEIGHT  8

long bitmap[HEIGHT];

int last_digit[6] = { -1, -1, -1, -1, -1, -1 };

#define ROW(a,b,c,d) (a<<3 | b<<2 | c<<1 | d)
#define _ 0
#define O 1

const unsigned char digits[] = {
	ROW (_,O,O,_),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (_,O,O,_),

	ROW (_,_,O,_),
	ROW (_,O,O,_),
	ROW (O,_,O,_),
	ROW (_,_,O,_),
	ROW (_,_,O,_),
	ROW (_,_,O,_),
	ROW (_,_,O,_),
	ROW (_,O,O,O),

	ROW (O,O,O,_),
	ROW (_,_,_,O),
	ROW (_,_,_,O),
	ROW (_,_,_,O),
	ROW (_,_,O,_),
	ROW (_,O,_,_),
	ROW (O,_,_,_),
	ROW (O,O,O,O),

	ROW (O,O,O,_),
	ROW (_,_,_,O),
	ROW (_,_,_,O),
	ROW (_,O,O,_),
	ROW (_,_,_,O),
	ROW (_,_,_,O),
	ROW (O,_,_,O),
	ROW (_,O,O,_),

	ROW (O,_,_,_),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (O,O,O,O),
	ROW (_,_,_,O),
	ROW (_,_,_,O),
	ROW (_,_,_,O),

	ROW (O,O,O,O),
	ROW (O,_,_,_),
	ROW (O,_,_,_),
	ROW (O,O,O,_),
	ROW (_,_,_,O),
	ROW (_,_,_,O),
	ROW (O,_,_,O),
	ROW (_,O,O,_),

	ROW (_,O,O,O),
	ROW (O,_,_,_),
	ROW (O,_,_,_),
	ROW (O,O,O,_),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (_,O,O,_),

	ROW (O,O,O,O),
	ROW (O,_,_,O),
	ROW (_,_,_,O),
	ROW (_,_,O,_),
	ROW (_,O,_,_),
	ROW (_,O,_,_),
	ROW (_,O,_,_),
	ROW (_,O,_,_),

	ROW (_,O,O,_),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (_,O,O,_),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (_,O,O,_),

	ROW (_,O,O,_),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (O,_,_,O),
	ROW (_,O,O,O),
	ROW (_,_,_,O),
	ROW (_,_,_,O),
	ROW (O,O,O,_),
};

//
// Send bitmap to display.
//
int write_bitmap()
{
    int i;

    for (i=0; i<HEIGHT; i++) {
        if (led_write_row(i, bitmap[i]) < 0)
            return -1;
    }
    return 0;
}

//
// Clear the digit.
//
void clear_digit(int xoff)
{
    int i;
    long mask = 0xf << (32 - 4 - xoff);

    for (i=0; i<HEIGHT; i++) {
        bitmap[i] &= ~mask;
    }
}

//
// Slide in one row of the glyph to the bitmap.
//
void slide_glyph_row(long data, int xoff)
{
    int i;
    long mask = 0xf << (32 - 4 - xoff);

    for (i=HEIGHT-1; i>=1; i--) {
        bitmap[i] &= ~mask;
        bitmap[i] |= bitmap[i-1] & mask;
    }
    bitmap[0] &= ~mask;
    bitmap[0] |= data << (32 - 4 - xoff);
}

//
// Update one digit of the clock image.
//
int slide_glyph(int index, int digit, int column)
{
    const unsigned char *glyph;
    int i;

    if (last_digit[index] == digit)
        return 0;
    glyph = &digits[digit * HEIGHT];
    last_digit[index] = digit;

    clear_digit(column);

    // Slide by one row at a time.
    for (i=0; i<HEIGHT; i++) {
        slide_glyph_row(glyph[HEIGHT-1-i], column);
        if (write_bitmap() < 0)
            return -1;
    }
    return 0;
}

//
// Update the clock image.
//
int update_time(time_t sec)
{
    struct tm *tm = localtime(&sec);

    //printf("%02d:%02d:%02d\n", tm->tm_hour, tm->tm_min, tm->tm_sec);

    slide_glyph(5, tm->tm_sec % 10, 27);
    slide_glyph(4, tm->tm_sec / 10, 22);
    slide_glyph(3, tm->tm_min % 10, 16);
    slide_glyph(2, tm->tm_min / 10, 11);
    slide_glyph(1, tm->tm_hour % 10, 5);
    return slide_glyph(0, tm->tm_hour / 10, 0);
}

int main(int argc, char *argv[])
{
    struct timeval tv;
    time_t last_sec = 0;

    if (led_init() < 0) {
        goto out;
    }

    for (;;) {
        gettimeofday(&tv, 0);
        if (tv.tv_sec == last_sec) {
            // Sleep for 20 msec.
            usleep(20000);
            continue;
        }

        last_sec = tv.tv_sec;
        if (update_time(last_sec) < 0)
            break;
    }

out:
    led_close();
    return 0;
}
