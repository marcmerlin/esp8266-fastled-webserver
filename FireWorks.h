// FireworksXY
// Quick and dirty 2-D fireworks simulation using FastLED.
//
// Originaly designed an Adafruit 5x8 WS2811 shield, but works fine
// on other XY matricies.
//
// by Mark Kriegsman, July 2013
//
// Updated by Sublime for Novel Mutations Costume Controllers 2018

#define MODEL_BORDER 1
#define MODEL_WIDTH  (MODEL_BORDER + MATRIX_WIDTH  + MODEL_BORDER)
#define MODEL_HEIGHT (MODEL_BORDER + MATRIX_HEIGHT + MODEL_BORDER)

CRGB overrun;

CRGB& getXY( byte x, byte y) 
{
	x -= MODEL_BORDER;
	y -= MODEL_BORDER;
	if( x < MATRIX_WIDTH && y < MATRIX_HEIGHT) {
		return leds[ XY(x,y) ] ;
	} else
		return overrun;
}

void screenscale( accum88 a, byte N, byte& screen, byte& screenerr)
{
	byte ia = a >> 8;
	screen = scale8( ia, N);
	byte m = screen * (256 / N);
	screenerr = (ia - m) * scale8(255,N);
	return;
}

static int16_t scale15by8_local( int16_t i, fract8 scale )
{
	int16_t result;
	result = (int32_t)((int32_t)i * scale) / 256;
	return result;
}

saccum78 gGravity = 10;
fract8  gBounce = 200;
fract8  gDrag = 255;
bool gSkyburst = 0;

accum88 gBurstx;
accum88 gBursty;
saccum78 gBurstxv;
saccum78 gBurstyv;
CRGB gBurstcolor;

#define NONE 0
#define SHELL 1
#define SPARK 5

class Dot {
	public:
	byte     show;
	byte     theType;
	accum88  x;
	accum88  y;
	saccum78 xv;
	saccum78 yv;
	accum88  r;
	CRGB     color;

	Dot() {
		show = 0;
		theType = 0;
		x =  0;
		y =  0;
		xv = 0;
		yv = 0;
		r  = 0;
		color.setRGB( 0, 0, 0);
	}

	void Draw() {
		if( !show) return;

		byte ix, xe, xc;
		byte iy, ye, yc;
		screenscale( x, MODEL_WIDTH, ix, xe);
		screenscale( y, MODEL_HEIGHT, iy, ye);
		yc = 255 - ye;
		xc = 255 - xe;
    
		CRGB c00 = CRGB( dim8_video( scale8( scale8( color.r, yc), xc)), 
			dim8_video( scale8( scale8( color.g, yc), xc)), 
			dim8_video( scale8( scale8( color.b, yc), xc))
		);
		CRGB c01 = CRGB( dim8_video( scale8( scale8( color.r, ye), xc)), 
			dim8_video( scale8( scale8( color.g, ye), xc)), 
			dim8_video( scale8( scale8( color.b, ye), xc))
		);
		CRGB c10 = CRGB( dim8_video( scale8( scale8( color.r, yc), xe)), 
			dim8_video( scale8( scale8( color.g, yc), xe)), 
			dim8_video( scale8( scale8( color.b, yc), xe))
		);
		CRGB c11 = CRGB( dim8_video( scale8( scale8( color.r, ye), xe)), 
			dim8_video( scale8( scale8( color.g, ye), xe)), 
			dim8_video( scale8( scale8( color.b, ye), xe))
		);

		getXY(ix, iy) += c00;
		getXY(ix, iy + 1) += c01;
		getXY(ix + 1, iy) += c10;
		getXY(ix + 1, iy + 1) += c11;
	}
  
	void Move() {
		saccum78 oyv = yv;

		if( !show) return;
		yv -= gGravity;
		xv = scale15by8_local( xv, gDrag);    
		yv = scale15by8_local( yv, gDrag);

		if( theType == SPARK) {
			xv = scale15by8_local( xv, gDrag);    
			yv = scale15by8_local( yv, gDrag);
			color.nscale8( 255);
			if( !color) {
				show = 0;
			}
		}

		// if we'd hit the ground, bounce
		if( yv < 0 && (y < (-yv)) ) {
			if( theType == SPARK ) {
				show = 0;
			} else {
				yv = -yv;
				yv = scale15by8_local( yv, gBounce);
				if( yv < 500 ) {
					show = 0;
				}
			}
		}

		if( (yv < -300) /* && (!(oyv < 0))*/ ) {
			// pinnacle
			if( theType == SHELL ) {
				if( (y > (uint16_t)(0x8000)) /*&& (random8() < 64)*/) {
					// boom
					LEDS.showColor( CRGB::White);
		  			//delay( 1);
		  			LEDS.showColor( CRGB::Black);
				}

				show = 0;

				gSkyburst = 1;
				gBurstx = x;
				gBursty = y;
				gBurstxv = xv;
				gBurstyv = yv;
				gBurstcolor = color;        
			}
		}
		if( theType == SPARK) {
			if( ((xv >  0) && (x > xv)) || ((xv < 0 ) && (x < (0xFFFF + xv))) ) {
				x += xv;
			} else {
				show = 0;
			}
		} else {
			x += xv;
		}
		y += yv;
	}
  
	void GroundLaunch() {
		yv = (MATRIX_HEIGHT * 40) + random16(MATRIX_HEIGHT * 10);	// Vertical velocity = Minimum velocity + Random maximum difference
		xv = (int16_t)random16(600) - (int16_t)300;			// Horizontal velocity
		y = 0;
		x = 0x8000;
		hsv2rgb_rainbow( CHSV( random8(), 255, 255), color);		// Shell color at launch
		show = 1;
	}
  
	void Skyburst( accum88 basex, accum88 basey, saccum78 basedv, CRGB& basecolor) {
		yv = (int16_t)0 + (int16_t)random16(1500) - (int16_t)500;	// Vertical velocity
		xv = basedv + (int16_t)random16(2000) - (int16_t)1000;		// Horizontal velocity
		y = basey;
		x = basex;
		color = basecolor;
		color *= 4;
		theType = SPARK;
		show = 1;
	}

};

#define NUM_SPARKS ((NUM_LEDS/30)+5)

Dot gDot;
Dot gSparks[NUM_SPARKS];

void fireworks() 
{ 
	random16_add_entropy( 65481 );
	memset8( leds, 0, NUM_LEDS * 3);

	gDot.Move();
	gDot.Draw();

	for( byte b = 0; b < NUM_SPARKS; b++) {
		gSparks[b].Move();
		gSparks[b].Draw();
	}

	static uint16_t launchcountdown = 0;
		if( gDot.show == 0 ) {
			if( launchcountdown == 0) {
				gDot.GroundLaunch();
				gDot.theType = SHELL;
				launchcountdown = random16(map(speed,1,255,200,10)) + 1;
			} else {
				launchcountdown --;
		}
	}
  
	if( gSkyburst) {
		byte nsparks = random8( NUM_SPARKS / 2, NUM_SPARKS + 1);
		for( byte b = 0; b < nsparks; b++) {
			gSparks[b].Skyburst( gBurstx, gBursty, gBurstyv, gBurstcolor);
		gSkyburst = 0;
		}
	}

  delay(1);
}

