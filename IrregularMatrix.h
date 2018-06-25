// IrregularMatrix.h

// Example irregular map generated by https://intrinsically-sublime.github.io/FastLED-XY-Map-Generator/
// The main sketch has a built in wrap function and it's own XY function so we only need the XYTable and definitions

// XY mapping function preserving all pixel data.
// Requires 771 Bytes's of SRAM and 7.71 ms/frame for WS2811 based LEDs.
// You COULD save 384 Bytes's of SRAM and 3.84 ms/frame for WS2811 based LEDs.
// Maximum frame rate for WS2811 based LEDs = 517 FPS using 4 parallel outputs.
// Connect LEDs every 32 LEDs for 4 way parallel output.
// Wired in vertical striped layout starting at the top left corner.

// Parameters for width and height
#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 16

#define NUM_LEDS 256	// 128 LEDs visible out of 256

CRGB leds[257];	// 1 extra pixel for hiding out of bounds data

const uint16_t XYTable[] = {
	 128, 143, 157, 170, 182, 193, 203, 212, 220, 228, 235, 241, 246, 250, 253, 255,
	 129, 144, 158, 171, 183, 194, 204, 213, 221, 229, 236, 242, 247, 251, 254, 113,
	 130, 145, 159, 172, 184, 195, 205, 214, 222, 230, 237, 243, 248, 252,  99, 114,
	 131, 146, 160, 173, 185, 196, 206, 215, 223, 231, 238, 244, 249,  86, 100, 115,
	 132, 147, 161, 174, 186, 197, 207, 216, 224, 232, 239, 245,  74,  87, 101, 116,
	 133, 148, 162, 175, 187, 198, 208, 217, 225, 233, 240,  63,  75,  88, 102, 117,
	 134, 149, 163, 176, 188, 199, 209, 218, 226, 234,  53,  64,  76,  89, 103, 118,
	 135, 150, 164, 177, 189, 200, 210, 219, 227,  44,  54,  65,  77,  90, 104, 119,
	 136, 151, 165, 178, 190, 201, 211,  28,  36,  45,  55,  66,  78,  91, 105, 120,
	 137, 152, 166, 179, 191, 202,  21,  29,  37,  46,  56,  67,  79,  92, 106, 121,
	 138, 153, 167, 180, 192,  15,  22,  30,  38,  47,  57,  68,  80,  93, 107, 122,
	 139, 154, 168, 181,  10,  16,  23,  31,  39,  48,  58,  69,  81,  94, 108, 123,
	 140, 155, 169,   6,  11,  17,  24,  32,  40,  49,  59,  70,  82,  95, 109, 124,
	 141, 156,   3,   7,  12,  18,  25,  33,  41,  50,  60,  71,  83,  96, 110, 125,
	 142,   1,   4,   8,  13,  19,  26,  34,  42,  51,  61,  72,  84,  97, 111, 126,
	   0,   2,   5,   9,  14,  20,  27,  35,  43,  52,  62,  73,  85,  98, 112, 127
};