// The original script sets trial-by-trial LASER randomly on 50% of the trials (shuffled) without consideration of whether a trial is CS+ or CS-
// Here, there are going to be 0.8*numCSminus (=40) CS+ laser trials and 0.8*numCSminus (=40) CS- laser trials with 10 each being laser off.
// Added on 8/22/2016

// Records event from pin 'lick' and sends it
// through serial port as the time of event from the
// "start". Cues will be triggered throgh pin "cue".
// Designed to be used with MATLAB.
//
// Program will wait for signal from MATLAB containing paramenters for experiment. Following parameters need to be in the format xxx+xxx+xxx+xxx+xxx+xxx+xxx+xxx+xxx+xxx+xxx+xxx+xxx
// Every temporal parameter is expressed in units of milliseconds. Parameters to be set in MATLAB include

#include <math.h>
#include <avr/wdt.h>

// Pins
int lick1     = 22;   // lick1 sensor
int lick2     = 24;   // lick2 sensor
int lick3     = 26;   // lick3 sensor
int speaker1  = 28;   // pin for speaker 1
int speaker2  = 30;   // pin for speaker 2
int light1    = 23;   // pin for light 1, light 1 is used to indicate that animal has met golickreq if CSsignal == 2 or 3;
int light2    = 25;   // pin for light 2, light 2 is used to indicate that animal has met golickreq if CSsignal == 2 or 3;
int lickretractsolenoid1 = 31;  // pin for lick retraction solenoid 1
int lickretractsolenoid2 = 33;  // pin for lick retraction solenoid 2
int solenoid1 = 35;   // pin for solenoid1
int solenoid2 = 41;   // pin for solenoid2
int solenoid3 = 43;   // pin for solenoid3
int solenoid4 = 45;   // pin for solenoid4
int framein   = 32;   // pin receiving the TTL input for frame start
int vacuum    = 38;   // pin for vacuum
int laser     = 40;   // laser to pin 9
int ttloutpin = 42;   // ttl out pin for starting imaging
int ttloutstoppin = 44; // ttl out pin for stopping imaging

// Global variables
unsigned long reading;           // variable to temporarily store data being read

const int numCS = 4;             // Number of different CSs
unsigned long solenoidopentime[numCS];

