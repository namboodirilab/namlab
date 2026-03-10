#include <math.h>
#include <avr/wdt.h>

const int ttl1 = 2; // exposure active (frame) from camera
const int ttl2 = 3; // acquisition active from camera
const int ttl3 = 4; // session active from beh arduino
const int ttl4 = 5; // beh event active from beh arduino
boolean TTLState[4];            // state of lickometer on all 3 licktubes
boolean TTLon;                  // new lick or not
boolean TTLoff;           // was previous lick withdrawn or not?

unsigned long reading;           // variable to temporarily store data being read
unsigned long start;             // timestamp of start of session
unsigned long ts;                // current timestamp


void setup() {
  wdt_disable();                   // Disable watchdog timer on bootup. This prevents constant resetting by the watchdog timer in the endSession() function
  pinMode(ttl1, INPUT);
  pinMode(ttl2, INPUT);
  pinMode(ttl3, INPUT);
  pinMode(ttl4, INPUT);
  Serial.begin(57600);

  while (Serial.available() <= 0) {}   // wait for signal from MATLAB

  TTLState[0] = digitalRead(ttl1); // initialize
  TTLState[1] = digitalRead(ttl2);
  TTLState[2] = digitalRead(ttl3);
  TTLState[3] = digitalRead(ttl4);

  reading = 0;

  while (reading != 48) {              // Before "Start" is pressed in MATLAB GUI
    reading = Serial.read();
  }

    // start session
  start = millis();     
}

void loop() {
  ts = millis() - start; 
  reading = Serial.read(); 

  if (reading == 49) {    // END SESSION
    endSession();                      // end
  }
  ttl();

}


// Check ttl status //////
void ttl() {
  boolean prevTTL;

  prevTTL  = TTLState[0];                // record previous lick1 state
  TTLState[0] = digitalRead(ttl1);       // record new lick1 state
  TTLon    = TTLState[0] > prevTTL;     // determine if lick1 occured
  TTLoff = TTLState[0] < prevTTL; // determine if lick1 was withdrawn

  if (TTLon) {                            // if lick
    Serial.print(1);                       //   code data as lick1 timestamp
    Serial.print(" ");
    Serial.print(ts);                      //   send timestamp of lick
    Serial.print('\n');
  }

  if (TTLoff) {                     // if lick withdrawn
    Serial.print(2);                       //   code data as lick1 withdrawn timestamp
    Serial.print(" ");
    Serial.print(ts);                      //   send timestamp of lick
    Serial.print('\n');
  }

  prevTTL  = TTLState[1];                // record previous lick2 state
  TTLState[1] = digitalRead(ttl2);       // record new lick2 state
  TTLon    = TTLState[1] > prevTTL;     // determine if lick2 occured
  TTLoff = TTLState[1] < prevTTL; // determine if lick2 was withdrawn

  if (TTLon) {                            // if lick
    Serial.print(3);                       //   code data as lick2 timestamp
    Serial.print(" ");
    Serial.print(ts);   
    Serial.print('\n');
  }

  if (TTLoff) {                     // if lick withdrawn
    Serial.print(4);                       //   code data as lick2 withdrawn timestamp
    Serial.print(" ");
    Serial.print(ts);      
    Serial.print('\n');
  }

  prevTTL  = TTLState[2];                // record previous lick3 state
  TTLState[2] = digitalRead(ttl3);       // record new lick3 state
  TTLon    = TTLState[2] > prevTTL;     // determine if lick3 occured
  TTLoff = TTLState[2] < prevTTL; // determine if lick3 was withdrawn

  if (TTLon) {                            // if lick
    Serial.print(5);                       //   code data as lick3 timestamp
    Serial.print(" ");
    Serial.print(ts);      
    Serial.print('\n');
  }

  if (TTLoff) {                     // if lick withdrawn
    Serial.print(6);                       //   code data as lick3 withdrawn timestamp
    Serial.print(" ");
    Serial.print(ts);          
    Serial.print('\n');
  }
  
  prevTTL  = TTLState[3];                // record previous lick3 state
  TTLState[3] = digitalRead(ttl4);       // record new lick3 state
  TTLon    = TTLState[3] > prevTTL;     // determine if lick3 occured
  TTLoff = TTLState[3] < prevTTL; // determine if lick3 was withdrawn

  if (TTLon) {                            // if lick
    Serial.print(7);                       //   code data as lick3 timestamp
    Serial.print(" ");
    Serial.print(ts);      
    Serial.print('\n');
  }

  if (TTLoff) {                     // if lick withdrawn
    Serial.print(8);                       //   code data as lick3 withdrawn timestamp
    Serial.print(" ");
    Serial.print(ts);          
    Serial.print('\n');
  }

}

void software_Reboot()
{
  wdt_enable(WDTO_500MS);
  while (1)
  {
  }
  wdt_reset();
}

// End session //////////////
void endSession() {

  Serial.print(0);                       //   code data as end of session
  Serial.print(" ");
  Serial.print(ts);    
  Serial.print('\n');

  delay(100);             
  software_Reboot();

}
