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
int solenoid3 = 43;   // pin for solenoid3 - gives rewards
int solenoid4 = 45;   // pin for so\lenoid4
int framein   = 32;   // pin receiving the TTL input for frame start
int vacuum    = 38;   // pin for vacuum
int laser     = 40;   // laser to pin 9
int ttloutpin = 42;   // ttl out pin for starting imaging
int ttloutstoppin = 44; // ttl out pin for stopping imaging

// Global variables
const int numcues = 5;
const int numstates = 6;          //includes reward state
const int rewstate = 6;
unsigned long reading;            // variable to temporarily store data being read

unsigned long start;              // timestamp of start of session
unsigned long ts;                 // current timestamp
unsigned long transitions[numstates][numstates]; // transition probability matrix
int cuetype[numcues];
int cuesource[numcues];
unsigned long cuefreq[numcues];
bool pulsecue[numcues];           // 1=pulse, 0=no pulse
unsigned long cuepulsedur[numcues][2];
unsigned long statedur[numstates];
unsigned long ISI[numstates];
bool pulselaser[numstates];        // 1=pulse, 0=no pulse
unsigned long laserpulsedur[numstates][2];
unsigned long laserdelay[numstates];
unsigned long laserdur[numstates];
unsigned long vacuumdelay;         //time from solenoid off to vacuum on
unsigned long vacuumdur;           //vacuum duration
bool sessionendtime;               // true if using time for session length, false if using max rewards
unsigned long sesdur;              // session duration (min or number of rewards)
bool timedses;                     // true if session is timed, false if max number of rewards

unsigned long vacuumOn_ts = 0;
unsigned long vacuumOff_ts = 0;
unsigned long cueOff_ts = 0;
unsigned long nextState_ts = 0;
unsigned long rewOff_ts = 0;
unsigned long cuepulseOn_ts = 0;
unsigned long cuepulseOff_ts = 0;
unsigned long laserOn_ts = 0;
unsigned long laserOff_ts = 0;
unsigned long laserpulseOn_ts = 0;
unsigned long laserpulseOff_ts = 0;

int state = 1;                     //current state
int rewcount = 0;
bool initcues = true;
boolean lickState = false;         // state of lickometer
boolean licked;                    // new lick or not
boolean lickwithdrawn;             // was previous lick withdrawn or not?
  

//boolean framestate;              // state of frame input
//boolean frameon;                 // did frame input turn on?