// SETUP code ////////////////
void setup() {
  wdt_disable();                   // Disable watchdog timer on bootup. This prevents constant resetting by the watchdog timer in the endSession() function
  // initialize arduino states
  Serial.begin(57600);
  randomSeed(analogRead(0));       // Generate a random sequence of numbers every time
  pinMode(lick1, INPUT);
  pinMode(lick2, INPUT);
  pinMode(lick3, INPUT);
  pinMode(solenoid1, OUTPUT);
  pinMode(solenoid2, OUTPUT);
  pinMode(solenoid3, OUTPUT);
  pinMode(solenoid4, OUTPUT);
  pinMode(lickretractsolenoid1, OUTPUT);
  pinMode(lickretractsolenoid2, OUTPUT);
  pinMode(vacuum, OUTPUT);
  pinMode(speaker1, OUTPUT);
  pinMode(speaker2, OUTPUT);
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(ttloutpin, OUTPUT);
  pinMode(laser, OUTPUT);
  pinMode(ttloutstoppin, OUTPUT);
  pinMode(framein, INPUT);

  // import parameters
  while (Serial.available() <= 0) {}   // wait for signal from MATLAB
  getParams();

  reading = 0;

  // Key code sent from MATLAB;
  // = 65 for turning solenoid 1 on for r_fxd duration, (A)
  // = 66 for turning solenoid 1 on, (B)
  // = 67 for turning solenoid 1 off, (C)
  // = 68 for turning solenoid 2 on for r_fxd duration, (D)
  // = 69 for turning solenoid 2 on, (E)
  // = 70 for turning solenoid 2 off, (F)
  // = 71 for turning solenoid 3 on for r_fxd duration, (G)
  // = 72 for turning solenoid 3 on, (H)
  // = 73 for turning solenoid 3 off, (I)
  // = 74 for turning solenoid 4 on for r_fxd duration, (J)
  // = 75 for turning solenoid 4 on, (K)
  // = 76 for turning solenoid 4 off, (L)
  // = 77 for turning solenoid 1 on for r_fxd duration 1000 times, (M)
  // = 78 for turning solenoid 1 on for r_fxd duration 1000 times, (N)
  // = 79 for turning solenoid 1 on for r_fxd duration 1000 times, (O)
  // = 80 for turning solenoid 1 on for r_fxd duration 1000 times, (P)

  while (reading != 48) {              // Before "Start" is pressed in MATLAB GUI
    reading = Serial.read();
    if (reading == 65) {                 // MANUAL solenoid 1
      digitalWrite(solenoid1, HIGH);          // turn on solenoid 1
      delay(solenoidopentime[0]);
      digitalWrite(solenoid1, LOW);           // turn off solenoid 1
    }

    if (reading == 66) {                 // PRIME SOLENOID 1
      digitalWrite(solenoid1, HIGH);          // turn on solenoid 1
    }

    if (reading == 67) {                 // TURN OFF SOLENOID 1
      digitalWrite(solenoid1, LOW);           // turn off solenoid 1
    }

    if (reading == 68) {                 // MANUAL solenoid 2
      digitalWrite(solenoid2, HIGH);          // turn on solenoid 2
      delay(solenoidopentime[1]);
      digitalWrite(solenoid2, LOW);           // turn off solenoid 2
    }

    if (reading == 69) {                 // PRIME SOLENOID 2
      digitalWrite(solenoid2, HIGH);          // turn on solenoid 2
    }

    if (reading == 70) {                 // TURN OFF SOLENOID 2
      digitalWrite(solenoid2, LOW);           // turn off solenoid 2
    }

    if (reading == 71) {                 // MANUAL solenoid 3
      digitalWrite(solenoid3, HIGH);          // turn on solenoid 3
      delay(solenoidopentime[2]);
      digitalWrite(solenoid3, LOW);           // turn off solenoid 3
    }

    if (reading == 72) {                 // PRIME SOLENOID 3
      digitalWrite(solenoid3, HIGH);          // turn on solenoid 3
    }

    if (reading == 73) {                 // TURN OFF SOLENOID 3
      digitalWrite(solenoid3, LOW);           // turn off solenoid 3
    }

    if (reading == 74) {                 // MANUAL solenoid 4
      digitalWrite(solenoid4, HIGH);          // turn on solenoid 4
      delay(solenoidopentime[3]);
      digitalWrite(solenoid4, LOW);           // turn off solenoid 4
    }

    if (reading == 75) {                 // PRIME SOLENOID 4
      digitalWrite(solenoid4, HIGH);          // turn on solenoid 4
    }

    if (reading == 76) {                 // TURN OFF SOLENOID 4
      digitalWrite(solenoid4, LOW);           // turn off solenoid 4
    }

    if (reading == 77) {                 // MANUAL lickretractsolenoid11
      for (int a=0; a<1000; a++) {
        digitalWrite(solenoid1, HIGH);          // turn on solenoid 1
        delay(solenoidopentime[0]);
        digitalWrite(solenoid1, LOW);           // turn off solenoid 1
        delay(100);
      }
    }

    if (reading == 78) {                 // MANUAL lickretractsolenoid11
      for (int a=0; a<1000; a++) {
        digitalWrite(solenoid2, HIGH);          // turn on solenoid 1
        delay(solenoidopentime[1]);
        digitalWrite(solenoid2, LOW);           // turn off solenoid 1
        delay(100);
      }
    }

    if (reading == 79) {                 // MANUAL lickretractsolenoid11
      for (int a=0; a<1000; a++) {
        digitalWrite(solenoid3, HIGH);          // turn on solenoid 1
        delay(solenoidopentime[2]);
        digitalWrite(solenoid3, LOW);           // turn off solenoid 1
        delay(100);
      }
    }
    
    if (reading == 80) {                 // MANUAL lickretractsolenoid11
      for (int a=0; a<1000; a++) {
        digitalWrite(solenoid4, HIGH);          // turn on solenoid 1
        delay(solenoidopentime[3]);
        digitalWrite(solenoid4, LOW);           // turn off solenoid 1
        delay(100);
      }
    }

  }
}

// LOOP code ////////////////
void loop() {
  
}


// Accept parameters from MATLAB
void getParams() {
  int pn = 4;                              // number of parameter inputs
  unsigned long param[pn];                  // parameters
  
  
  for (int p = 0; p < pn; p++) {
    reading = Serial.parseInt();           // read parameter
    param[p] = reading;                    // convert to int
  }
  reading = 0;
  
  solenoidopentime[0] = param[0];
  solenoidopentime[1] = param[1];
  solenoidopentime[2] = param[2];
  solenoidopentime[3] = param[3];
}

void software_Reboot()
{
  wdt_enable(WDTO_500MS);
  while (1)
  {
  }
  wdt_reset();
}
