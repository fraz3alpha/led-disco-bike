// This is a demonstration on how to use an input device to trigger changes on your neo pixels.
// You should wire a momentary push button to connect from ground to a digital IO pin.  When you
// press the button it will change to a new pixel animation.  Note that you need to press the
// button once to start the first animation!

#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   2    // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.

#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 144


/*
Rough layout of the LEDS

LEFT

======   =========
       /|      __/
      / |   _/      |
     /  | _/        |
     --  /          |
     

RIGHT     
======   =========
       /|      __/
      / |   _/      |
     /  | _/        |
     --  /          |


*/ 
#define L_CROSSBAR_START 0
#define L_CROSSBAR_LENGTH 14

#define L_FORK_START L_CROSSBAR_START + L_CROSSBAR_LENGTH
#define L_FORK_LENGTH 7

#define L_DOWN_TUBE_START R_FORK_START + R_FORK_LENGTH
#define L_DOWN_TUBE_LENGTH 17

#define L_CHAIN_STAY_START L_DOWN_TUBE_START + L_DOWN_TUBE_LENGTH
#define L_CHAIN_STAY_LENGTH 4

#define L_SEAT_STAY_START L_CHAIN_STAY_START + L_CHAIN_STAY_LENGTH
#define L_SEAT_STAY_LENGTH 9

#define L_SEAT_TUBE_START L_SEAT_STAY_START + L_SEAT_STAY_LENGTH
#define L_SEAT_TUBE_LENGTH 8

#define L_MUDGUARD_START R_MUDGUARD_START + R_MUDGUARD_LENGTH
#define L_MUDGUARD_LENGTH 13


#define R_CROSSBAR_START R_SEAT_TUBE_START + R_SEAT_TUBE_LENGTH
#define R_CROSSBAR_LENGTH 14

#define R_FORK_START L_FORK_START + L_FORK_LENGTH
#define R_FORK_LENGTH 7

#define R_DOWN_TUBE_START R_CROSSBAR_START + R_CROSSBAR_LENGTH
#define R_DOWN_TUBE_LENGTH 17

#define R_CHAIN_STAY_START R_DOWN_TUBE_START + R_DOWN_TUBE_LENGTH
#define R_CHAIN_STAY_LENGTH 4

#define R_SEAT_STAY_START R_CHAIN_STAY_START + R_CHAIN_STAY_LENGTH
#define R_SEAT_STAY_LENGTH 9

#define R_SEAT_TUBE_START L_SEAT_TUBE_START + L_SEAT_TUBE_LENGTH
#define R_SEAT_TUBE_LENGTH 8

#define R_MUDGUARD_START R_SEAT_STAY_START + R_SEAT_STAY_LENGTH
#define R_MUDGUARD_LENGTH 13

#define WHITE 0xFFFFFF
#define RED 0xFF0000
#define BLACK 0x0

uint32_t colours[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF};

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = -1;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  roadMode();
  strip.show(); // Initialize all pixels to 'off'
//  standardRainbow(20);

}

uint8_t counter = 0;
void loop() {

  // Get current button state.
  bool newState = digitalRead(BUTTON_PIN);
  Serial.println(newState);
  
  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      counter=0;
      showType++;
      if (showType > 5)
        showType=0;
      //startShow(showType);
    }
  }

  // Set the last button state to the old state.
  oldState = newState;
  
  doShow(counter, showType);
  delay(10);
  counter++;

}

void doShow(uint8_t counter, int i) {
  switch(i){
    case -1:
            firstRun();
            break;
    case 0: 
            off();
            break;
    case 1: 
            roadMode();
            break;
    case 2: 
            standardRainbow2(counter);
            break;
    case 3: 
            frontToBackRainbow(counter);
            break;
    case 4: disco(counter);
            break;
    case 5: rainbowCycle(counter);
            break;
  }
}

/*
 * Set a couple of pixels on the front, so that I know it's on
 *  and working
 */
void firstRun(void) {
  int i=0;
  for (i=0; i<PIXEL_COUNT; i++) {
    strip.setPixelColor(i, BLACK);
  }

  strip.setPixelColor(L_FORK_START+L_FORK_LENGTH-1, WHITE);
  strip.setPixelColor(R_FORK_START+R_FORK_LENGTH-1, WHITE);
  strip.setPixelColor(L_MUDGUARD_START, RED);
  strip.setPixelColor(R_MUDGUARD_START+R_MUDGUARD_LENGTH-1, RED);

  strip.show();
}

void off(void) {
  int i=0;
  for (i=0; i<PIXEL_COUNT; i++) {
    strip.setPixelColor(i, BLACK);
  }
  strip.show();
}