void setup() {
  // put your setup code here, to run once:
  
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

  while (reading != 48) {              // Before "Start" is pressed in MATLAB GUI
    reading = Serial.read();

    // Test Cue1,2,3,4, or 5
    if (reading == 50 || reading == 51 || reading == 52 || reading == 53 || reading == 54) {                       
      reading -= 50;
      if (pulsecue[reading]) {          //TEST PULSE CUE
        int numpulses = floor(statedur[reading]/(cuepulsedur[reading][1] + cuepulsedur[reading][2]));
        for (int i = 0; i < numpulses; i++) {
          if (cuetype[reading] == 1) {    //pulse tone
            tone(cuesource[reading], cuefreq[reading]); //turn on tone
            delay(cuepulsedur[reading][1]);             //keep tone on for cuepulsedur on time
            noTone(cuesource[reading]);
            delay(cuepulsedur[reading][2]);             //keep tone off for cuepulsedur off time
          } else {                        //pulse light
            digitalWrite(cuesource[reading], HIGH);     //turn on light
            delay(cuepulsedur[reading][1]);             
            digitalWrite(cuesource[reading], LOW);
            delay(cuepulsedur[reading][2]);             //keep light off for cuepulsedur off time          
          }
        }
      } else {                          //TEST CUE (NO PULSE)
        if (cuetype[reading] == 1) {    //tone
          tone(cuesource[reading], cuefreq[reading]);     //turn on tone
          delay(statedur[reading]);
          noTone(cuesource[reading]);
        } else {                        //light
          digitalWrite(cuesource[reading], HIGH);         //turn on light
          delay(1000);
          digitalWrite(cuesource[reading], LOW);
        }
      }
    }  

//    if (reading == 65) {                 // MANUAL solenoid 1
//      digitalWrite(solenoid1, HIGH);          // turn on solenoid 1
//      delay(statedur[rewstate]);
//      digitalWrite(solenoid1, LOW);           // turn off solenoid 1
//    }
//
//    if (reading == 66) {                 // PRIME SOLENOID 1
//      digitalWrite(solenoid1, HIGH);          // turn on solenoid 1
//    }
//
//    if (reading == 67) {                 // TURN OFF SOLENOID 1
//      digitalWrite(solenoid1, LOW);           // turn off solenoid 1
//    }
//
//    if (reading == 68) {                 // MANUAL solenoid 2s
//      digitalWrite(solenoid2, HIGH);          // turn on solenoid 2
//      delay(statedur[rewstate]);
//      digitalWrite(solenoid2, LOW);           // turn off solenoid 2
//    }
//
//    if (reading == 69) {                 // PRIME SOLENOID 2
//      digitalWrite(solenoid2, HIGH);          // turn on solenoid 2
//    }
//
//    if (reading == 70) {                 // TURN OFF SOLENOID 2
//      digitalWrite(solenoid2, LOW);           // turn off solenoid 2
//    }

    if (reading == 71) {                 // MANUAL solenoid 3
      digitalWrite(solenoid3, HIGH);          // turn on solenoid 3
      delay(statedur[rewstate-1]);
      digitalWrite(solenoid3, LOW);           // turn off solenoid 3
    }

    if (reading == 72) {                 // PRIME SOLENOID 3
      digitalWrite(solenoid3, HIGH);          // turn on solenoid 3
    }

    if (reading == 73) {                 // TURN OFF SOLENOID 3
      digitalWrite(solenoid3, LOW);           // turn off solenoid 3
    }

//    if (reading == 74) {                 // MANUAL solenoid 4
//      digitalWrite(solenoid4, HIGH);          // turn on solenoid 4
//      delay(statedur[rewstate]);
//      digitalWrite(solenoid4, LOW);           // turn off solenoid 4
//    }
//
//    if (reading == 75) {                 // PRIME SOLENOID 4
//      digitalWrite(solenoid4, HIGH);          // turn on solenoid 4
//    }
//
//    if (reading == 76) {                 // TURN OFF SOLENOID 4
//      digitalWrite(solenoid4, LOW);           // turn off solenoid 4
//    }

//    if (reading == 77) {                 // MANUAL lickretractsolenoid11
//      digitalWrite(lickretractsolenoid1, HIGH);          // turn on lickretractsolenoid1
//      delay(statedur[rewstate]);
//      digitalWrite(lickretractsolenoid1, LOW);           // turn off lickretractsolenoid1
//    }
//
//    if (reading == 78) {                 // PRIME LICKRETRACTSOLENOID 1
//      digitalWrite(lickretractsolenoid1, HIGH);          // turn on lickretractsolenoid1
//    }
//
//    if (reading == 79) {                 // TURN OFF LICKRETRACTSOLENOID 1
//      digitalWrite(lickretractsolenoid1, LOW);           // turn off lickretractsolenoid1
//    }
//
//    if (reading == 80) {                 // MANUAL lickretractsolenoid12
//      digitalWrite(lickretractsolenoid2, HIGH);          // turn on lickretractsolenoid2
//      delay(statedur[rewstate]);
//      digitalWrite(lickretractsolenoid2, LOW);           // turn off lickretractsolenoid2
//    }
//
//    if (reading == 81) {                 // PRIME LICKRETRACTSOLENOID 2
//      digitalWrite(lickretractsolenoid2, HIGH);          // turn on lickretractsolenoid2
//    }
//
//    if (reading == 82) {                 // TURN OFF LICKRETRACTSOLENOID 2
//      digitalWrite(lickretractsolenoid2, LOW);           // turn off lickretractsolenoid2
//    }

    if (reading == 86) {                 // Vacuum
      digitalWrite(vacuum, HIGH);          // turn on vacuum
      delay(vacuumdur);
      digitalWrite(vacuum, LOW);           // turn off vacuum
    }

    if (reading == 56) {                 // TEST LASER
      digitalWrite(laser, HIGH);         // turn on LASER
      delay(5000);
      digitalWrite(laser, LOW);         // turn off LASER
    }
  }
  

  // start session
  start = millis();                    // start time
 
//  cues();                              //begin state 1
//  cueOff_ts = start + statedur[state-1];
}

