
#include <Adafruit_NeoPixel.h>


/*************************************************************
 * gemmaTouch - aka the team 102 spirit pin project
 *    uses the Adafruit gemma M0 board and a neopixel light ring
 *    to create a wearable spirit pin for team 102 members to wear
 *    at competitions.
 *    
 *    many of the light patterns found in this project are taken from
 *    adafruit neopixel example code.
 */
#define PIXEL_PIN    3
#define PIXEL_COUNT  90
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB );

int digitFirst = 0;
int digitSecond = 0;
int digitThird = 0; 
int digitFourth = 0;





//create the neopixel ring 

/*
 * initialize the serial monitor for debuggin, the neopixel ring &
 *   the capacitive touch sensor. setup is executed 1 time when the
 *   board is powered up or reset.
 */
void setup() {
			pinMode(10,INPUT);
			pinMode(11,INPUT);
			pinMode(12,INPUT);
			pinMode(13,INPUT);
			Serial.begin(9600);

  //initialize the serial monitor for debugging
  Serial.begin(9600);
  //when debugging, start the serial monitor before uploading the sketch or
  //  you won't see these lines.
  Serial.println("Light strip patterns");
 
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.setBrightness(100);

  //set all pixels to 'off'
  strip.clear();
  strip.show();
  
 
}

//The main loop of the sketch
//Read the capacitive touch sensor. Treat it like a button press
//Each time the sensor is touched, change the neopixel pattern
//according to the mode


//mode is global - keep track of it outside the loop
int mode = 0;

/*
 * loop is executed over & over in arduino sketches
 * for each iteration of loop():
 *   - read the capacitive touch sensor, if it registers a high number it's being touched, so update
 *      the mode, otherwise keep the current mode
 *   - modes start at 0 & are updated 1 time at the start of loop
 *   - the switch/case statement executes a different light pattern for each mode. Some light patterns like
 *     the rainbow have loops that take a while to execute, so they don't react immediately to the cap touch
 *     sensor press (an interrupt could solve this problem, but it's a more advanced topic)
 *    
 * Modify the code below to change colors & create new patterns. Remove and add modes as you like.
 * have fun & ask a mentor or team mate for help if needed.
 */
void loop() {
 
  uint32_t rgbColor = 0;
  Serial.println("\n*********Start of Loop ****************************");

  	int inputPattern = 0;
		//inputPattern = readInput();
		Serial.print("read in:");
		Serial.println(inputPattern);
		delay(10);
    
  

  //do a different pattern for each mode.
	 mode = readInput();
	 Serial.println(mode);
	 switch (mode){         
	        case 0:
	          //Shooting EMPTY GREEN
	          strip.clear();
	          strip.show();
	          delay(5);
	          rgbColor = strip.Color(0,255,0);
	          colorWipe(rgbColor, 70);
	          Serial.println("Im over here-");
	          break;          
	        case 1:
	        //Intaking TRAIL ORANGE
	          //call the cometChase function defined after loop()
	          //HSV orange comet chase pattern
	          cometChase(4900, 10, 50);
	          break;
	        case 2:
	        //Disabled
	          rainbow(10);
	          break;
	        case 3:
	        //Auto  WHITE FLASH 
						theaterChase(12098765,50);
						strip.show();
	        		delay(100);
	        	break;
	      
	        case 4:
	        //We can Shoot SOLID GREEN
	        	strip.fill(strip.Color(0,255,0),0,90);
	        	strip.show();
	        		delay(100);
	        	break;
	
	        case 5:
	        //Teleop RED ALLIANCE 
	        	strip.fill(strip.Color(255,0,0),0,90);
	        	strip.show();
	        		delay(100);
	        	break;
	
	        case 6: 
	        	//Teleop BLUE ALLIANCE
	        	strip.fill(strip.Color(0,0,255),0,90);
	        	strip.show();
	        		delay(100);
	        		break;
	
	       case 7:
	       //Climb TRAILING PURPLE
	       	cometChase(0xcfef, 20, 50);
	       	break;
	
	       case 8:
					//Have a note WHITE FADE
					strip.fill(strip.Color(255,255,255),0,90);
						strip.show();
						delay(5000);
					breathe(30,100,100);
					break;
	       	    		
	        
	 }
  
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
	strip.fill(color,0,90);
	strip.show();
	delay(5);
  for(int i=strip.numPixels(); i>=0; i--) { // For each pixel in strip...
    strip.setPixelColor(i, 0);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(15);                           //  Pause for a moment
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 2 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 2*65536. Adding 256 to firstPixelHue each time
  // means we'll make 2*65536/256 = 512 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 2*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(5);  // Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<5; a++) {  // Repeat 5 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

//comet chase - chases starts with one bright light & then dims each light for
// the length of the tail. HSV - hue, saturation, value are used to make this easier,
// because it allows us to just modify value, keeping the color the same. gamma32 is
// necessary for color correction as the value dims. The neopixels are cleared after
// each round to take of the tail when the outer loop finishes (not the most elegant solution
// for fixing the tail)
void cometChase(uint16_t hsvColor, int tailLen, int delayTime){
   uint32_t rgbcolor;
   int dimFactor = 255/tailLen;
   int brightness =255;
   int currPixel;
   for (int i = 0; i < strip.numPixels(); i++){
      brightness = 255;
      for (int j = 0; j <= tailLen; j++){
        currPixel = i-j;
        if (j == tailLen){
          brightness = 0;
        }
        else if (j != 0){
          brightness = brightness - dimFactor;
        }
        rgbcolor = strip.gamma32(strip.ColorHSV(hsvColor, 255, brightness));
        strip.setPixelColor(currPixel,rgbcolor);
        
      }
      strip.show();
      delay(delayTime);
   }
   strip.clear();
   strip.show();

}



void breathe(int hue,int saturation, int value) {
	for (int i = 0; i < 255; i--) {
		strip.fill(strip.gamma32(strip.ColorHSV(hue, saturation, i)));
		strip.show();
	}
	for (int i = 255; i > 0; i++) {
		strip.fill(strip.gamma32(strip.ColorHSV(hue, saturation, i)));
		strip.show();
	}
}




		
			
	
		

	int readInput(){
		int digitFirst= digitalRead(10);
		int digitSecond= digitalRead(11);
		int digitThird = digitalRead(12);
		int digitFourth = digitalRead(13);
	
		
		Serial.print(digitFourth);
		Serial.print(digitThird);
		Serial.print(digitSecond);
		Serial.println(digitFirst);

		return(digitFourth*8 + digitThird*4 + digitSecond*2 + digitFirst*1);
	}
