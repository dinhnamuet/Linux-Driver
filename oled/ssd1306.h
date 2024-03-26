#ifndef __SSD1306_H__
#define __SSD1306_H__

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define FONT_X	6

static const uint8_t ssd1306_font[][FONT_X] = {
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // space
		{0x00, 0x00, 0x2f, 0x00, 0x00, 0x00}, // !
		{0x00, 0x07, 0x00, 0x07, 0x00, 0x00}, // "
		{0x14, 0x7f, 0x14, 0x7f, 0x14, 0x00}, // #
		{0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00}, // $
		{0x23, 0x13, 0x08, 0x64, 0x62, 0x00}, // %
		{0x36, 0x49, 0x55, 0x22, 0x50, 0x00}, // &
		{0x00, 0x05, 0x03, 0x00, 0x00, 0x00}, // '
		{0x00, 0x1c, 0x22, 0x41, 0x00, 0x00}, // (
		{0x00, 0x41, 0x22, 0x1c, 0x00, 0x00}, // )
		{0x14, 0x08, 0x3E, 0x08, 0x14, 0x00}, // *
		{0x08, 0x08, 0x3E, 0x08, 0x08, 0x00}, // +
		{0x00, 0x00, 0xA0, 0x60, 0x00, 0x00}, // ,
		{0x08, 0x08, 0x08, 0x08, 0x08, 0x00}, // -
		{0x00, 0x60, 0x60, 0x00, 0x00, 0x00}, // .
		{0x20, 0x10, 0x08, 0x04, 0x02, 0x00}, // /
		{0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00}, // 0
		{0x00, 0x42, 0x7F, 0x40, 0x00, 0x00}, // 1
		{0x42, 0x61, 0x51, 0x49, 0x46, 0x00}, // 2
		{0x21, 0x41, 0x45, 0x4B, 0x31, 0x00}, // 3
		{0x18, 0x14, 0x12, 0x7F, 0x10, 0x00}, // 4
		{0x27, 0x45, 0x45, 0x45, 0x39, 0x00}, // 5
		{0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00}, // 6
		{0x01, 0x71, 0x09, 0x05, 0x03, 0x00}, // 7
		{0x36, 0x49, 0x49, 0x49, 0x36, 0x00}, // 8
		{0x06, 0x49, 0x49, 0x29, 0x1E, 0x00}, // 9
		{0x00, 0x36, 0x36, 0x00, 0x00, 0x00}, // :
		{0x00, 0x56, 0x36, 0x00, 0x00, 0x00}, // ;
		{0x08, 0x14, 0x22, 0x41, 0x00, 0x00}, // <
		{0x14, 0x14, 0x14, 0x14, 0x14, 0x00}, // =
		{0x00, 0x41, 0x22, 0x14, 0x08, 0x00}, // >
		{0x02, 0x01, 0x51, 0x09, 0x06, 0x00}, // ?
		{0x32, 0x49, 0x59, 0x51, 0x3E, 0x00}, // @
		{0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00}, // A
		{0x7F, 0x49, 0x49, 0x49, 0x36, 0x00}, // B
		{0x3E, 0x41, 0x41, 0x41, 0x22, 0x00}, // C
		{0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00}, // D
		{0x7F, 0x49, 0x49, 0x49, 0x41, 0x00}, // E
		{0x7F, 0x09, 0x09, 0x09, 0x01, 0x00}, // F
		{0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00}, // G
		{0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00}, // H
		{0x00, 0x41, 0x7F, 0x41, 0x00, 0x00}, // I
		{0x20, 0x40, 0x41, 0x3F, 0x01, 0x00}, // J
		{0x7F, 0x08, 0x14, 0x22, 0x41, 0x00}, // K
		{0x7F, 0x40, 0x40, 0x40, 0x40, 0x00}, // L
		{0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00}, // M
		{0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00}, // N
		{0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00}, // O
		{0x7F, 0x09, 0x09, 0x09, 0x06, 0x00}, // P
		{0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00}, // Q
		{0x7F, 0x09, 0x19, 0x29, 0x46, 0x00}, // R
		{0x46, 0x49, 0x49, 0x49, 0x31, 0x00}, // S
		{0x01, 0x01, 0x7F, 0x01, 0x01, 0x00}, // T
		{0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00}, // U
		{0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00}, // V
		{0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00}, // W
		{0x63, 0x14, 0x08, 0x14, 0x63, 0x00}, // X
		{0x07, 0x08, 0x70, 0x08, 0x07, 0x00}, // Y
		{0x61, 0x51, 0x49, 0x45, 0x43, 0x00}, // Z
		{0x00, 0x7F, 0x41, 0x41, 0x00, 0x00}, // [
		{0x55, 0xAA, 0x55, 0xAA, 0x55, 0x00}, // Backslash (Checker pattern)
		{0x00, 0x41, 0x41, 0x7F, 0x00, 0x00}, // ]
		{0x04, 0x02, 0x01, 0x02, 0x04, 0x00}, // ^
		{0x40, 0x40, 0x40, 0x40, 0x40, 0x00}, // _
		{0x00, 0x03, 0x05, 0x00, 0x00, 0x00}, // `
		{0x20, 0x54, 0x54, 0x54, 0x78, 0x00}, // a
		{0x7F, 0x48, 0x44, 0x44, 0x38, 0x00}, // b
		{0x38, 0x44, 0x44, 0x44, 0x20, 0x00}, // c
		{0x38, 0x44, 0x44, 0x48, 0x7F, 0x00}, // d
		{0x38, 0x54, 0x54, 0x54, 0x18, 0x00}, // e
		{0x08, 0x7E, 0x09, 0x01, 0x02, 0x00}, // f
		{0x18, 0xA4, 0xA4, 0xA4, 0x7C, 0x00}, // g
		{0x7F, 0x08, 0x04, 0x04, 0x78, 0x00}, // h
		{0x00, 0x44, 0x7D, 0x40, 0x00, 0x00}, // i
		{0x40, 0x80, 0x84, 0x7D, 0x00, 0x00}, // j
		{0x7F, 0x10, 0x28, 0x44, 0x00, 0x00}, // k
		{0x00, 0x41, 0x7F, 0x40, 0x00, 0x00}, // l
		{0x7C, 0x04, 0x18, 0x04, 0x78, 0x00}, // m
		{0x7C, 0x08, 0x04, 0x04, 0x78, 0x00}, // n
		{0x38, 0x44, 0x44, 0x44, 0x38, 0x00}, // o
		{0xFC, 0x24, 0x24, 0x24, 0x18, 0x00}, // p
		{0x18, 0x24, 0x24, 0x18, 0xFC, 0x00}, // q
		{0x7C, 0x08, 0x04, 0x04, 0x08, 0x00}, // r
		{0x48, 0x54, 0x54, 0x54, 0x20, 0x00}, // s
		{0x04, 0x3F, 0x44, 0x40, 0x20, 0x00}, // t
		{0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00}, // u
		{0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00}, // v
		{0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00}, // w
		{0x44, 0x28, 0x10, 0x28, 0x44, 0x00}, // x
		{0x1C, 0xA0, 0xA0, 0xA0, 0x7C, 0x00}, // y
		{0x44, 0x64, 0x54, 0x4C, 0x44, 0x00}, // z
		{0x00, 0x10, 0x7C, 0x82, 0x00, 0x00}, // {
		{0x00, 0x00, 0xFF, 0x00, 0x00, 0x00}, // |
		{0x00, 0x82, 0x7C, 0x10, 0x00, 0x00}, // }
		{0x00, 0x06, 0x09, 0x09, 0x06}	// ~ (Degrees)
};
const uint8_t frames[][512] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,127,255,128,0,1,254,0,1,255,255,255,0,3,199,0,7,225,255,255,192,7,135,0,31,0,0,1,192,15,7,0,60,0,0,0,224,30,7,0,120,0,0,0,224,60,14,0,240,0,0,0,224,120,30,1,192,0,0,1,192,240,60,7,128,0,1,255,193,224,120,15,0,0,31,255,3,192,240,30,0,0,31,224,7,129,224,60,0,0,15,192,15,3,192,112,0,0,7,255,254,7,128,224,0,0,1,255,252,15,0,112,0,0,0,1,248,30,0,56,0,0,0,0,0,60,0,60,0,0,0,0,0,120,0,28,0,0,0,0,0,240,0,14,0,0,0,0,1,224,0,7,0,0,0,0,1,192,0,3,128,28,0,0,3,128,0,1,192,127,255,255,15,0,0,1,225,255,255,255,254,0,0,0,227,192,0,255,248,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,28,0,0,0,0,0,0,0,8,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,127,224,0,0,0,252,0,3,255,255,252,0,1,254,0,7,247,255,255,128,3,199,0,31,0,0,255,192,7,135,0,60,0,0,0,224,15,7,0,120,0,0,0,96,30,7,0,240,0,0,0,96,60,14,3,192,0,0,0,224,120,30,7,128,0,0,1,224,240,60,15,0,0,31,255,193,224,120,28,0,0,31,255,3,192,240,120,0,0,15,128,7,129,224,240,0,0,7,252,15,3,192,112,0,0,1,255,222,7,128,112,0,0,0,31,252,7,0,56,0,0,0,0,248,14,0,28,0,0,0,0,0,28,0,14,0,0,0,0,0,56,0,14,0,0,0,0,0,112,0,7,0,0,0,0,0,224,0,3,128,24,0,0,1,192,0,1,192,127,252,0,3,128,0,1,193,255,255,254,7,0,0,0,227,192,127,255,254,0,0,0,119,128,0,63,252,0,0,0,62,0,0,0,0,0,0,0,28,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,126,63,128,0,0,0,0,0,195,240,192,0,0,0,0,1,128,192,96,0,0,0,0,1,0,192,32,0,0,0,0,1,0,0,48,0,0,0,0,3,0,0,48,0,0,0,0,3,0,0,32,0,0,0,0,1,0,0,32,0,0,0,0,1,128,0,96,0,0,0,0,0,128,0,64,0,0,0,0,0,64,0,128,0,0,0,0,0,96,1,128,0,0,0,0,0,48,3,0,0,0,0,0,0,24,6,0,0,0,0,0,0,14,24,0,0,0,0,0,0,3,48,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,56,0,0,63,255,128,0,0,252,0,1,255,255,255,128,1,254,0,7,241,255,255,192,3,135,0,31,0,0,1,224,7,7,0,60,0,0,0,224,14,7,0,120,0,0,0,224,28,7,0,240,0,0,0,224,56,14,1,192,0,0,1,192,112,28,7,128,0,7,255,192,224,60,15,0,0,31,255,1,192,120,30,0,0,15,192,3,128,240,56,0,0,15,128,7,1,224,240,0,0,7,255,14,3,192,240,0,0,1,255,252,7,128,112,0,0,0,15,248,15,0,56,0,0,0,0,0,30,0,28,0,0,0,0,0,60,0,30,0,0,0,0,0,120,0,14,0,0,0,0,0,240,0,7,0,0,0,0,1,224,0,3,128,24,0,0,3,192,0,1,192,127,255,192,7,128,0,1,225,255,255,255,255,0,0,0,227,192,15,255,252,0,0,0,119,128,0,0,112,0,0,0,63,0,0,0,0,0,0,0,28,0,0,0,0,0,0,0,8,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,254,31,192,0,0,0,0,1,239,61,224,0,0,0,0,3,1,240,48,0,0,0,0,6,1,224,24,0,0,0,0,4,0,192,8,0,0,0,0,4,0,0,12,0,0,0,0,12,0,0,12,0,0,0,0,12,0,0,12,0,0,0,0,12,0,0,8,0,0,0,0,6,0,0,24,0,0,0,0,6,0,0,16,0,0,0,0,3,0,0,48,0,0,0,0,1,128,0,96,0,0,0,0,0,192,0,192,0,0,0,0,0,224,1,192,0,0,0,0,0,112,3,128,0,0,0,0,0,56,7,0,0,0,0,0,0,12,12,0,0,0,0,0,0,6,56,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,255,0,0,120,0,0,63,255,255,128,1,254,0,1,255,255,227,192,3,207,0,7,240,0,1,192,7,135,0,31,0,0,0,192,15,7,0,60,0,0,1,192,30,7,0,120,0,0,1,192,56,15,0,240,0,0,31,128,112,30,1,224,0,3,255,1,224,60,3,128,0,31,248,3,192,120,15,0,0,31,0,7,128,240,30,0,0,14,0,15,1,224,60,0,0,7,255,254,3,192,120,0,0,7,255,252,7,128,240,0,0,1,255,240,15,0,240,0,0,0,0,0,30,0,112,0,0,0,0,0,60,0,56,0,0,0,0,0,120,0,28,0,0,0,0,0,240,0,14,0,0,0,0,1,224,0,7,0,0,0,0,3,192,0,3,128,31,255,192,15,128,0,1,192,127,255,255,254,0,0,0,224,255,255,255,252,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,8,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,1,128,0,0,0,0,3,252,15,240,0,0,0,0,15,254,63,252,0,0,0,0,28,7,248,14,0,0,0,0,56,1,240,7,0,0,0,0,112,1,224,3,128,0,0,0,96,0,192,1,128,0,0,0,96,0,0,1,128,0,0,0,224,0,0,1,128,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,96,0,0,1,128,0,0,0,96,0,0,1,128,0,0,0,112,0,0,3,128,0,0,0,48,0,0,3,0,0,0,0,56,0,0,7,0,0,0,0,28,0,0,14,0,0,0,0,14,0,0,28,0,0,0,0,7,0,0,56,0,0,0,0,3,128,0,112,0,0,0,0,1,192,0,224,0,0,0,0,0,224,1,192,0,0,0,0,0,112,3,128,0,0,0,0,0,56,15,0,0,0,0,0,0,30,30,0,0,0,0,0,0,7,60,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,0,0,0,7,255,0,0,16,0,0,1,255,255,128,0,252,0,0,63,254,3,128,1,254,0,1,255,128,1,128,7,135,0,7,224,0,1,128,15,7,0,31,0,0,3,128,30,7,0,60,0,0,15,0,60,7,0,120,0,0,126,0,240,14,0,240,0,3,248,1,224,28,1,224,0,31,192,3,192,56,3,192,0,62,0,7,128,112,7,128,0,28,0,30,0,224,14,0,0,14,7,252,3,192,28,0,0,7,255,248,7,128,120,0,0,7,254,0,15,0,240,0,0,1,0,0,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,192,0,14,0,0,0,0,7,128,0,7,0,0,0,31,255,0,0,3,128,31,255,255,254,0,0,1,192,127,255,255,192,0,0,0,224,255,128,0,0,0,0,0,113,192,0,0,0,0,0,0,59,128,0,0,0,0,0,0,31,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,4,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,7,248,7,248,0,0,0,0,31,254,31,254,0,0,0,0,62,31,62,31,0,0,0,0,120,7,248,7,128,0,0,0,224,3,240,1,192,0,0,1,192,1,224,0,192,0,0,1,192,0,192,0,224,0,0,1,128,0,0,0,96,0,0,1,128,0,0,0,96,0,0,3,128,0,0,0,112,0,0,3,128,0,0,0,112,0,0,3,128,0,0,0,96,0,0,1,128,0,0,0,96,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,192,0,0,0,224,0,0,1,192,0,0,0,96,0,0,3,128,0,0,0,112,0,0,7,128,0,0,0,56,0,0,7,0,0,0,0,28,0,0,14,0,0,0,0,14,0,0,28,0,0,0,0,7,0,0,56,0,0,0,0,3,128,0,112,0,0,0,0,1,192,1,224,0,0,0,0,0,240,3,192,0,0,0,0,0,120,7,128,0,0,0,0,0,60,15,0,0,0,0,0,0,30,30,0,0,0,0,0,0,15,252,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,28,192,0,0,0,0,0,1,254,0,0,0,0,0,0,63,255,0,0,56,0,0,3,255,3,128,0,252,0,0,63,240,3,128,3,254,0,1,254,0,3,128,7,135,0,7,224,0,3,128,15,7,0,31,0,0,15,0,60,7,0,60,0,0,62,0,120,7,0,120,0,1,248,0,240,14,0,240,0,7,224,3,192,28,1,224,0,63,0,7,128,120,3,192,0,60,0,15,0,240,7,128,0,28,0,252,1,224,15,0,0,14,31,248,3,192,30,0,0,7,255,192,7,128,56,0,0,7,248,0,15,0,112,0,0,1,0,0,28,0,224,0,0,0,0,0,120,0,240,0,0,0,0,0,240,0,120,0,0,0,0,1,224,0,28,0,0,0,0,3,192,0,14,0,0,0,0,127,128,0,7,0,0,3,255,254,0,0,3,128,31,255,255,240,0,0,1,192,127,255,192,0,0,0,0,224,254,0,0,0,0,0,0,113,192,0,0,0,0,0,0,59,128,0,0,0,0,0,0,31,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,4,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,3,240,7,240,0,0,0,0,15,252,31,252,0,0,0,0,63,190,63,254,0,0,0,0,120,7,120,7,0,0,0,0,112,3,240,3,128,0,0,0,224,1,224,1,192,0,0,1,192,0,192,0,192,0,0,1,192,0,0,0,224,0,0,1,128,0,0,0,224,0,0,1,128,0,0,0,96,0,0,1,128,0,0,0,96,0,0,1,128,0,0,0,96,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,0,192,0,0,1,192,0,0,0,224,0,0,1,128,0,0,0,112,0,0,3,128,0,0,0,56,0,0,7,0,0,0,0,56,0,0,15,0,0,0,0,28,0,0,30,0,0,0,0,14,0,0,60,0,0,0,0,7,0,0,120,0,0,0,0,3,128,0,240,0,0,0,0,1,224,1,224,0,0,0,0,0,240,3,192,0,0,0,0,0,120,7,128,0,0,0,0,0,28,30,0,0,0,0,0,0,15,60,0,0,0,0,0,0,7,248,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,126,0,0,120,0,0,0,255,255,0,1,254,0,0,63,255,255,128,3,206,0,1,255,224,1,128,7,135,0,7,224,0,1,192,15,7,0,31,0,0,1,192,28,7,0,60,0,0,3,128,120,14,0,120,0,0,15,128,240,30,0,240,0,0,126,1,224,60,1,224,0,7,248,3,192,120,3,192,0,31,192,7,128,240,7,128,0,30,0,15,1,224,14,0,0,14,1,252,3,192,28,0,0,7,255,248,7,128,120,0,0,7,255,224,15,0,240,0,0,1,128,0,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,112,0,56,0,0,0,0,0,224,0,28,0,0,0,0,1,192,0,14,0,0,0,0,3,128,0,7,0,0,0,1,255,0,0,3,128,31,255,255,254,0,0,1,192,127,255,255,240,0,0,0,224,255,192,0,0,0,0,0,113,192,0,0,0,0,0,0,123,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,96,0,192,0,0,0,0,7,252,15,248,0,0,0,0,31,254,31,254,0,0,0,0,60,7,56,15,0,0,0,0,120,3,240,3,128,0,0,0,96,1,224,3,128,0,0,0,224,1,224,1,192,0,0,0,192,0,192,0,192,0,0,1,192,0,0,0,192,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,192,0,0,0,192,0,0,0,192,0,0,0,224,0,0,1,192,0,0,0,112,0,0,3,128,0,0,0,112,0,0,3,128,0,0,0,56,0,0,7,0,0,0,0,28,0,0,14,0,0,0,0,14,0,0,28,0,0,0,0,7,0,0,60,0,0,0,0,3,128,0,120,0,0,0,0,1,192,0,240,0,0,0,0,0,224,1,224,0,0,0,0,0,112,7,128,0,0,0,0,0,60,15,0,0,0,0,0,0,30,30,0,0,0,0,0,0,15,60,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,96,0,192,0,0,0,0,7,252,15,248,0,0,0,0,31,254,31,254,0,0,0,0,60,7,56,15,0,0,0,0,120,3,240,3,128,0,0,0,96,1,224,3,128,0,0,0,224,1,224,1,192,0,0,0,192,0,192,0,192,0,0,1,192,0,0,0,192,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,192,0,0,0,192,0,0,0,192,0,0,0,224,0,0,1,192,0,0,0,112,0,0,3,128,0,0,0,112,0,0,3,128,0,0,0,56,0,0,7,0,0,0,0,28,0,0,14,0,0,0,0,14,0,0,28,0,0,0,0,7,0,0,60,0,0,0,0,3,128,0,120,0,0,0,0,1,192,0,240,0,0,0,0,0,224,1,224,0,0,0,0,0,112,7,128,0,0,0,0,0,60,15,0,0,0,0,0,0,30,30,0,0,0,0,0,0,15,60,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,96,0,192,0,0,0,0,7,252,15,248,0,0,0,0,31,254,31,254,0,0,0,0,60,7,56,15,0,0,0,0,120,3,240,3,128,0,0,0,96,1,224,3,128,0,0,0,224,1,224,1,192,0,0,0,192,0,192,0,192,0,0,1,192,0,0,0,192,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,192,0,0,0,192,0,0,0,192,0,0,0,224,0,0,1,192,0,0,0,112,0,0,3,128,0,0,0,112,0,0,3,128,0,0,0,56,0,0,7,0,0,0,0,28,0,0,14,0,0,0,0,14,0,0,28,0,0,0,0,7,0,0,60,0,0,0,0,3,128,0,120,0,0,0,0,1,192,0,240,0,0,0,0,0,224,1,224,0,0,0,0,0,112,7,128,0,0,0,0,0,60,15,0,0,0,0,0,0,30,30,0,0,0,0,0,0,15,60,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,96,0,192,0,0,0,0,7,252,15,248,0,0,0,0,31,254,31,254,0,0,0,0,60,7,56,15,0,0,0,0,120,3,240,3,128,0,0,0,96,1,224,3,128,0,0,0,224,1,224,1,192,0,0,0,192,0,192,0,192,0,0,1,192,0,0,0,192,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,192,0,0,0,192,0,0,0,192,0,0,0,224,0,0,1,192,0,0,0,112,0,0,3,128,0,0,0,112,0,0,3,128,0,0,0,56,0,0,7,0,0,0,0,28,0,0,14,0,0,0,0,14,0,0,28,0,0,0,0,7,0,0,60,0,0,0,0,3,128,0,120,0,0,0,0,1,192,0,240,0,0,0,0,0,224,1,224,0,0,0,0,0,112,7,128,0,0,0,0,0,60,15,0,0,0,0,0,0,30,30,0,0,0,0,0,0,15,60,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,96,0,192,0,0,0,0,7,252,15,248,0,0,0,0,31,254,31,254,0,0,0,0,60,7,56,15,0,0,0,0,120,3,240,3,128,0,0,0,96,1,224,3,128,0,0,0,224,1,224,1,192,0,0,0,192,0,192,0,192,0,0,1,192,0,0,0,192,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,192,0,0,0,192,0,0,0,192,0,0,0,224,0,0,1,192,0,0,0,112,0,0,3,128,0,0,0,112,0,0,3,128,0,0,0,56,0,0,7,0,0,0,0,28,0,0,14,0,0,0,0,14,0,0,28,0,0,0,0,7,0,0,60,0,0,0,0,3,128,0,120,0,0,0,0,1,192,0,240,0,0,0,0,0,224,1,224,0,0,0,0,0,112,7,128,0,0,0,0,0,60,15,0,0,0,0,0,0,30,30,0,0,0,0,0,0,15,60,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,96,0,192,0,0,0,0,7,252,15,248,0,0,0,0,31,254,31,254,0,0,0,0,60,7,56,15,0,0,0,0,120,3,240,3,128,0,0,0,96,1,224,3,128,0,0,0,224,1,224,1,192,0,0,0,192,0,192,0,192,0,0,1,192,0,0,0,192,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,192,0,0,0,192,0,0,0,192,0,0,0,224,0,0,1,192,0,0,0,112,0,0,3,128,0,0,0,112,0,0,3,128,0,0,0,56,0,0,7,0,0,0,0,28,0,0,14,0,0,0,0,14,0,0,28,0,0,0,0,7,0,0,60,0,0,0,0,3,128,0,120,0,0,0,0,1,192,0,240,0,0,0,0,0,224,1,224,0,0,0,0,0,112,7,128,0,0,0,0,0,60,15,0,0,0,0,0,0,30,30,0,0,0,0,0,0,15,60,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,96,0,192,0,0,0,0,7,252,15,248,0,0,0,0,31,254,31,254,0,0,0,0,60,7,56,15,0,0,0,0,120,3,240,3,128,0,0,0,96,1,224,3,128,0,0,0,224,1,224,1,192,0,0,0,192,0,192,0,192,0,0,1,192,0,0,0,192,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,192,0,0,0,192,0,0,0,192,0,0,0,224,0,0,1,192,0,0,0,112,0,0,3,128,0,0,0,112,0,0,3,128,0,0,0,56,0,0,7,0,0,0,0,28,0,0,14,0,0,0,0,14,0,0,28,0,0,0,0,7,0,0,60,0,0,0,0,3,128,0,120,0,0,0,0,1,192,0,240,0,0,0,0,0,224,1,224,0,0,0,0,0,112,7,128,0,0,0,0,0,60,15,0,0,0,0,0,0,30,30,0,0,0,0,0,0,15,60,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,3,240,7,240,0,0,0,0,15,252,31,252,0,0,0,0,63,190,63,254,0,0,0,0,120,7,120,7,0,0,0,0,112,3,240,3,128,0,0,0,224,1,224,1,192,0,0,1,192,0,192,0,192,0,0,1,192,0,0,0,224,0,0,1,128,0,0,0,224,0,0,1,128,0,0,0,96,0,0,1,128,0,0,0,96,0,0,1,128,0,0,0,96,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,224,0,0,0,192,0,0,1,192,0,0,0,224,0,0,1,128,0,0,0,112,0,0,3,128,0,0,0,56,0,0,7,0,0,0,0,56,0,0,15,0,0,0,0,28,0,0,30,0,0,0,0,14,0,0,60,0,0,0,0,7,0,0,120,0,0,0,0,3,128,0,240,0,0,0,0,1,224,1,224,0,0,0,0,0,240,3,192,0,0,0,0,0,120,7,128,0,0,0,0,0,28,30,0,0,0,0,0,0,15,60,0,0,0,0,0,0,7,248,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,126,0,0,120,0,0,0,255,255,0,1,254,0,0,63,255,255,128,3,206,0,1,255,224,1,128,7,135,0,7,224,0,1,192,15,7,0,31,0,0,1,192,28,7,0,60,0,0,3,128,120,14,0,120,0,0,15,128,240,30,0,240,0,0,126,1,224,60,1,224,0,7,248,3,192,120,3,192,0,31,192,7,128,240,7,128,0,30,0,15,1,224,14,0,0,14,1,252,3,192,28,0,0,7,255,248,7,128,120,0,0,7,255,224,15,0,240,0,0,1,128,0,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,112,0,56,0,0,0,0,0,224,0,28,0,0,0,0,1,192,0,14,0,0,0,0,3,128,0,7,0,0,0,1,255,0,0,3,128,31,255,255,254,0,0,1,192,127,255,255,240,0,0,0,224,255,192,0,0,0,0,0,113,192,0,0,0,0,0,0,123,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,7,248,7,248,0,0,0,0,31,254,31,254,0,0,0,0,62,31,62,31,0,0,0,0,120,7,248,7,128,0,0,0,224,3,240,1,192,0,0,1,192,1,224,0,192,0,0,1,192,0,192,0,224,0,0,1,128,0,0,0,96,0,0,1,128,0,0,0,96,0,0,3,128,0,0,0,112,0,0,3,128,0,0,0,112,0,0,3,128,0,0,0,96,0,0,1,128,0,0,0,96,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,192,0,0,0,224,0,0,1,192,0,0,0,96,0,0,3,128,0,0,0,112,0,0,7,128,0,0,0,56,0,0,7,0,0,0,0,28,0,0,14,0,0,0,0,14,0,0,28,0,0,0,0,7,0,0,56,0,0,0,0,3,128,0,112,0,0,0,0,1,192,1,224,0,0,0,0,0,240,3,192,0,0,0,0,0,120,7,128,0,0,0,0,0,60,15,0,0,0,0,0,0,30,30,0,0,0,0,0,0,15,252,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,28,192,0,0,0,0,0,1,254,0,0,0,0,0,0,63,255,0,0,56,0,0,3,255,3,128,0,252,0,0,63,240,3,128,3,254,0,1,254,0,3,128,7,135,0,7,224,0,3,128,15,7,0,31,0,0,15,0,60,7,0,60,0,0,62,0,120,7,0,120,0,1,248,0,240,14,0,240,0,7,224,3,192,28,1,224,0,63,0,7,128,120,3,192,0,60,0,15,0,240,7,128,0,28,0,252,1,224,15,0,0,14,31,248,3,192,30,0,0,7,255,192,7,128,56,0,0,7,248,0,15,0,112,0,0,1,0,0,28,0,224,0,0,0,0,0,120,0,240,0,0,0,0,0,240,0,120,0,0,0,0,1,224,0,28,0,0,0,0,3,192,0,14,0,0,0,0,127,128,0,7,0,0,3,255,254,0,0,3,128,31,255,255,240,0,0,1,192,127,255,192,0,0,0,0,224,254,0,0,0,0,0,0,113,192,0,0,0,0,0,0,59,128,0,0,0,0,0,0,31,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,4,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,1,128,0,0,0,0,3,252,15,240,0,0,0,0,15,254,63,252,0,0,0,0,28,7,248,14,0,0,0,0,56,1,240,7,0,0,0,0,112,1,224,3,128,0,0,0,96,0,192,1,128,0,0,0,96,0,0,1,128,0,0,0,224,0,0,1,128,0,0,0,224,0,0,1,192,0,0,0,224,0,0,1,192,0,0,0,96,0,0,1,128,0,0,0,96,0,0,1,128,0,0,0,112,0,0,3,128,0,0,0,48,0,0,3,0,0,0,0,56,0,0,7,0,0,0,0,28,0,0,14,0,0,0,0,14,0,0,28,0,0,0,0,7,0,0,56,0,0,0,0,3,128,0,112,0,0,0,0,1,192,0,224,0,0,0,0,0,224,1,192,0,0,0,0,0,112,3,128,0,0,0,0,0,56,15,0,0,0,0,0,0,30,30,0,0,0,0,0,0,7,60,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,0,0,0,7,255,0,0,16,0,0,1,255,255,128,0,252,0,0,63,254,3,128,1,254,0,1,255,128,1,128,7,135,0,7,224,0,1,128,15,7,0,31,0,0,3,128,30,7,0,60,0,0,15,0,60,7,0,120,0,0,126,0,240,14,0,240,0,3,248,1,224,28,1,224,0,31,192,3,192,56,3,192,0,62,0,7,128,112,7,128,0,28,0,30,0,224,14,0,0,14,7,252,3,192,28,0,0,7,255,248,7,128,120,0,0,7,254,0,15,0,240,0,0,1,0,0,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,192,0,14,0,0,0,0,7,128,0,7,0,0,0,31,255,0,0,3,128,31,255,255,254,0,0,1,192,127,255,255,192,0,0,0,224,255,128,0,0,0,0,0,113,192,0,0,0,0,0,0,59,128,0,0,0,0,0,0,31,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,4,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,254,31,192,0,0,0,0,1,239,61,224,0,0,0,0,3,1,240,48,0,0,0,0,6,1,224,24,0,0,0,0,4,0,192,8,0,0,0,0,4,0,0,12,0,0,0,0,12,0,0,12,0,0,0,0,12,0,0,12,0,0,0,0,12,0,0,8,0,0,0,0,6,0,0,24,0,0,0,0,6,0,0,16,0,0,0,0,3,0,0,48,0,0,0,0,1,128,0,96,0,0,0,0,0,192,0,192,0,0,0,0,0,224,1,192,0,0,0,0,0,112,3,128,0,0,0,0,0,56,7,0,0,0,0,0,0,12,12,0,0,0,0,0,0,6,56,0,0,0,0,0,0,3,240,0,0,0,0,0,0,1,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,255,0,0,120,0,0,63,255,255,128,1,254,0,1,255,255,227,192,3,207,0,7,240,0,1,192,7,135,0,31,0,0,0,192,15,7,0,60,0,0,1,192,30,7,0,120,0,0,1,192,56,15,0,240,0,0,31,128,112,30,1,224,0,3,255,1,224,60,3,128,0,31,248,3,192,120,15,0,0,31,0,7,128,240,30,0,0,14,0,15,1,224,60,0,0,7,255,254,3,192,120,0,0,7,255,252,7,128,240,0,0,1,255,240,15,0,240,0,0,0,0,0,30,0,112,0,0,0,0,0,60,0,56,0,0,0,0,0,120,0,28,0,0,0,0,0,240,0,14,0,0,0,0,1,224,0,7,0,0,0,0,3,192,0,3,128,31,255,192,15,128,0,1,192,127,255,255,254,0,0,0,224,255,255,255,252,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,8,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,56,0,0,63,255,128,0,0,252,0,1,255,255,255,128,1,254,0,7,241,255,255,192,3,135,0,31,0,0,1,224,7,7,0,60,0,0,0,224,14,7,0,120,0,0,0,224,28,7,0,240,0,0,0,224,56,14,1,192,0,0,1,192,112,28,7,128,0,7,255,192,224,60,15,0,0,31,255,1,192,120,30,0,0,15,192,3,128,240,56,0,0,15,128,7,1,224,240,0,0,7,255,14,3,192,240,0,0,1,255,252,7,128,112,0,0,0,15,248,15,0,56,0,0,0,0,0,30,0,28,0,0,0,0,0,60,0,30,0,0,0,0,0,120,0,14,0,0,0,0,0,240,0,7,0,0,0,0,1,224,0,3,128,24,0,0,3,192,0,1,192,127,255,192,7,128,0,1,225,255,255,255,255,0,0,0,227,192,15,255,252,0,0,0,119,128,0,0,112,0,0,0,63,0,0,0,0,0,0,0,28,0,0,0,0,0,0,0,8,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,127,224,0,0,0,252,0,3,255,255,252,0,1,254,0,7,247,255,255,128,3,199,0,31,0,0,255,192,7,135,0,60,0,0,0,224,15,7,0,120,0,0,0,96,30,7,0,240,0,0,0,96,60,14,3,192,0,0,0,224,120,30,7,128,0,0,1,224,240,60,15,0,0,31,255,193,224,120,28,0,0,31,255,3,192,240,120,0,0,15,128,7,129,224,240,0,0,7,252,15,3,192,112,0,0,1,255,222,7,128,112,0,0,0,31,252,7,0,56,0,0,0,0,248,14,0,28,0,0,0,0,0,28,0,14,0,0,0,0,0,56,0,14,0,0,0,0,0,112,0,7,0,0,0,0,0,224,0,3,128,24,0,0,1,192,0,1,192,127,252,0,3,128,0,1,193,255,255,254,7,0,0,0,227,192,127,255,254,0,0,0,119,128,0,63,252,0,0,0,62,0,0,0,0,0,0,0,28,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,127,255,128,0,1,254,0,1,255,255,255,0,3,199,0,7,225,255,255,192,7,135,0,31,0,0,1,192,15,7,0,60,0,0,0,224,30,7,0,120,0,0,0,224,60,14,0,240,0,0,0,224,120,30,1,192,0,0,1,192,240,60,7,128,0,1,255,193,224,120,15,0,0,31,255,3,192,240,30,0,0,31,224,7,129,224,60,0,0,15,192,15,3,192,112,0,0,7,255,254,7,128,224,0,0,1,255,252,15,0,112,0,0,0,1,248,30,0,56,0,0,0,0,0,60,0,60,0,0,0,0,0,120,0,28,0,0,0,0,0,240,0,14,0,0,0,0,1,224,0,7,0,0,0,0,1,192,0,3,128,28,0,0,3,128,0,1,192,127,255,255,15,0,0,1,225,255,255,255,254,0,0,0,227,192,0,255,248,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,28,0,0,0,0,0,0,0,8,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,252,0,0,0,0,0,0,1,254,0,0,127,255,255,0,3,135,0,1,255,255,255,128,7,7,0,7,224,0,3,192,14,7,0,31,0,0,1,192,28,7,0,60,0,0,0,192,56,14,0,120,0,0,1,192,112,28,0,240,0,0,1,192,224,60,1,224,0,0,31,129,192,120,3,128,0,3,255,3,128,240,7,0,0,31,248,7,1,224,30,0,0,31,0,14,3,192,60,0,0,15,255,252,7,128,120,0,0,7,255,248,15,0,240,0,0,1,255,240,30,0,224,0,0,0,0,0,60,0,112,0,0,0,0,0,120,0,56,0,0,0,0,0,240,0,28,0,0,0,0,1,224,0,14,0,0,0,0,3,192,0,7,0,0,0,0,7,128,0,3,128,31,255,255,255,0,0,1,192,127,255,255,254,0,0,1,224,255,255,255,240,0,0,0,243,192,0,0,0,0,0,0,119,128,0,0,0,0,0,0,63,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,12,0,0,0,0,0,0}
};
typedef enum {
	COLOR_BLACK,
	COLOR_WHITE
} color_t;
typedef enum {
	COMMAND,
	DATA
} write_mode_t;
#endif /* __SSD1306_H__ */