void loop() {
  // put your main code here, to run repeatedly:
  ts = millis() - start;               // find time since start
  reading = Serial.read();             // look for signals from MATLAB

  if (initcues) {
    cues();
    initcues = false;
  }
  // Arduino outputs
  // 0 = Session ended
  // 1 = Lick1 onset
  // 2 = Lick1 offset
  // 3 = Lick2 onset
  // 4 = Lick2 offset                  // leave possible codes for a future lick tube
  // 5 = Lick3 onset
  // 6 = Lick3 offset
  // 7 = Background solenoid
  // 8 = Fixed solenoid 1
  // 9 = Fixed solenoid 2
  // 10 = Fixed solenoid 3                       // leave possible codes for future solenoid
  // 11 = Fixed solenoid 4
  // 14 = vacuum
  // 15 = CS1
  // 16 = CS2
  // 17 = CS3                                   // leave possible codes for future CS
  // 18 = CS4
  // 19 = CS5
  // 21 = light1
  // 22 = light2
  // 23 = light 3
  // 25 = both CSsound1 and CSlight1
  // 26 = both CSsound2 and CSlight2
  // 27 = both CSsound3 and CSlight3
  // 30 = frame
  // 31 = laser

  if (reading == 49) {    // MATLAB SIGNAL END SESSION
      endSession();       // end
  }

  licking();  //check for licking

  //if ts > nextstate
      //pick next state
      //set as current state
      //turn on cue or deliver rew
      //set cue off time
      
  if (ts >= nextState_ts && nextState_ts != 0) {
    chooseNextState();
    if (state == rewstate) {
      deliverRew();
      rewOff_ts = ts + statedur[state-1];
    } else {
      cues();
    }
    nextState_ts = 0;
  }
  
  if (ts >= cueOff_ts && cueOff_ts != 0) {
    noTone(speaker1);                   // turn off sound
    noTone(speaker2);
    digitalWrite(light1, LOW);          // turn off light
    digitalWrite(light2, LOW);
    cueOff_ts = 0;
    nextState_ts = ts + ISI[state-1];   //set next state time
  }

  // Pulse cue
  if (ts >= cuepulseOff_ts && cuepulseOff_ts != 0 && ts < cueOff_ts) {
    noTone(speaker1);                   // turn off tone
    noTone(speaker2);
    digitalWrite(light1, LOW);          // turn off light
    digitalWrite(light2, LOW);
    cuepulseOn_ts = ts + cuepulsedur[state-1][1];
    cuepulseOff_ts = 0;
  }

  if (ts >= cuepulseOn_ts && cuepulseOn_ts != 0 && ts < cueOff_ts) {
    if (cuetype[state-1] == 1) {
      tone(cuesource[state-1], cuefreq[state-1]);     //turn on tone
    } else {
      digitalWrite(cuesource[state-1], HIGH);         //turn on light
    }              
    cuepulseOff_ts = ts + cuepulsedur[state-1][0];    // Cue pulsing
    cuepulseOn_ts = 0;                                // No cue pulsing
  }
  
  if (ts >= laserOn_ts && laserOn_ts != 0) {
    digitalWrite(laser, HIGH);               //turn on laser              
    laserOff_ts = ts + laserdur[state-1];    //set laser off time
    laserpulseOff_ts = ts + laserpulsedur[state-1][0];
  }
      
  
  
  //if ts > rewoff
      //turn off solenoid
      //wait dur/set vacuum time
      
  if (ts >= rewOff_ts && rewOff_ts != 0) {
    digitalWrite(solenoid3, LOW);
    vacuumOn_ts = ts + vacuumdelay;
    nextState_ts = ts + ISI[state-1];
    rewOff_ts = 0;
  }
  
  //if ts > vacuumtime
      //turn on vacuum
      //set vacuum off time
      
  if (ts > vacuumOn_ts && vacuumOn_ts != 0) {
    Serial.print(14);         // code vacuum
    Serial.print(" ");
    Serial.print(ts);          
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
    digitalWrite(vacuum, HIGH);          // turn on vacuum
    vacuumOff_ts = ts + vacuumdur;
    vacuumOn_ts = 0;
  }
  
  //if ts > vacuumoff
      //turn off vacuum
      //set nextstate time
      
  if (ts > vacuumOff_ts && vacuumOff_ts != 0) {
    digitalWrite(vacuum, LOW);          // turn off vacuum
    vacuumOff_ts = 0;
  }
}

