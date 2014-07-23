#include <OctoWS2811.h>

const int ledsPerStrip = 144;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

const int num_lights = 144;
const int num_mics = 4;
const int brightness_steps = 4;

int val = 0;                                   // value to store mic1
int val2 = 0;                                  // value to store mic2 
int val3 = 0;                                  // value to store mic 3
int val4 = 0;                                  //value to store mic 4
// colors 
int r = 30;                                      // red value 
int g = 0;                                   // green value
int b = 30;                                    // blue value

int ressteps = 100;

int smoothedVal     = 0;    // smoothed result
int smoothedVal2     = 0;    // smoothed result
int smoothedVal3     = 0;    // smoothed result
int smoothedVal4     = 0;    // smoothed result
int samples         = 4;    // amount of samples

const int mic = 3;      // pin that mic1 is attached to
const int mic2 = 4;      // pin that mic2 is attached to 
const int mic3 = 5;      // pin that mic3 is attached to 
const int mic4 = 8;      // pin that mic4 is attached to 


const int ledPin = 9;           // pin that the LED is attached to
const int indicatorLedPin = 13; // pin that the built-in LED is attached to
const int buttonPin = 8;        // pin that the button is attached to


// These variables will change:
int sensorMin = 1023;  // minimum sensor value
int sensorMin2 = 1023;  // minimum sensor value2
int sensorMin3 = 1023;  // minimum sensor value3
int sensorMin4 = 1023;  // minimum sensor value4

int sensorMax = 0;     // maximum sensor value
int sensorMax2 = 0;    // maximum sensor value2
int sensorMax3 = 0;     // maximum sensor value3
int sensorMax4 = 0;     // maximum sensor value4

int ledsize = (num_lights / num_mics); //size of without the * i

void setup() {
  leds.begin();          // initialize pixel strip
  leds.show();           // Initialize all pixels to 'off'
  pinMode(indicatorLedPin, OUTPUT);
  pinMode (ledPin, OUTPUT);
  pinMode (buttonPin, INPUT);
  Serial.begin(9600);
  
  //calibrate mic's min and max here
  
  for(int c=0; c<1000000; c++){
  digitalWrite(indicatorLedPin, HIGH);
    val = analogRead(mic);
    val2 = analogRead(mic2);
    val3 = analogRead(mic3);
    val4 = analogRead(mic4);
    // record the maximum/minimum sensor value1
    if (val > sensorMax) {
    sensorMax = val;
    }
    if (val < sensorMin) {
    sensorMin = val;
    }
    // record the maximum/minimum sensor value2
    if (val2 > sensorMax2) {
    sensorMax2 = val2;
    }
    if (val2 < sensorMin2) {
    sensorMin2 = val2;
    }
    // record the maximum/minimum sensor value3
    if (val3 > sensorMax3) {
    sensorMax3 = val3;
    }
    if (val3 < sensorMin3) {
    sensorMin3 = val3;
    }
    // record the maximum/minimum sensor value4
    if (val4 > sensorMax4) {
    sensorMax4 = val4;
    }
    if (val4 < sensorMin4) {
    sensorMin4 = val4;
    }
  }
  //done calibrating 
  digitalWrite(indicatorLedPin, LOW);
}

