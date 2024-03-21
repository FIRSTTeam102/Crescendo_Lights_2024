
#include <Adafruit_NeoPixel.h>


/*************************************************************
 *    
 *    many of the light patterns found in this project are taken from
 *    adafruit neopixel example code.
 */
#define PIXEL_PIN    7
#define PIXEL_COUNT  90
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB );

int digitFirst = 0;
int digitSecond = 0;
int digitThird = 0; 
int digitFourth = 0; 

/*
 * initialize the serial monitor for debuggin, the neopixel ring &
 *   the capacitive touch sensor. setup is executed 1 time when the
 *   board is powered up or reset.
 */
void setup() {
	//set up the DIO - directly connected to the roborio
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
//Read the digital IO conviert the binary
//digits to an integer and change the neopixel pattern
//according to the mode


//mode is global - keep track of it outside the loop
int mode = 0;
int prevMode = 0;

//Start the rainbow pattern at Hue 0 and then just keep cycling
//to the next hue when you call the rainbow pattern 
long currFirstPixelHue = 0;
int currStartPixel = 0;
/*
 * loop is executed over & over in arduino sketches
 * for each iteration of loop():
 *   - read the digital IO pins, 
 *   - modes start at 0 & are updated 1 time at the start of loop
 *   - the switch/case statement executes a different light pattern for each mode.
 */
void loop() {
 
  uint32_t rgbColor = 0;
  Serial.println("\n*********Start of Loop ****************************");

  //do a different pattern for each mode.
	 mode = readInput();
	 Serial.print("Current mode: ");
	 Serial.println(mode);
	 if (prevMode != mode){
		//reset lights & patterns if we change modes
		strip.clear();
		strip.show();
		currFirstPixelHue = 0;
		currStartPixel = 0;
	 }
	 switch (mode){         
	    case 0:
	        //Shooting EMPTY GREEN
			Serial.println("case 0: Wipe Green");
	        //set strip to green & then turn off 1 at a time
	        rgbColor = strip.Color(0,255,0);
	        colorWipe(rgbColor, 70);
	    break;          
	    case 1:
	        //Intaking TRAIL ORANGE
	        //call the cometChase function defined after loop()
	        //HSV orange comet chase pattern
	        cometChase(4900, currStartPixel, 10, 50);
			if (currStartPixel >= strip.numPixels()) {
				currStartPixel = 0;
			}
			else currStartPixel++;
	    break;
	    case 2:
	        //Disabled
			//the rainbow hues are HSV 0 - 65536 fill each
			//pixel in the strip with the next hue & keep increasing
			//the pixel hue every time you call this mode to keep the
			//rainbow cycling.
			strip.rainbow(currFirstPixelHue);
			strip.show();
			if (currFirstPixelHue < 2*65536){
				currFirstPixelHue +=256;
			}
			else currFirstPixelHue = 0;
	    break;
	    case 3:
	        //Auto  WHITE FLASH 
			theaterChase(12098765,50);
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
	       	cometChase(0xcfef, currStartPixel, 10, 50);
			if (currStartPixel >= strip.numPixels()){
				currStartPixel = 0;
			}
			else currStartPixel++;
	    break;
	
	    case 8:
			//Have a note WHITE FADE
			strip.fill(strip.Color(255,255,255),0,90);
			strip.show();
			delay(500);
			breathe(30,100,100);
		break;
					
		default:
			theaterChase(strip.Color(200,1,160), 50);
	       	    		
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

//comet chase - 
// forms a comet chase pattern if called multiple times. This method shows one comet
// with a fading tail starting with the startPixel, allowing it to be quickly 
// interrupted if a new pattern is called for.
//HSV - hue, saturation, value are used to make the fade easier,
// because it allows a dim on the same color. gamma32 is
// necessary for color correction as the value dims.
//
void cometChase(uint16_t hsvColor, int startPixel, int tailLen, int delayTime){
	uint32_t rgbcolor;
	int dimFactor = 255/tailLen;
	int brightness =255;
	int currPixel;
	int i = startPixel;
    
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
	//make sure the 0 to the comet pixels are off
	strip.fill(strip.Color(0,0,0),0,i-tailLen-1);
    strip.show();
    delay(delayTime);
}

//breath takes 255 steps to go from off to bright & 255 more to go 
//from bright to off
void breathe(int hue,int saturation, int value) {
	for (int i = 0; i < 255; i++) {
		strip.fill(strip.gamma32(strip.ColorHSV(hue, saturation, i)));
		strip.show();
	}
	for (int i = 255; i > 0; i--) {
		strip.fill(strip.gamma32(strip.ColorHSV(hue, saturation, i)));
		strip.show();
	}
}

//Read the mode as a binary digit from
//from the DIO pins and return the decimal integer. 
int readInput(){
	int digitFirst= digitalRead(13);
	int digitSecond= digitalRead(12);
	int digitThird = digitalRead(11);
	int digitFourth = digitalRead(10);
		
	Serial.print("Left to Right: ");
	Serial.print(digitFourth);
	Serial.print(" ");
	Serial.print(digitThird);
	Serial.print(" ");
	Serial.print(digitSecond);
	Serial.print(" ");
	Serial.println(digitFirst);

	return(digitFourth*8 + digitThird*4 + digitSecond*2 + digitFirst);
}