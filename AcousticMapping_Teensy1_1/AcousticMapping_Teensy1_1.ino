#include <OctoWS2811.h>

const int ledsPerStrip = 144;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

const int num_lights = 144;
const int num_mics = 4;
const int brightness_steps = 4;

int val = 0;                     // value to store mic1
int val2 = 0;                    // value to store mic2 
int val3 = 0;                    // value to store mic 3
int val4 = 0;                    //value to store mic 4
// colors 
int r = 30;                      // red value 
int g = 0;                       // green value
int b = 30;                      // blue value

int ressteps = 1000;
const int numReadings = 20;      // number of readings for smoothing the inputs 

int readings[numReadings];       // the readings from the analog input
int total = 0;                   // the running total
int average = 0;                 // average of mic 1
int myIndex = 0;

int readings2[numReadings];      // the readings from the analog input
int total2 = 0;                  // the running total
int average2 = 0;                // average of mic 2
int myIndex2 = 0;

int readings3[numReadings];      // the readings from the analog input
int total3 = 0;                  // the running total
int average3 = 0;                // average of mic 3
int myIndex3 = 0;

int readings4[numReadings];      // the readings from the analog input
int total4 = 0;                  // the running total
int average4 = 0;                // average of mic 4
int myIndex4 = 0;

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

int sensorMaxAVG = 0;  //average max of all 4 mics 

int ledsize = (num_lights / num_mics); //size of without the * i

int x = 1.5;
/*-----------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------------------------*/
void setup() {
  leds.begin();          // initialize pixel strip
  leds.show();           // Initialize all pixels to 'off'
  pinMode(indicatorLedPin, OUTPUT); 
  pinMode (ledPin, OUTPUT);
  pinMode (buttonPin, INPUT);
  Serial.begin(9600);

  //CALIBRATE ALL MIC'S MIN & MAX//
  
  for(int c=0; c<4000; c++){  
  calibrate_Lights(c);  
  digitalWrite(indicatorLedPin, HIGH);
    val  = running_Average(mic);     // Grab the average of Mic 1 
    val2 = running_Average2(mic2);   // Grab the average of Mic 2
    val3 = running_Average3(mic3);   // Grab the average of Mic 3
    val4 = running_Average4(mic4);   // Grab the average of Mic 4
    
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
  sensorMaxAVG = ((sensorMin+sensorMin2+sensorMin3+sensorMin4)/4);
  digitalWrite(indicatorLedPin, LOW);

}
void loop() {
 //mic 1 
 val = running_Average(mic);  // smooth the mic readings with Running Average function
 Serial.println(val);
 val = map(val, sensorMin, sensorMaxAVG, 0, ressteps);  // map the range to res steps
 val = constrain(val, 0, ressteps);
 newledRange(val, 1); 

 //mic2
 val2 = running_Average2(mic2);  // smooth the mic readings with Running Average function
 val2 = map(val2, sensorMin2, sensorMaxAVG, 0, ressteps);  // map the range to 0-255
 val2 = constrain(val2, 0, ressteps);
 newledRange(val2, 2); 

 //mic3
 val3 = running_Average3(mic3);  // smooth the mic readings with Running Average function
 val3 = map(val3, sensorMin3, sensorMaxAVG, 0, ressteps);  // map the range to 0-255
 val3 = constrain(val3, 0, ressteps);
 newledRange(val3, 3); 

 //mic4
 val4 = running_Average4(mic4);  // smooth the mic readings with Running Average function
 val4 = map(val4, sensorMin4, sensorMaxAVG, 0, ressteps);  // map the range to 0-255
 val4 = constrain(val4, 0, ressteps);
 newledRange(val4, 4); 


leds.show();                               // refresh the strip

}

//AVERAGE MIC 1//
int running_Average(int input){  
  // subtract the last reading:
  total= total - readings[myIndex];         
  // read from the sensor:  
  readings[myIndex] = analogRead(input); 
  // add the reading to the total:
  total= total + readings[myIndex];       
  // advance to the next position in the array:  
  myIndex = myIndex + 1;                    

  // if we're at the end of the array...
  if (myIndex >= numReadings)              
    // ...wrap around to the beginning: 
    myIndex = 0;                           

  // calculate the average:
  average = total / numReadings;  
  delay(1);  
  // send it to the computer as ASCII digits
  return average;   
}
int running_Average2(int input){  
  // subtract the last reading:
  total2= total2 - readings2[myIndex2];         
  // read from the sensor:  
  readings2[myIndex2] = analogRead(input); 
  // add the reading to the total:
  total2= total2 + readings2[myIndex2];       
  // advance to the next position in the array:  
  myIndex2 = myIndex2 + 1;                    

  // if we're at the end of the array...
  if (myIndex2 >= numReadings)              
    // ...wrap around to the beginning: 
    myIndex2 = 0;                           

  // calculate the average:
  average2 = total2 / numReadings;  
  delay(1);  
  // send it to the computer as ASCII digits
  return average2;            
}
//AVERAGES MIC 3
int running_Average3(int input){  
  // subtract the last reading:
  total3= total3 - readings3[myIndex3];         
  // read from the sensor:  
  readings3[myIndex3] = analogRead(input); 
  // add the reading to the total:
  total3= total3 + readings3[myIndex3];       
  // advance to the next position in the array:  
  myIndex3 = myIndex3 + 1;                    

  // if we're at the end of the array...
  if (myIndex3 >= numReadings)              
    // ...wrap around to the beginning: 
    myIndex3 = 0;                           

  // calculate the average:
  average3 = total3 / numReadings;  
  delay(1);  
  // send it to the computer as ASCII digits
  return average3;   
}
//average mic4
int running_Average4(int input){  
  // subtract the last reading:
  total4= total4 - readings4[myIndex4];         
  // read from the sensor:  
  readings4[myIndex4] = analogRead(input); 
  // add the reading to the total:
  total4= total4 + readings4[myIndex4];       
  // advance to the next position in the array:  
  myIndex4 = myIndex4 + 1;                    

  // if we're at the end of the array...
  if (myIndex4 >= numReadings)              
    // ...wrap around to the beginning: 
    myIndex4 = 0;                           

  // calculate the average:
  average4 = total4 / numReadings;  
  delay(1);  
  // send it to the computer as ASCII digits
  return average4;   
 
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

void calibrate_Lights(int c){
    c = map(c,0,4000,0,144);
    leds.setPixel(c, 0,10,30);
    leds.show();
    delayMicroseconds(1);
    leds.setPixel(c,0,0,0);
}
  

