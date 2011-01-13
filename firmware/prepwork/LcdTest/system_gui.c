#include "helpers.h"
#include "system_gui.h"
#include "driver_lcd.h"


void drawPixel(uint8 x0, uint8 y0)
{
    uint8 lcd_x, lcd_y;
    uint8 rel_x;
    uint8 curRam_0, curRam_1;
    
    beginDraw();
    rel_x = xyToLcdXY(x0, y0, &lcd_x, &lcd_y);
    // set position
    writeInst(0x80 + lcd_y);
    writeInst(0x80 + lcd_x);
    // read ram
    curRam_0 = readRam();
    curRam_1 = readRam();
    // modify ram
    if (rel_x & 0x10) { // rel_x > 7, modify curRam_1
        curRam_1 |= 0x01 << (rel_x - 8);
    }
    else {
        curRam_0 |= 0x01 << (rel_x);
    }
    // write back to ram
    writeInst(0x80 + lcd_y);
    writeInst(0x80 + lcd_x);
    writeData(curRam_0);
    writeData(curRam_1);
    
    endDraw();
}

void drawLine(int x0, int y0, int x1, int y1)
{
    int x, y;
    int dx, dy;
    int sx, sy;
    int err, e2;
    
    x = x0;
    y = y0;
    dx = x1 - x0; if (dx < 0) dx = -dx; // dx = abs(x1 - x0)
    dy = y1 - y0; if (dy < 0) dy = -dy; // dy = abs(y1 - y0)
    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = dx - dy;
    
    while (1) {
        drawPixel(x, y);
        if (x == x1 || y == y1) break;
        e2 = 2 * err;
        if (e2 > -dy) {
            err = err - dy;
            x += sx;
        }
        if (e2 > dx) {
            err += dx;
            y += sy;
        }
    }
}
