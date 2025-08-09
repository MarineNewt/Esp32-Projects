#ifndef USER_SETUP_LOADED
#define USER_SETUP_LOADED 1
#endif

// Custom setup for TTGO T-Display (ST7789 1.14")

#define ST7789_DRIVER

#define TFT_WIDTH  135
#define TFT_HEIGHT 240

#define CGRAM_OFFSET      // Fixes display offset

#define TFT_MISO -1
#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_CS    5
#define TFT_DC   16
#define TFT_RST  23
#define TFT_BL    4   // Optional backlight control

#define LOAD_GLCD
#define LOAD_GFXFF
#define SMOOTH_FONT

#define SPI_FREQUENCY  40000000
#define SPI_READ_FREQUENCY  20000000