void chooseNextState() {
   long u = random(0, 100);
   long prob_sum[numstates];
   prob_sum[0] = transitions[0][state-1];
   for (int i = 1; i < numstates; i++) {
    prob_sum[i] = prob_sum[i-1] + transitions[i][state-1];     //get probabilities from matrix and put in array
   }
   for (int i = 0; i < numstates; i++) {
    if (u <= prob_sum[i]) {   //go through array seeing if <= cumulative prob
      state = i + 1;
      break;
    }
   }
}

// Check lick status //////
void licking() {
  boolean prevLick;

  prevLick  = lickState;                // record previous lick3 state
  lickState = digitalRead(lick3);       // record new lick3 state
  licked    = lickState > prevLick;     // determine if lick3 occured
  lickwithdrawn = lickState < prevLick; // determine if lick3 was withdrawn

  if (licked) {                            // if lick
    Serial.print(5);                       //   code data as lick3 timestamp
    Serial.print(" ");
    Serial.print(ts);                      //   send timestamp of lick
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
  }

  if (lickwithdrawn) {                     // if lick withdrawn
    Serial.print(6);                       //   code data as lick3 withdrawn timestamp
    Serial.print(" ");
    Serial.print(ts);                      //   send timestamp of lick
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
  }
}

void deliverRew() {
  Serial.print(10);         // code data as fixed solenoid 3
  Serial.print(" ");
  Serial.print(ts);         // send timestamp of cue
  Serial.print(" ");
  Serial.print(0);          //is this the right code for delivering rew?
  Serial.print('\n');
  digitalWrite(solenoid3, HIGH);
}

// DELIVER CUE //////////////
void cues() {
    Serial.print(14 + state);         // code data as CS1,2,3,4 or 5 timestamp
    Serial.print(" ");
    Serial.print(ts);                 // send timestamp of cue
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');

    if (cuetype[state-1] == 1) {
      tone(cuesource[state-1], cuefreq[state-1]);     //turn on tone
    } else {
      digitalWrite(cuesource[state-1], HIGH);         //turn on light
    }

    if (pulsecue[state-1]) {
      cuepulseOff_ts = ts + cuepulsedur[state-1][0];  // Cue pulsing
      cuepulseOn_ts = 0;
    }
    else {
      cuepulseOff_ts = 0;                         // No cue pulsing
      cuepulseOn_ts = 0;                          // No cue pulsing
    }

    cueOff_ts = ts + statedur[state-1];

    if (pulselaser[state-1]) {
      laserOn_ts = ts + laserdelay[state-1];
    }
}