void loop() {

  //mic 1 
 val = running_Average(mic);  // smooth the mic readings with Running Average function
 val = map(val, sensorMin, sensorMax/2, 0, ressteps);  // map the range to 0-255
 val = constrain(val, 0, ressteps);
 Serial.println(val); // for debugging 
 newledRange(val, 1); 
 //mic2
 val2 = running_Average2(mic2);  // smooth the mic readings with Running Average function
 val2 = map(val2, sensorMin2, sensorMax2, 0, ressteps);  // map the range to 0-255
 val2 = constrain(val2, 0, ressteps);
 newledRange(val2, 2); 

 //mic3
 val3 = running_Average3(mic3);  // smooth the mic readings with Running Average function
 val3 = map(val3, sensorMin3, sensorMax3, 0, ressteps);  // map the range to 0-255
 val3 = constrain(val3, 0, ressteps);
 newledRange(val3, 3); 
 //mic4
 val4 = running_Average4(mic4);  // smooth the mic readings with Running Average function
 val4 = map(val4, sensorMin4, sensorMax4, 0, ressteps);  // map the range to 0-255
 val4 = constrain(val4, 0, ressteps);
 newledRange(val4, 4); 
 
leds.show();                               // refresh the strip

}
//average mic1
int running_Average(int microphone) {
  // read the value from the sensor
  // the range from an analog input is 0 - 1023
 int sensorVal = analogRead(microphone); 
 
  smoothedVal = smoothedVal + ((sensorVal - smoothedVal)/samples);

  return smoothedVal; 
 
}
//average mic2
int running_Average2(int microphone) {
  // read the value from the sensor
  // the range from an analog input is 0 - 1023
  int sensorVal = analogRead(microphone); 
 
  smoothedVal2 = smoothedVal2 + ((sensorVal - smoothedVal2)/samples);

  return smoothedVal2; 
 
}
//average mic3
int running_Average3(int microphone) {
  // read the value from the sensor
  // the range from an analog input is 0 - 1023
  int sensorVal = analogRead(microphone); 
 
  smoothedVal3 = smoothedVal3 + ((sensorVal - smoothedVal3)/samples);

  return smoothedVal3; 
 
}
//average mic4
int running_Average4(int microphone) {
  // read the value from the sensor
  // the range from an analog input is 0 - 1023
  int sensorVal = analogRead(microphone); 
 
  smoothedVal4 = smoothedVal4 + ((sensorVal - smoothedVal4)/samples);

  return smoothedVal4; 
 
}

void newledRange(int mappedval, int mic_num) {
  int rangemax = (ledsize * mic_num) -1;
  int rangemin = (ledsize * mic_num) - ledsize;
  boolean usemidpoint = false;
  int midpoint;
  int midpointlow;
  int midpointhigh;
 
  if (ledsize % 2 == 0){
    usemidpoint = true;
    midpoint = rangemin + (ledsize/2) -1; //15,45 when num_mics =
    }
  
    else{
      midpointlow = floor(ledsize/2) + rangemin - 1; 
      midpointhigh = midpointlow + 1; //ceil(ledsize/2) + rangemin - 1
       }
  
  int strength = (((mappedval * ledsize) / ressteps) / 2); // x gives a ratio of mappedval to the size of the set //MAKE SIZE OF MIDPOINT INSTEAD!!!
  //int brightness = setbrightness(mappedval);
  /*
  int strength_steps = brightness_steps * strength;//(ledsize/2);
  const int brightness_percent = 255/brightness_steps;
  int x = 0;
  
  for ( int i = 1; i <= strength_steps; i++ ) {
    int mod = i % brightness_steps;
   int brightness = map(mod, 0, brightness_steps-1, brightness_percent, 255); 
      if ( mod == 0 ){
        x++;
      }
      */
     // Serial.println(midpoint);
      turnoffleds(rangemin, rangemax);
      if ( usemidpoint == true ) {
        for(int i=midpoint; i<midpoint+strength; i++){
        leds.setPixel(i, 30, 30, 0);
        }
          for(int j=midpoint; j>midpoint-strength; --j){
            leds.setPixel(j, 30, 30, 0);
            }
      } 
       else {
         for(int i=midpoint; i<midpoint+strength; i++){
           leds.setPixel(i, 30, 30, 0);
            }
         for(int j=midpoint; j>midpoint-strength; --j){
           leds.setPixel(j, 30, 30, 0);
           }
      }
      
     
  //}
  
}

void turnoffleds(int rangemin, int rangemax) {
  for ( int i = rangemin; i <= rangemax; i++ ) {
    leds.setPixel(i, 0, 0, 0);
    } 
}
  