void disco(uint8_t counter) {
  int i=0;
  if (counter % 32 == 0) {
    for (i=0; i<PIXEL_COUNT; i++) {
      long rand = random(6);
      strip.setPixelColor(i, colours[rand]);
    }
    strip.show();
  }
}

// Put the front lights as white, the rear (mud guard) as red
void roadMode(void) {
  int i=0;
  for (i=L_FORK_START; i<L_FORK_START+L_FORK_LENGTH; i++) {
    strip.setPixelColor(i, WHITE);
  }
  for (i=R_FORK_START; i<R_FORK_START+R_FORK_LENGTH; i++) {
    strip.setPixelColor(i, WHITE);
  }
  for (i=L_MUDGUARD_START; i<L_MUDGUARD_START+L_MUDGUARD_LENGTH; i++) {
    strip.setPixelColor(i, RED);
  }

  for (i=R_MUDGUARD_START; i<R_MUDGUARD_START+R_MUDGUARD_LENGTH; i++) {
    strip.setPixelColor(i, RED);
  }
  strip.show();
  
}

// Standard Rainbow puts the same colours on each side, going roughly from left to right
/*
- - - - - - - - - - - - - | 0 - - - - - - - 0 - - - - - - - 0 |
        - - - - - - - - - |                                   |
        - - - -           | - - - - - - - - - - - - - - - - - |
                          |                                   |
                          .                                   .
*/
void standardRainbow(uint8_t counter) {
// The rainbow is MUDGUARD + 1 + DOWN_TUBE + 1 long = 32
  uint16_t i, j, k;
  
  j = counter;
    // Mudguard
    for(i=0; i<13; i++) {
      strip.setPixelColor(L_MUDGUARD_START + L_MUDGUARD_LENGTH - i, Wheel((i+j) & 255));
      strip.setPixelColor(R_MUDGUARD_START + i, Wheel((i+j) & 255));
    }
    // Chain Stay
    for(i=0; i<4; i++) {
      k=4;
      strip.setPixelColor(L_CHAIN_STAY_START + L_CHAIN_STAY_LENGTH - i, Wheel((k+i+j) & 255));
      strip.setPixelColor(R_CHAIN_STAY_START + L_CHAIN_STAY_LENGTH - i, Wheel((k+i+j) & 255));
    }
    // Seat Stay
    for(i=0; i<9; i++) {
      k=4;
      strip.setPixelColor(L_SEAT_STAY_START + L_SEAT_STAY_LENGTH - i, Wheel((k+i+j) & 255));
      strip.setPixelColor(R_SEAT_STAY_START + L_SEAT_STAY_LENGTH - i, Wheel((k+i+j) & 255));
    }
    // Seat post
    for (k=0; k<8; k++) {
      i=13;
      strip.setPixelColor(L_SEAT_TUBE_START + k, Wheel((i+j) & 255));
      strip.setPixelColor(R_SEAT_TUBE_START + k, Wheel((i+j) & 255));
    }
    // Down Tube
    for(i=0; i<17; i++) {
      k = 14;
      strip.setPixelColor(L_DOWN_TUBE_START + i, Wheel((k+i+j) & 255));
      strip.setPixelColor(R_DOWN_TUBE_START + i, Wheel((k+i+j) & 255));
    }
    // Crossbar
    for(i=0; i<17; i++) {
      k = 14;
      // Skip 0, 8 and 16
      if (i %8 != 0) {
        strip.setPixelColor(L_CROSSBAR_START + i, Wheel((k+i+j) & 255));
        strip.setPixelColor(R_CROSSBAR_START + i, Wheel((k+i+j) & 255));
      }
    }
    // Forks
    for (k=0; k<7; k++) {
      i=31;
      strip.setPixelColor(L_FORK_START + k, Wheel((i+j) & 255));
      strip.setPixelColor(R_FORK_START + k, Wheel((i+j) & 255));
    }
    
    strip.show();
  
}