// Accept parameters from MATLAB
void getParams() {
  int pn = 101;                             // number of parameter inputs
  unsigned long param[pn];                  // parameters

  for (int p = 0; p < pn; p++) {
    reading = Serial.parseInt();           // read parameter
    param[p] = reading;                    // convert to int
  }
  reading = 0;
  int p = 0;    // first param

  for (int i = 0; i < numstates; i++) {
    for (int j = 0; j < numstates; j++) {
      transitions[j][i]= param[p];    //matrix organized same as ui table
      p++;
    }
  }

  for (int i = 0; i < numcues; i++) {
    cuetype[i] = param[p];
    p++;
  }

  for (int i = 0; i < numcues; i++) {
    cuefreq[i] = param[p];
    p++;
  }

  for (int i = 0; i < numcues; i++) {
    if (cuetype[i] == 1) {   //sound cue
      if (param[p] == 1) {
        cuesource[i] = speaker1;
      } else {
        cuesource[i] = speaker2;
      }
    } else {                 // light cue
      if (param[p] == 1) {
        cuesource[i] = light1;
      } else {
        cuesource[i] = light2;
      }
    }
    p++;
  }

  for (int i = 0; i < numstates; i++) {
    statedur[i] = param[p];
    p++;
  }

  for (int i = 0; i < numcues; i++) {
    for (int j = 0; j < 2; j++) {
      cuepulsedur[i][j] = param[p];   //each row = on, off times for each cue
      p++;
    }
  }

  for (int i = 0; i < numcues; i++) {
    if (cuepulsedur[i][0] == 0 && cuepulsedur[i][1] == 0) {
      pulsecue[i] = false;
    } else {
      pulsecue[i] = true;
    }
  }

  for (int i = 0; i < numstates; i++) {
    ISI[i] = param[p];
    p++;
  }
  
  for (int i = 0; i < numstates; i++) {
    for (int j = 0; j < 2; j++) {
      laserpulsedur[i][j] = param[p];   //each row = on, off times for each state
      p++;
    }
  }

  for (int i = 0; i < numstates; i++) {
    if (laserpulsedur[i][0] == 0 && laserpulsedur[i][1] == 0) {
      pulselaser[i] = false;
    } else {
      pulselaser[i] = true;
    }
  }

  for (int i = 0; i < numstates; i++) {
    laserdelay[i] = param[p];
    p++;
  }

  for (int i = 0; i < numstates; i++) {
    laserdur[i] = param[p];
    p++;
  }

  vacuumdelay = param[p];
  p++;

  vacuumdur = param[p];
  p++;

  sesdur = param[p];
  p++;

  if (param[p] == 1) {   //ses dur is minutes
    timedses = true;
  } else {               //sesdur is max number of rewards
    timedses = false;
  }  
}

// End session //////////////
void endSession() {

  // TURN OFF 2P IMAGING
  //  digitalWrite(ttloutstoppin, HIGH);
  //  delay(100);
  //  digitalWrite(ttloutstoppin, LOW);

  //TURN OFF PHOTOMETRY
  digitalWrite(ttloutpin, LOW);

  Serial.print(0);                       //   code data as end of session
  Serial.print(" ");
  Serial.print(ts);                      //   send timestamp
  Serial.print(" ");
  Serial.print(0);
  Serial.print('\n');

  digitalWrite(solenoid1, LOW);                 //  turn off solenoid
  digitalWrite(solenoid2, LOW);                 //  turn off solenoid
  digitalWrite(solenoid3, LOW);                 //  turn off solenoid
  digitalWrite(solenoid4, LOW);                 //  turn off solenoid
  digitalWrite(lickretractsolenoid1, LOW);
  digitalWrite(lickretractsolenoid2, LOW);
  digitalWrite(vacuum, LOW);                 //  turn off solenoid
  noTone(speaker1);                         //  turn off tone
  noTone(speaker2);                         //  turn off tone
  delay(100);                              //  wait
  //while(1){}                               //  Stops executing the program
  //asm volatile (" jmp 0");                 //  reset arduino; this is unclean and doesn't reset the hardware
//  delete [] cueList;
//  int *cueList = 0;
//  delete [] Laserontrial;
//  int *Laserontrial = 0;
  software_Reboot();

}

void software_Reboot() {
  wdt_enable(WDTO_500MS);
  while (1){
  }
  wdt_reset();
}


  
