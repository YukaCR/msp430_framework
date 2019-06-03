#define AdafruitGFX_Comp_H
#ifndef AdafruitGFX_Comp_H
#define AdafruitGFX_Comp_H
#include "stdint.h"
#include "AdafruitGFX_Fonts.h"
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
    {                       \
        int16_t t = a;      \
        a = b;              \
        b = t;              \
    }
#endif
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif
/// Font data stored PER GLYPH
typedef struct
{
    uint16_t bitmapOffset; ///< Pointer into GFXfont->bitmap
    uint8_t width;         ///< Bitmap dimensions in pixels
    uint8_t height;        ///< Bitmap dimensions in pixels
    uint8_t xAdvance;      ///< Distance to advance cursor (x axis)
    int8_t xOffset;        ///< X dist from cursor pos to UL corner
    int8_t yOffset;        ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct
{
    uint8_t *bitmap;  ///< Glyph bitmaps, concatenated
    GFXglyph *glyph;  ///< Glyph array
    uint8_t first;    ///< ASCII extents (first char)
    uint8_t last;     ///< ASCII extents (last char)
    uint8_t yAdvance; ///< Newline distance (y axis)
} GFXfont;
const int16_t
    WIDTH,  ///< This is the 'raw' display width - never changes
    HEIGHT; ///< This is the 'raw' display height - never changes
int16_t
    _width,   ///< Display width as modified by current rotation
    _height,  ///< Display height as modified by current rotation
    cursor_x, ///< x location to start print()ing text
    cursor_y; ///< y location to start print()ing text
uint16_t
    textcolor,   ///< 16-bit background color for print()
    textbgcolor; ///< 16-bit text color for print()
uint8_t
    textsize, ///< Desired magnification of text to print()
    rotation; ///< Display rotation (0 thru 3)
bool
    wrap,   ///< If set, 'wrap' text at right edge of display
    _cp437; ///< If set, use correct CP437 charset (default is off)
GFXfont
    *gfxFont; ///< Pointer to special font
void charBounds(char c, int16_t *x, int16_t *y,
                int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
int16_t height(void);
int16_t width(void);
uint8_t getRotation(void);

// get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
int16_t getCursorX(void);
int16_t getCursorY(void);
void drawPixel(int16_t x, int16_t y, uint16_t color);    ///< Virtual drawPixel() function to draw to the screen/framebuffer/etc, must be overridden in subclass. @param x X coordinate.  @param y Y coordinate. @param color 16-bit pixel color. 
void setRotation(uint8_t r);
void invertDisplay(bool i);
   void startWrite(void);
   void writePixel(int16_t x, int16_t y, uint16_t color);
   void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
   void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
   void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
   void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
   void endWrite(void);
void
// It's good to implement those, even if using transaction API
drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
    drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
    fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    fillScreen(uint16_t color),
    // Optional and probably not necessary to change
    drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                     uint16_t color),
    fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                     int16_t delta, uint16_t color),
    drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                 int16_t x2, int16_t y2, uint16_t color),
    fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                 int16_t x2, int16_t y2, uint16_t color),
    drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                  int16_t radius, uint16_t color),
    fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                  int16_t radius, uint16_t color),
    drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
               int16_t w, int16_t h, uint16_t color),
    drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
               int16_t w, int16_t h, uint16_t color, uint16_t bg),
    drawBitmap(int16_t x, int16_t y, uint8_t *bitmap,
               int16_t w, int16_t h, uint16_t color),
    drawBitmap(int16_t x, int16_t y, uint8_t *bitmap,
               int16_t w, int16_t h, uint16_t color, uint16_t bg),
    drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                int16_t w, int16_t h, uint16_t color),
    drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                        int16_t w, int16_t h),
    drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap,
                        int16_t w, int16_t h),
    drawGrayscaleBitmap(int16_t x, int16_t y,
                        const uint8_t bitmap[], const uint8_t mask[],
                        int16_t w, int16_t h),
    drawGrayscaleBitmap(int16_t x, int16_t y,
                        uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h),
    drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[],
                  int16_t w, int16_t h),
    drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap,
                  int16_t w, int16_t h),
    drawRGBBitmap(int16_t x, int16_t y,
                  const uint16_t bitmap[], const uint8_t mask[],
                  int16_t w, int16_t h),
    drawRGBBitmap(int16_t x, int16_t y,
                  uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h),
    drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
             uint16_t bg, uint8_t size),
    setCursor(int16_t x, int16_t y),
    setTextColor(uint16_t c),
    setTextColor(uint16_t c, uint16_t bg),
    setTextSize(uint8_t s),
    setTextWrap(bool w),
    cp437(bool x = true),
    setFont(const GFXfont *f = NULL),
    getTextBounds(const char *string, int16_t x, int16_t y,
                  int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h),
#endif