void standardRainbow2(uint8_t counter) {
// The rainbow is MUDGUARD + 1 + DOWN_TUBE + 1 long = 32
  uint16_t i, j, k;
  
  j = counter;
    // Mudguard
    for(i=0; i<13; i++) {
      k=0;
      strip.setPixelColor(R_MUDGUARD_START + R_MUDGUARD_LENGTH -1 - i, Wheel(i+k+j % 255));
      strip.setPixelColor(L_MUDGUARD_START + i, Wheel(i+k+j % 255));
    }
    // Chain Stay
    for(i=0; i<4; i++) {
      k=5;
      strip.setPixelColor(L_CHAIN_STAY_START + L_CHAIN_STAY_LENGTH -1 - i, Wheel(i+k+j % 255));
      strip.setPixelColor(R_CHAIN_STAY_START + L_CHAIN_STAY_LENGTH -1 - i, Wheel(i+k+j % 255));
    }
    // Seat Stay
    for(i=0; i<9; i++) {
      k=4;
      strip.setPixelColor(L_SEAT_STAY_START + i, Wheel(i+k+j % 255));
      strip.setPixelColor(R_SEAT_STAY_START + i, Wheel(i+k+j % 255));
    }
    // Seat post
    for (k=0; k<8; k++) {
      i=13;
      strip.setPixelColor(L_SEAT_TUBE_START + k, Wheel(i+j % 255));
      strip.setPixelColor(R_SEAT_TUBE_START + k, Wheel(i+j % 255));
    }
    // Down Tube
    for(i=0; i<17; i++) {
      k = 15;
      strip.setPixelColor(L_DOWN_TUBE_START + L_DOWN_TUBE_LENGTH - 1- i, Wheel(i+k+j % 255));
      strip.setPixelColor(R_DOWN_TUBE_START + R_DOWN_TUBE_LENGTH - 1- i, Wheel(i+k+j % 255));
    }
    // Crossbar
    for(i=0; i<14; i++) {
      k = 14;
        strip.setPixelColor(L_CROSSBAR_START + i, Wheel(i+k+j % 255));
        strip.setPixelColor(R_CROSSBAR_START + i, Wheel(i+k+j % 255));
    }
    // Forks
    for (i=0; i<7; i++) {
      k=31;
      strip.setPixelColor(L_FORK_START + i, Wheel(i+k+j % 255));
      strip.setPixelColor(R_FORK_START + i, Wheel(i+k+j % 255));
    }
    
    strip.show();
  
}

void frontToBackRainbow(uint8_t counter) {
// The rainbow is MUDGUARD + 1 + DOWN_TUBE + 1 long = 32
  uint16_t i, j, k;
  
  j = 255-counter;
    // Mudguard
    for(i=0; i<13; i++) {
      k=0;
      strip.setPixelColor(R_MUDGUARD_START + R_MUDGUARD_LENGTH -1 - i, Strobe(i+k, j));
      strip.setPixelColor(L_MUDGUARD_START + i, Strobe(i+k, j));
    }
    // Chain Stay
    for(i=0; i<4; i++) {
      k=5;
      strip.setPixelColor(L_CHAIN_STAY_START + L_CHAIN_STAY_LENGTH -1 - i, Strobe(i+k, j));
      strip.setPixelColor(R_CHAIN_STAY_START + L_CHAIN_STAY_LENGTH -1 - i, Strobe(i+k, j));
    }
    // Seat Stay
    for(i=0; i<9; i++) {
      k=4;
      strip.setPixelColor(L_SEAT_STAY_START + i, Strobe(i+k, j));
      strip.setPixelColor(R_SEAT_STAY_START + i, Strobe(i+k, j));
    }
    // Seat post
    for (k=0; k<8; k++) {
      i=13;
      strip.setPixelColor(L_SEAT_TUBE_START + k, Strobe(i, j));
      strip.setPixelColor(R_SEAT_TUBE_START + k, Strobe(i, j));
    }
    // Down Tube
    for(i=0; i<17; i++) {
      k = 15;
      strip.setPixelColor(L_DOWN_TUBE_START + L_DOWN_TUBE_LENGTH - 1- i, Strobe(i+k, j));
      strip.setPixelColor(R_DOWN_TUBE_START + R_DOWN_TUBE_LENGTH - 1- i, Strobe(i+k, j));
    }
    // Crossbar
    for(i=0; i<14; i++) {
      k = 14;
        strip.setPixelColor(L_CROSSBAR_START + i, Strobe(i+k, j));
        strip.setPixelColor(R_CROSSBAR_START + i, Strobe(i+k, j));
    }
    // Forks
    for (i=0; i<7; i++) {
      k=31;
      strip.setPixelColor(L_FORK_START + i, Strobe(i+k, j));
      strip.setPixelColor(R_FORK_START + i, Strobe(i+k, j));
    }
    
    strip.show();
  
}

uint32_t Strobe(uint8_t line, uint8_t counter) {
  if (line == (counter % 37)) {
    return Wheel(counter);
  }
  return BLACK;
}



// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t counter) {
  uint16_t i, j;

  j = counter;
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t counter) {
  uint16_t i, j;

  j = counter;
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
}

//Theatre-style crawling lights.
void theaterChase(uint8_t counter, uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
         
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
