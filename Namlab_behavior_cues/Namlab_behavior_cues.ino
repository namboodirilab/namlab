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

//Parameter meaning 
//0) number of CS1 trials
//1) number of CS2 trials
//2) number of CS3 trials
//3) number of CS4 trials
//4) frequency(kHz) of CS1 tone
//5) frequency(kHz) of CS2 tone
//6) frequency(kHz) of CS3 tone
//7) frequency(kHz) of CS4 tone
//8) predicted 1st fixed solenoid of CS1
//9) predicted 2nd fixed solenoid of CS1
//10) predicted 1st fixed solenoid of CS2
//11) predicted 2nd fixed solenoid of CS2
//12) predicted 1st fixed solenoid of CS3
//13) predicted 2nd fixed solenoid of CS3
//14) predicted 1st fixed solenoid of CS4
//15) predicted 2nd fixed solenoid of CS4
//16) probability of 1st fixed solenoid openning of CS1
//17) probability of 2nd fixed solenoid openning of CS1
//18) probability of 1st fixed solenoid openning of CS2
//19) probability of 2nd fixed solenoid openning of CS2
//20) probability of 1st fixed solenoid openning of CS3
//21) probability of 2nd fixed solenoid openning of CS3
//22) probability of 1st fixed solenoid openning of CS4
//23) probability of 2nd fixed solenoid openning of CS4
//24) openning time(ms) for 1st fixed solenoid of CS1
//25) openning time(ms) for 2nd fixed solenoid of CS1
//26) openning time(ms) for 1st fixed solenoid of CS2
//27) openning time(ms) for 2nd fixed solenoid of CS2
//28) openning time(ms) for 1st fixed solenoid of CS3
//29) openning time(ms) for 2nd fixed solenoid of CS3
//30) openning time(ms) for 1st fixed solenoid of CS4
//31) openning time(ms) for 2nd fixed solenoid of CS4
//32) cue duration(ms) for CS1
//33) cue duration(ms) for CS2
//34) cue duration(ms) for CS3
//35) cue duration(ms) for CS4
//36) delay(ms) to 1st fixed solenoid of CS1
//37) delay(ms) to 2nd fixed solenoid of CS1
//38) delay(ms) to 1st fixed solenoid of CS2
//39) delay(ms) to 2nd fixed solenoid of CS2
//40) delay(ms) to 1st fixed solenoid of CS3
//41) delay(ms) to 2nd fixed solenoid of CS3
//42) delay(ms) to 1st fixed solenoid of CS4
//43) delay(ms) to 2nd fixed solenoid of CS4
//44) flag to signal pulse tone (if==1) or not(if==0) for CS1
//45) flag to signal pulse tone (if==1) or not(if==0) for CS2
//46) flag to signal pulse tone (if==1) or not(if==0) for CS3
//47) flag to signal pulse tone (if==1) or not(if==0) for CS4
//48) speaker for CS1
//49) speaker for CS2
//50) speaker for CS3
//51) speaker for CS4
//52) number of licks required on the first fixed solenoid in order to get reward on the second fixed solenoid of CS1
//53) number of licks required on the first fixed solenoid in order to get reward on the second fixed solenoid of CS2
//54) number of licks requried on the first fixed solenoid in order to get reward on the second fixed solenoid of CS3
//55) number of licks requried on the first fixed solenoid in order to get reward on the second fixed solenoid of CS4
//56) appropriate licktube or solenoid for golickreq of CS1
//57) appropriate licktube or solenoid for golickreq of CS2
//58) appropriate licktube or solenoid for golickreq of CS3
//59) appropriate licktube or solenoid for golickreq of CS4
//60) signal for golickreq met of CS1
//61) signal for golickreq met of CS2
//62) signal for golickreq met of CS3
//63) signal for golickreq met of CS4
//64) mean intertrial interval (ITI) in ms from fixed solenoid to next tone based on exponential distirbution
//65) max ITI; truncation for exponential distribution is set at minimum of maximum ITI or 3*meanITI; if maxITI==meanITI, use fixed ITI
//66) min ITI
//67) flag to set ITI distribution. If==1, draw from exponential, if==0 draw from uniform
//68) which solenoid set to be the background solenoid
//69) background solenoid period, 1/lambda, in ms
//70) background solenoid openning time, in ms
//71) minimum delay between a background solenoid and the next cue, in ms
//72) minimum delay between fixed solenoid to the next background solenoid
//73) signal which experiment mode to run: if==1, run with cues; if==2, give only background poisson solenoids, if==3, give lick dependent rewards
//74) flag to run experiment with background solenoid rates changing on a trial-by-trial basis if==1
//75) total number of background solenoids to stop the session if experimentmode==2, only Poisson session
//76) required number of licks on lick tube 1 to get reward
//77) required number of licks on lick tube 2 to get reward
//78) predicted fixed solenoid for reward after licking lick tube 1
//79) predicted fixed solenoid for reward after licking lick tube 2
//80) probability of fixed solenoid for reward after licking lick tube 1
//81) probability of fixed solenoid for reward after licking lick tube 2
//82) opening time of fixed solenoid for reward after licking lick tube 1
//83) opening time of fixed solenoid for reward after licking lick tube 2
//84) delay time (ms) to fixed solenoid
//85) delay time (ms) to fixed solenoid
//86) delay time (ms) to activate lick tube 1
//87) delay time (ms) to activate lick tube 2
//88) minimum number of rewards delivered to lick tube 1
//89) minimum number of rewards delivered to lick tube 2
//90）signal to meet number of lick requirements of tube 1
//91) signal to meet number of lick requirements of tube 2
//92) sound signal of 70) to pulse or not
//93) sound signal of 71) to pulse or not
//94) sound signal frequency (kHz) of lick tube 1
//95) sound signal frequency (kHz) of lick tube 2
//96) sound signal duration (ms) of lick tube 1
//97) sound signal duration (ms) of lick tube 2
//98) sound signal speaker of lick tube 1
//99) sound signal speaker of lick tube 2
//100) value of the latency wrt the cue at which the laser turns on (0 for cue start; t_fxd for solenoid start)
//101) value of the duration for which the laser remains on. It can pulse within this duration
//102) flag to run sessions with laser turning on randomly if==1
//103) period for which laser is on in a cycle (ms)
//104) period for which laser is off in a cycle (ms); If equal to laserpulseperiod, duty cycle is 50%
//105) flag to turn laser on a trial-by-trial basis
//106) maximum delay to vacuum after cue turns on. Change this if different cues have different delays to reward
//107)to be such that it is longer than the longest delay to reward. Basically, this quantity measures duration of trial.
//108) light number for CS1
//109) light number for CS2
//110) light number for CS3
//111) light number for CS4
//112) variable ratio check for lick 1s. 1==variable, 0==fixed
//113) variable ratio check for lick 2s. 1==variable, 0==fixed
//114) variable interval flag for lick 1s. 1==variable, 0==fixed
//115) variable interval flag for lick 2s. 1==variable, 0==fixed
//116) light number for lick 1
//117) light number for lick 2
//118) laser on flag for CS1, 1==laser on, 0==laser off
//119) laser on flag for CS2, 1==laser on, 0==laser off
//120) laser on flag for CS3, 1==laser on, 0==laser off
//121) laser on flag for CS4, 1==laser on, 0==laser off
//122) fixed reward check for left lick tube (lick tube 1) for delay discounting task
//123) fixed reward check for right lick tube (lick tube 2) for delay discounting task
//124) reward laser check flag 1==laser, 0==no laser
//125) ramp max delay to CS1 for ramp timing task
//126) ramp max delay to CS2 for ramp timing task
//127) ramp max delay to CS3 for ramp timing task
//128) ramp max delay to CS4 for ramp timing task
//129) exponent factor for ramp function for CS1
//130) exponent factor for ramp function for CS2
//131) exponent factor for ramp function for CS3
//132) exponent factor for ramp function for CS4
//133) frequency increasing or decreasing for CS1
//134) frequency increasing or decreasing for CS2
//135) frequency increasing or decreasing for CS3
//136) frequency increasing or decreasing for CS4
//137) delay between first cue and second cue onset for CS1
//138) delay between first cue and second cue onset for CS2
//139) delay between first cue and second cue onset for CS3
//140) delay between first cue and second cue onset for CS4
//141) second cue type for CS1, 1=sound, 2=light, 0=no second cue
//142) second cue type for CS2, 1=sound, 2=light, 0=no second cue
//143) second cue type for CS3, 1=sound, 2=light, 0=no second cue
//144) second cue type for CS4, 1=sound, 2=light, 0=no second cue
//145) second cue frequency for CS1 if it's sound 
//146) second cue frequency for CS2 if it's sound 
//147) second cue frequency for CS3 if it's sound 
//148) second cue frequency for CS4 if it's sound 
//149) second cue speaker number for CS1 
//150) second cue speaker number for CS2
//151) second cue speaker number for CS3
//152) second cue speaker number for CS4
//153) second cue light number for CS1 
//154) second cue light number for CS2
//155) second cue light number for CS3
//156) second cue light number for CS4

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

unsigned long start;             // timestamp of start of session
unsigned long ts;                // current timestamp

boolean lickState[3];            // state of lickometer on all 3 licktubes
boolean licked;                  // new lick or not
boolean lickwithdrawn;           // was previous lick withdrawn or not?
boolean ITIflag;                 // are you currently in ITI? This needs to be true to give background solenoids
//boolean CSminusflag;             // is current trial a CS- trial?
boolean licktubesactive;         // signal to enter active lick tube state
boolean framestate;              // state of frame input
boolean frameon;                 // did frame input turn on?

const int numCS = 4;             // Number of different CSs
unsigned long numtrials[numCS];
unsigned long CSfreq[numCS];
unsigned long CSsolenoid[2 * numCS];
unsigned long CSprob[2 * numCS];
unsigned long CSopentime[2 * numCS];
unsigned long CSdur[numCS];
unsigned long CS_t_fxd[2 * numCS];
unsigned long CSpulse[numCS];
unsigned long CSspeaker[numCS];
unsigned long CSlight[numCS];
signed long golickreq[numCS];
int golicktube[numCS];
unsigned long CSsignal[numCS];
unsigned long meanITI;           // mean duration of ITI for the exponential distribution
unsigned long maxITI;            // maximum duration of ITI
unsigned long minITI;            // minimum duration of ITI
int intervaldistribution;              // 1, exponential iti; 2, uniform iti; 3, poisson cue
int backgroundsolenoid;
unsigned long T_bgd;             // inverse of the background rate of solenoids =1/lambda
unsigned long r_bgd;             // magnitude of background solenoid; in solenoid duration
unsigned long mindelaybgdtocue;  // minimum delay between background solenoid and the following cue
unsigned long mindelayfxdtobgd;  // minimum delay between fixed solenoid to the next background solenoid
unsigned long experimentmode;    // if==1, run experiment with cues; if==2, give only background solenoids; if==3, give lick dependent rewards
//boolean trialbytrialbgdsolenoidflag;  // if ==1, run experiment by changing bgd solenoid rate on a trial-by-trial basis
boolean isibgdsolenoidflag;       // if==1, background reward is given thoughout a trial including inter-stimulus interval (cue-reward delay)
boolean bgdsolenoidcueflag;       // if==1, background solenoid is preceded by a cue; the cue feature and delay to reward is hard-coded for now
unsigned long totbgdsolenoid;         // total number of background solenoids if experimentmode==2, i.e. when only Poisson solenoids are delivered.
unsigned long CSsolenoidcode[2 * numCS];
boolean rewardactive;
unsigned long maxdelaytosolenoid;
unsigned long cueonset;
float actualopentime;
unsigned long timeforfirstlick;
unsigned long CSrampmaxdelay[numCS];
float CSrampexp[numCS];
unsigned long CSincrease[numCS];
signed long delayforsecondcue[numCS];
boolean cueover;                  // indicator for cue to be over or not
unsigned long secondcue;          // for second cue in both cues task
unsigned long CSsecondcue[numCS];           // for immediate second cue type
unsigned long CSsecondcuefreq[numCS];       // for immediate second frequency
unsigned long CSsecondcuespeaker[numCS];    // speaker number for second cue
unsigned long CSsecondcuelight[numCS];      // light number for second cue

const int numlicktube = 2;       // number of recording lick tubes for lick dependent experiments
unsigned long reqlicknum[numlicktube];
unsigned long licksolenoid[numlicktube];
unsigned long variableratioflag[numlicktube];
unsigned long variableintervalflag[numlicktube];
unsigned long lickprob[numlicktube];
unsigned long lickopentime[numlicktube];
unsigned long delaytoreward[numlicktube];
unsigned long delaytolick[numlicktube];
unsigned long minrewards[numlicktube];
unsigned long signaltolickreq[numlicktube];
unsigned long soundsignalpulse[numlicktube];
unsigned long soundfreq[numlicktube];
unsigned long sounddur[numlicktube];
unsigned long lickspeaker[numlicktube];
unsigned long licklight[numlicktube];
float rewardprobforlick[numlicktube];
unsigned long fixedsidecheck[numlicktube];
int progressivemultiplier[numlicktube];

unsigned long laserlatency;      // Laser latency wrt cue (ms)
unsigned long laserduration;     // Laser duration (ms)
boolean randlaserflag;           // if ==1, session has laser turning on randomly for a duration equaling the longest delay between cue and fxd solenoid
unsigned long laserpulseperiod;  // The period for which laser is on in a cycle (ms)
unsigned long laserpulseoffperiod;// The period for which laser is off in a cycle (ms); If equal to laserpulseperiod, duty cycle is 50%
boolean lasertrialbytrialflag;   // if ==1, laser is turned on on a trial-by-trial basis
unsigned long maxdelaytovacuumfromcueonset; // maximum delay to vacuum after cue turns on. Change this if different cues have different delays to reward
// to be such that it is longer than the longest delay to reward. Basically, this quantity measures duration of trial.

unsigned long truncITI;          // truncation for the exponential ITI distribution: set at 3 times the meanITI or that hardcoded in maxITI

unsigned long ttloutdur      = 100;   // duration that the TTL out pin for starting imaging lasts. This happens only for the case where ITI is uniformly distributed
unsigned long baselinedur    = 7000;  // Duration prior to CS to turn on imaging through TTLOUTPIN. Only relevant when ITI is uniformly distributed
unsigned long vacuumopentime = 200;   // Duration to keep vacuum on
unsigned long lightdur       = 500;   // Duration to keep light (signal for lick requirement being met) on


int totalnumtrials = 0;
unsigned long rewardct[numlicktube];                   // number of rewards given for each lick tube in lick dependent experiment
int licktubethatmetlickreq;      // Lick tube that met the lick requirement

unsigned long nextcue;           // timestamp of next trial
unsigned long nextbgdsolenoid;   // timestamp of next background solenoid onset
unsigned long nextbgdcue;        // timestamp of next background solenoid cue
unsigned long nextfxdsolenoid;   // timestamp of next fixed solenoid onset
unsigned long nextvacuum;        // timestamp of next vacuum
unsigned long nextvacuumOff;     // timestamp of next vacuum off
unsigned long nextlaser;         // timestamp of next laser
unsigned long solenoidOff;       // timestamp to turn off solenoid
unsigned long cueOff;            // timestamp to turn off cues (after cue started)
unsigned long cuePulseOff;       // timestamp to pulse cue off (for CS-)
unsigned long cuePulseOn;        // timestamp to pulse cue on (for CS-)
unsigned long lightOff;          // timestamp to turn off light
unsigned long bgdcueOff;         // timestamp to turn off background solenoid cue

unsigned long nextttlouton;      // timestamp to turn on the TTL out pin for starting imaging
unsigned long nextttloutoff;     // timestamp to turn off the TTL out pin for starting imaging
unsigned long laserPulseOn;      // timestamp to turn on the laser on while pulsing
unsigned long laserPulseOff;     // timestamp to turn the laser off while pulsing
unsigned long laserOff;          // timestamp to turn the laser off
unsigned long CSlasercheck[numCS];    // flag for checking laser or not for each cue
unsigned long Rewardlasercheck;  // flag for checking laser for reward

unsigned long u;                 // uniform random number for inverse transform sampling to create an exponential distribution
unsigned long sessionendtime;    // the time at which session ends. Set to 5s after last fixed solenoid
float temp;                      // temporary float variable for temporary operations
float temp1;                     // temporary float variable for temporary operations
int temp2;
unsigned long tempu;
unsigned long tempITI;
unsigned long tempincrease;

int lickctforreq[3];            // number of licks on lick tubes 1, 2 and 3 during the cue-reward delay. If this is >= golickreq for the appropriate golicktube, animals get rewarded after the corresponding cue

int CSct;                        // number of cues delivered
int fxdrwct;                     // number of fixed reward delivered
int numbgdsolenoid;              // number of background solenoids delivered
int numfxdsolenoids;             // number of fixed solenoids delivered per cue till now. Useful since same cue can have two delayed solenoids

int *cueList = 0;                // Using dynamic allocation for defining the cueList. Be very very careful with memory allocation. All sorts of problems can come about if the program becomes too large. This is done just to be able to set #CSs from MATLAB
//int elements = 0;
unsigned long T_bgdvec[120];     // inverse of the background rate of solenoids for each trial. This assumes that if background solenoid changes on a trial-by-trial basis, there are a total of 120 trials
//unsigned long T_bgdvecnonzero[60]; // all the non-zero elements of the bgd vecs. Every other trial has zero background solenoid rate. This vector will be shuffled later
int *Laserontrial = 0;             // Is there laser on any given trial?
unsigned long *fxdrwtime = 0;             // save fixed reward times for poisson cue condition
unsigned long *cuetime = 0;        // save cue times for poisson reward condition

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

  for (int temp = 0; temp < numCS; temp++) {
    totalnumtrials += numtrials[temp];
  }

  reading = 0;

  //  The following block is for the case when you want to pulse the lower frequency stimulus
  //  if (CSplusfreq < CSminusfreq) {
  //    pulseCSplusorminus = 0;            // Pulse CSplus if CSplusfreq<CSminusfreq; here pulseCSplusorminus = 0;
  //  }
  //  else {
  //    pulseCSplusorminus = 1;            // Pulse CSminus if CSplusfreq>=CSminusfreq; here pulseCSplusorminus = 1;
  //  }

  // Key code sent from MATLAB;
  // = 48 for starting session, (0)
  // = 49 for END session, (1)
  // = 50 for testing CS1, (2)
  // = 51 for testing CS2, (3)
  // = 52 for testing CS3, (4)
  // = 56 for testing laser (8)
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
  // = 77 for turning lick retract solenoid 1 on for r_fxd duration, (M)
  // = 78 for turning lick retract solenoid 1 on, (N)
  // = 79 for turning lick retract solenoid 1 off, (O)
  // = 80 for turning lick retract solenoid 2 on for r_fxd duration, (P)
  // = 81 for turning lick retract solenoid 2 on, (Q)
  // = 82 for turning lick retract solenoid 2 off, (R)
  // = 86 for turning vacuum on for 200 ms duration, (V)

  while (reading != 48) {              // Before "Start" is pressed in MATLAB GUI
    reading = Serial.read();
    if (reading == 50 || reading == 51 || reading == 52) {                       // Test CS1 or CS2 or CS3
      reading -= 50;
      if (CSsignal[reading] == 1) {
        if (CSincrease[reading] == 1) {
          tone(speaker2, 6600);
          delay(200);
          noTone(speaker2);
          delay(5);
          tone(speaker2, 6800);
          delay(200);
          noTone(speaker2);
          delay(5);
          tone(speaker1, 7000);
          delay(200);
          noTone(speaker1);
          delay(5);
          tone(speaker1, 7200);
          delay(200);
          noTone(speaker1);
          delay(5);
        }
        else {
          if (CSpulse[reading] == 1) {
            tone(CSspeaker[reading], CSfreq[reading]);               // turn on tone
            delay(200);                               // Pulse with 200ms cycle
            noTone(CSspeaker[reading]);
            delay(200);
            tone(CSspeaker[reading], CSfreq[reading]);               // turn on tone
            delay(200);                               // Pulse with 200ms cycle
            noTone(CSspeaker[reading]);
            delay(200);
            tone(CSspeaker[reading], CSfreq[reading]);               // turn on tone
            delay(200);                               // Pulse with 200ms cycle
            noTone(CSspeaker[reading]);
          }
          else if (CSpulse[reading] == 0) {
            tone(CSspeaker[reading], CSfreq[reading]);               // turn on tone
            delay(1000);
            noTone(CSspeaker[reading]);
          }
        }
      }
      else if (CSsignal[reading] == 2) {
        if (CSpulse[reading] == 1) {
          digitalWrite(CSlight[reading], HIGH);               // turn on light
          delay(200);                               // Pulse with 200ms cycle
          digitalWrite(CSlight[reading], LOW);
          delay(200);
          digitalWrite(CSlight[reading], HIGH);               // turn on light
          delay(200);                               // Pulse with 200ms cycle
          digitalWrite(CSlight[reading], LOW);
          delay(200);
          digitalWrite(CSlight[reading], HIGH);               // turn on light
          delay(200);                               // Pulse with 200ms cycle
          digitalWrite(CSlight[reading], LOW);
        }
        else if (CSpulse[reading] == 0) {
          digitalWrite(CSlight[reading], HIGH);               // turn on light
          delay(1000);                               // delay 1s
          digitalWrite(CSlight[reading], LOW);
        }
      }
      else if (CSsignal[reading] == 3) {
        if (CSpulse[reading] == 1) {
          tone(CSspeaker[reading], CSfreq[reading]);               // turn on tone
          digitalWrite(CSlight[reading], HIGH);               // turn on light
          delay(200);                               // Pulse with 200ms cycle
          noTone(CSspeaker[reading]);
          digitalWrite(CSlight[reading], LOW);
          delay(200);
          tone(CSspeaker[reading], CSfreq[reading]);               // turn on tone
          digitalWrite(CSlight[reading], HIGH);               // turn on light
          delay(200);                               // Pulse with 200ms cycle
          noTone(CSspeaker[reading]);
          digitalWrite(CSlight[reading], LOW);
          delay(200);
          digitalWrite(CSlight[reading], HIGH);               // turn on light
          delay(200);                               // Pulse with 200ms cycle
          noTone(CSspeaker[reading]);
          digitalWrite(CSlight[reading], LOW);
        }
        else if (CSpulse[reading] == 0) {
          tone(CSspeaker[reading], CSfreq[reading]);               // turn on tone
          digitalWrite(CSlight[reading], HIGH);               // turn on light
          delay(1000);                               // delay 1s
          noTone(CSspeaker[reading]);
          digitalWrite(CSlight[reading], LOW);
        }
      }
    }
    if (reading == 65) {                 // MANUAL solenoid 1
      digitalWrite(solenoid1, HIGH);          // turn on solenoid 1
      delay(CSopentime[1]);
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
      delay(CSopentime[1]);
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
      delay(CSopentime[1]);
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
      delay(CSopentime[1]);
      digitalWrite(solenoid4, LOW);           // turn off solenoid 4
    }

    if (reading == 75) {                 // PRIME SOLENOID 4
      digitalWrite(solenoid4, HIGH);          // turn on solenoid 4
    }

    if (reading == 76) {                 // TURN OFF SOLENOID 4
      digitalWrite(solenoid4, LOW);           // turn off solenoid 4
    }

    if (reading == 77) {                 // MANUAL lickretractsolenoid11
      digitalWrite(lickretractsolenoid1, HIGH);          // turn on lickretractsolenoid1
      delay(CSopentime[1]);
      digitalWrite(lickretractsolenoid1, LOW);           // turn off lickretractsolenoid1
    }

    if (reading == 78) {                 // PRIME LICKRETRACTSOLENOID 1
      digitalWrite(lickretractsolenoid1, HIGH);          // turn on lickretractsolenoid1
    }

    if (reading == 79) {                 // TURN OFF LICKRETRACTSOLENOID 1
      digitalWrite(lickretractsolenoid1, LOW);           // turn off lickretractsolenoid1
    }

    if (reading == 80) {                 // MANUAL lickretractsolenoid12
      digitalWrite(lickretractsolenoid2, HIGH);          // turn on lickretractsolenoid2
      delay(CSopentime[1]);
      digitalWrite(lickretractsolenoid2, LOW);           // turn off lickretractsolenoid2
    }

    if (reading == 81) {                 // PRIME LICKRETRACTSOLENOID 2
      digitalWrite(lickretractsolenoid2, HIGH);          // turn on lickretractsolenoid2
    }

    if (reading == 82) {                 // TURN OFF LICKRETRACTSOLENOID 2
      digitalWrite(lickretractsolenoid2, LOW);           // turn off lickretractsolenoid2
    }

    if (reading == 86) {                 // Vacuum
      digitalWrite(vacuum, HIGH);          // turn on vacuum
      delay(vacuumopentime);
      digitalWrite(vacuum, LOW);           // turn off vacuum
    }

    if (reading == 56) {                 // TEST LASER
      digitalWrite(laser, HIGH);         // turn on LASER
      delay(5000);
      digitalWrite(laser, LOW);         // turn off LASER
    }

  }
  // initialize T_bgdvec to the non-zero background solenoid rates for trials
  int r;
//  if (trialbytrialbgdsolenoidflag == 1) {
//    for (int a = 0; a < 120; a++) {
//      if (a < 22) {
//        T_bgdvec[a] = 6000;
//      }
//      else if (a < 44) {
//        T_bgdvec[a] = 12000;
//      }
//      else if (a < 66) {
//        T_bgdvec[a] = 15000;
//      }
//      else if (a < 88) {
//        T_bgdvec[a] = 18000;
//      }
//      else if (a < 120) {
//        T_bgdvec[a] = 0;
//      }
//    }
//    //shuffle T_bgdvec
//    for (int a = 0; a < 120; a++)
//    {
//      r = random(a, 120);
//      int temp = T_bgdvec[a];
//      T_bgdvec[a] = T_bgdvec[r];
//      T_bgdvec[r] = temp;
//    }
//  }
  //initialize cueList
  cueList = new int[totalnumtrials];
  if (lasertrialbytrialflag == 1) {
    //initialize cueList
    Laserontrial = new int[totalnumtrials];
  }
  //r = 0;
  for (int a = 0; a < totalnumtrials; a++) {
    if (a < numtrials[0]) {
      cueList[a] = 0;
      if (lasertrialbytrialflag == 1) {
        if (a < 0.8 * numtrials[0]) {
          Laserontrial[a] = 1;
        }
        else {
          Laserontrial[a] = 0;
        }
      }
    }
    else if (a < numtrials[0] + numtrials[1]) {
      cueList[a] = 1;
      if (lasertrialbytrialflag == 1) {
        if (a < numtrials[0] + 0.8 * numtrials[1]) {
          Laserontrial[a] = 1;
        }
        else {
          Laserontrial[a] = 0;
        }
      }
    }
    else if (a < numtrials[0] + numtrials[1] + numtrials[2]) {
      cueList[a] = 2;
      if (lasertrialbytrialflag == 1) {
        if (a < numtrials[0] + numtrials[1] + 0.8 * numtrials[2]) {
          Laserontrial[a] = 1;
        }
        else {
          Laserontrial[a] = 0;
        }
      }
    }
    else {
      cueList[a] = 3;
      if (lasertrialbytrialflag == 1) {
        if (a < numtrials[0] + numtrials[1] + 0.8 * numtrials[2] + 0.8 * numtrials[3]) {
          Laserontrial[a] = 1;
        }
        else {
          Laserontrial[a] = 0;
        }
      }
    }
  }
  //shuffle cueList
  for (int a = 0; a < totalnumtrials; a++)
  {
    r = random(a, totalnumtrials);
    int temp = cueList[a];
    cueList[a] = cueList[r];
    cueList[r] = temp;
    if (lasertrialbytrialflag == 1) {
      int temp1 = Laserontrial[a];
      Laserontrial[a] = Laserontrial[r];
      Laserontrial[r] = temp1;
    }
  }

  truncITI = min(3 * meanITI, maxITI); //truncation is set at 3 times the meanITI or that hardcoded in maxITI; used for exponential distribution
  if (meanITI == maxITI) {
    nextcue = meanITI;
  }
  else {
    if (intervaldistribution == 2) { // generate uniform random numbers for itis
      u = random(0, 10000);
      temp = (float)u / 10000;
      tempu = (unsigned long)(maxITI - minITI) * temp;
      nextcue    = minITI + tempu; // set timestamp of first cue
    }
    else { // generate exponential random numbers for itis
      if (intervaldistribution != 5) {
        tempITI = 0;
        while (tempITI <= minITI) {
          u = random(0, 10000);
          temp = (float)u / 10000;
          temp1 = (float)truncITI / meanITI;
          temp1 = exp(-temp1);
          temp1 = 1 - temp1;
          temp = temp * temp1;
          temp = -log(1 - temp);
          tempITI = (unsigned long)mindelaybgdtocue + meanITI * temp;
        }
        nextcue  = tempITI; // set timestamp of first cue
      }
      else {
        nextcue = 0;
      }

      tempITI = 0;
      if (intervaldistribution > 3) {
        while (tempITI <= minITI) {
          u = random(0, 10000);
          temp = (float)u / 10000;
          temp1 = (float)truncITI / meanITI;
          temp1 = exp(-temp1);
          temp1 = 1 - temp1;
          temp = temp * temp1;
          temp = -log(1 - temp);
          tempITI = (unsigned long)mindelaybgdtocue + meanITI * temp;
        }
        nextfxdsolenoid  = tempITI; // set timestamp of first poisson reward
      }
    }
  }

  if (randlaserflag == 1) {
    temp = nextcue - mindelaybgdtocue;
    nextlaser = random(0, temp);
  }

  nextbgdcue = 0;
  if (r_bgd>0) {
    u = random(0, 10000);
    temp = (float)u / 10000;
    temp = log(temp);
    nextbgdsolenoid = 0 - T_bgd * temp;
    while (nextbgdsolenoid > (nextcue - mindelaybgdtocue) && experimentmode != 1 && !isibgdsolenoidflag) {
      u = random(0, 10000);
      temp = (float)u / 10000;
      temp = log(temp);
      nextbgdsolenoid = 0 - T_bgd * temp;
    }
    if (bgdsolenoidcueflag) {
      nextbgdcue = nextbgdsolenoid-1000; // a cue is given 1000ms before a background reward
    }
  }
  else {
    nextbgdsolenoid = 0;
  }

  if (intervaldistribution == 3) {
    fxdrwtime = new unsigned long[totalnumtrials];
  }
  if (intervaldistribution == 5) {
    cuetime = new unsigned long[totalnumtrials];
  }

  bgdcueOff = 0;
  cueOff     = nextcue + CSdur[cueList[0]];           // get timestamp of first cue cessation
  ITIflag = true;
  solenoidOff = 0;
  licktubesactive = true;
  lightOff = 0;
  cueover = false;    // set cueover to be false initially
  secondcue = 0;      // second cue time to be zero

  CSct = 0;                            // Number of CSs is initialized to 0
  fxdrwct = 0;                         // Number of initial fixed rewards is initialized to 0
  rewardct[0] = 0;                        // Number of initial rewards for lick tube 1 is initialized to 0
  rewardct[1] = 0;                        // Number of initial rewards for lick tube 2 is initialized to 0
  numfxdsolenoids = 0;                      // Number of fixed solenoids initialized to 0
  numbgdsolenoid = 0;                       // Number of background solenoids initialized to 0
  sessionendtime = 0;
  lickctforreq[0] = 0;                 // Number of licks1 during cue for first trial is initialized to 0
  lickctforreq[1] = 0;                 // Number of licks2 during cue for first trial is initialized to 0
  lickctforreq[2] = 0;                 // Number of licks3 during cue for first trial is initialized to 0

  nextvacuum = 0;
  nextvacuumOff = 0;

  // UNCOMMENT THESE LINES FOR TRIGGERING IMAGE COLLECTION AT BEGINNING
  //digitalWrite(ttloutpin, HIGH);
  //delay(100);
  //digitalWrite(ttloutpin, LOW);
  // TILL HERE


  // UNCOMMENT THESE LINES FOR TRIGGERING PHOTOMETRY IMAGE COLLECTION AT BEGINNING
  digitalWrite(ttloutpin, HIGH);
  // TILL HERE

  // start session
  start = millis();                    // start time
  nextttlouton = 0;
  nextttloutoff = 0;
}

// LOOP code ////////////////
void loop() {
  ts = millis() - start;               // find time since start
  reading = Serial.read();             // look for signals from MATLAB

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
  // 21 = light1
  // 22 = light2
  // 23 = light 3
  // 25 = both CSsound1 and CSlight1
  // 26 = both CSsound2 and CSlight2
  // 27 = both CSsound3 and CSlight3
  // 30 = frame
  // 31 = laser

  if (CSct >= totalnumtrials && sessionendtime == 0 && intervaldistribution < 3) {
    sessionendtime = ts + 5000;   // end session 5 seconds after the fixed solenoid is given (or would've been for CS-) so as to store licks occuring during this time
  }

  if ((ts >= sessionendtime && sessionendtime != 0) || reading == 49) {    // END SESSION
    endSession();                      // end
  }

  licking();                           // determine if lick occured or was withdrawn
  frametimestamp();                    // store timestamps of frames

  if (ts >= nextbgdcue && nextbgdcue !=0) { // everything about background cue is hardcoded for now - 0.25s 3khz tone at speaker 2
    Serial.print(19);
    Serial.print(" ");
    Serial.print(ts);
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
    tone(speaker2,3000);
    digitalWrite(ttloutstoppin, HIGH);
    bgdcueOff = nextbgdcue + 250; // backgrund cue is 250ms long
    nextbgdcue = 0;
  }

  if (ts >= bgdcueOff && bgdcueOff !=0) {
    noTone(speaker2);
    digitalWrite(ttloutstoppin, LOW);
    bgdcueOff = 0;
  }

  if (ts >= nextcue && ((ITIflag && intervaldistribution < 3) || intervaldistribution > 2) && nextcue != 0) {
    if (secondcue == 0) {
      if (CSsignal[cueList[CSct]] == 1) {
        Serial.print(15 + cueList[CSct]);         // code data as CS1, CS2 or CS3 timestamp
        Serial.print(" ");
        Serial.print(ts);                         // send timestamp of cue
        Serial.print(" ");
        Serial.print(0);
        Serial.print('\n');
        cues();
      }
      else if (CSsignal[cueList[CSct]] == 2) {
        Serial.print(21 + cueList[CSct]);
        Serial.print(" ");
        Serial.print(ts);                         // send timestamp of light cue
        Serial.print(" ");
        Serial.print(0);                          // indicates second cue
        Serial.print('\n');
        lights();
      }
      if (CSsecondcue[cueList[CSct]] != 0) {
        nextcue = ts + delayforsecondcue[cueList[CSct]];
        secondcue = 1;
      }
      else {
        cueover = true;
        nextcue = 0;
      }

      if (intervaldistribution < 3) {
        nextfxdsolenoid = ts + CS_t_fxd[2 * cueList[CSct]];    // next fixed solenoid comes at a fixed delay following cue onset
        numfxdsolenoids = 0;
      }
      else if (intervaldistribution == 3) {
        fxdrwtime[CSct] = ts + CS_t_fxd[2 * cueList[CSct] + 1];
        if (nextfxdsolenoid == 0) {
          nextfxdsolenoid = fxdrwtime[fxdrwct];
        }
      }
    }
    else if (secondcue == 1) {
      if (CSsecondcue[cueList[CSct]] == 1) {               // if second cue is sound cue, deliver sound
        Serial.print(15 + cueList[CSct]);
        Serial.print(" ");
        Serial.print(ts);                         // send timestamp of sound cue
        Serial.print(" ");
        Serial.print(1);                          // indicates second cue
        Serial.print('\n');
        cues();
      }
      else if (CSsecondcue[cueList[CSct]] == 2) {          // if second cue is light cue, deliver light
        Serial.print(21 + cueList[CSct]);
        Serial.print(" ");
        Serial.print(ts);                         // send timestamp of light cue
        Serial.print(" ");
        Serial.print(1);                          // indicates second cue
        Serial.print('\n');
        lights();
      }
      secondcue = 0;
      nextcue = 0;
      cueover = true;
    }

    if (CSlasercheck[cueList[CSct]]) {
      deliverlasertocues();              // check whether to and deliver laser if needed
    }

    if (intervaldistribution > 2 && cueover) {
      CSct++;
      if (intervaldistribution < 5) {
        if (CSct >= totalnumtrials && intervaldistribution > 3) {
          nextfxdsolenoid = 0;
          nextcue = 0;
          sessionendtime = ts + 5000;
        }
        u = random(0, 10000);
        temp = (float)u / 10000;
        temp1 = (float)truncITI / meanITI;
        temp1 = exp(-temp1);
        temp1 = 1 - temp1;
        temp = temp * temp1;
        temp = -log(1 - temp);
        nextcue =  (unsigned long)ts + meanITI * temp;
      }
      else {
        if (CSct > totalnumtrials) {
          nextcue = 0;
          nextfxdsolenoid = 0;
          sessionendtime = ts + 5000;
        }
        else {
          if (CSct < fxdrwct) {
            nextcue = cuetime[CSct];
          }
          else {
            nextcue = 0;
          }
        }
      }
    }
    else if (cueover) {
      ITIflag = false;
      cueover = false;
    }
  }

  // UNCOMMENT THESE LINES FOR TRIGGERING COLLECTION TRIAL-BY-TRIAL

  //    if (ts >= nextttlouton && nextttlouton != 0) {
  //      digitalWrite(ttloutpin, HIGH);
  //      nextttloutoff = ts + ttloutdur;
  //      nextttlouton = 0;
  //    }
  //
  //    if (ts >= nextttloutoff && nextttloutoff != 0) {
  //      digitalWrite(ttloutpin, LOW);
  //      nextttloutoff = 0;
  //    }

  // TILL HERE

  // Pulse cue
  if (ts >= cuePulseOff && cuePulseOff != 0 && ts < cueOff) {
    noTone(speaker1);                   // turn off tone
    noTone(speaker2);
    if (CSincrease[cueList[CSct]] == 0) {
      cuePulseOn = ts + 200;
    }
    else {
      cuePulseOn = ts + 5;
    }
    cuePulseOff = 0;
  }

  if (ts >= cuePulseOn && cuePulseOn != 0 && ts < cueOff && CSincrease[cueList[CSct]] == 0) {
    tone(CSspeaker[cueList[CSct]], CSfreq[cueList[CSct]]);               // turn on tone
    cuePulseOff = ts + 200;                  // Cue pulsing
    cuePulseOn = 0;                          // No cue pulsing
  }

  // Increasing cue
  if (ts >= cuePulseOn && cuePulseOn != 0 && ts < cueOff && CSincrease[cueList[CSct]] == 1) {
    if (CSfreq[cueList[CSct]] + tempincrease < 7000) {
      tone(speaker2, CSfreq[cueList[CSct]] + tempincrease);             // turn on tone
    }
    else {
      tone(speaker1, CSfreq[cueList[CSct]] - tempincrease);             // turn on tone
    }
    tempincrease = tempincrease + 80;
    cuePulseOff = ts + 200;                  // Cue pulsing
    cuePulseOn = 0;                          // No cue pulsing
  }


  // Pulse LASER
  if (ts >= nextlaser && nextlaser != 0) {
    Serial.print(31);                        // code data as laser timestamp
    Serial.print(" ");
    Serial.print(ts);                        // send timestamp of laser
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
    digitalWrite(laser, HIGH);
    laserPulseOff = ts + laserpulseperiod;
    laserOff = ts + laserduration;
    nextlaser = 0;
  }

  if (ts >= laserPulseOff && laserPulseOff != 0 && ts < laserOff) {
    digitalWrite(laser, LOW);                   // turn off laser
    laserPulseOn = ts + laserpulseoffperiod;
    laserPulseOff = 0;
  }

  if (ts >= laserPulseOn && laserPulseOn != 0 && ts < laserOff) {
    digitalWrite(laser, HIGH);                   // turn on laser
    laserPulseOn = 0;
    laserPulseOff = ts + laserpulseperiod;
  }

  // Turn off cue
  if (ts >= cueOff && cueOff != 0) {   // CUE CESSATION
    noTone(speaker1);                   // turn off cue
    noTone(speaker2);
    cueOff = 0;
    cuePulseOff = 0;
    cuePulseOn = 0;
    tempincrease = 0;
    // Sync with fiber photometry
    digitalWrite(ttloutstoppin, LOW);
  }
  // Turn off laser
  if (ts >= laserOff && laserOff != 0) {   // LASER CESSATION
    digitalWrite(laser, LOW);              // turn off laser
    laserOff = 0;
  }
  // Turn off light
  if (ts >= lightOff && lightOff != 0) {    // LIGHT CESSATION
    digitalWrite(light1, LOW);              // turn off light
    digitalWrite(light2, LOW);
    lightOff = 0;
  }

  if (((!ITIflag && intervaldistribution < 3) || intervaldistribution > 2) && ts >= nextfxdsolenoid && nextfxdsolenoid != 0) { // give fixed solenoid
    if (intervaldistribution < 3) {
      Serial.print(CSsolenoidcode[2 * cueList[CSct] + numfxdsolenoids]);
    }
    else if (intervaldistribution > 2) {
      Serial.print(CSsolenoidcode[2 * cueList[fxdrwct] + 1]);
    }
    Serial.print(" ");
    Serial.print(ts);                      //   send timestamp of solenoid onset
    Serial.print(" ");

    

    if (intervaldistribution < 3) {
      u = random(0,100); // if a given trial is determined as a rewarded trial, both first and and second rewards will be delivered in the trial 
      if (numfxdsolenoids == 1) {
        temp2 = golickreq[cueList[CSct]];     // Go lick requirement only applies to the second reward per cue
      }
      else {
        temp2 = 0;
      }
      if (CSsolenoidcode[2 * cueList[CSct] + numfxdsolenoids] == 35 && CSopentime[2 * cueList[CSct] + numfxdsolenoids] > 0 && u < CSprob[2 * cueList[CSct] + numfxdsolenoids]) {
        digitalWrite(lickretractsolenoid1, HIGH);
        digitalWrite(lickretractsolenoid2, HIGH);
        Serial.print(0);
        Serial.print('\n');
      }
      else if (CSopentime[2 * cueList[CSct] + numfxdsolenoids] > 0 && u < CSprob[2 * cueList[CSct] + numfxdsolenoids] && lickctforreq[golicktube[cueList[CSct]]] >= temp2 && temp2 != -1) {
        digitalWrite(CSsolenoid[2 * cueList[CSct] + numfxdsolenoids], HIGH);      // turn on solenoid
        Serial.print(0);                       //   this indicates that the solenoid was actually given
        Serial.print('\n');
        if (Rewardlasercheck) {
          delieverlasertorewards();              // check whether to and deliver laser if needed
          }
      }
      else if (CSopentime[2 * cueList[CSct] + numfxdsolenoids] > 0 && lickctforreq[golicktube[cueList[CSct]]] == 0 && temp2 == -1) {  // -1 on golickreq indicates no-go cue, no licks on the lick tube gives a reward
        digitalWrite(CSsolenoid[2 * cueList[CSct] + numfxdsolenoids], HIGH);      // turn on solenoid
        Serial.print(0);                       //   this indicates that the solenoid was actually given
        Serial.print('\n');
        if (Rewardlasercheck) {
          delieverlasertorewards();              // check whether to and deliver laser if needed
          }
      }
      else {
        Serial.print(1);                       //   this indicates that the solenoid was not given
        Serial.print('\n');
        // Serial.print(CSopentime[2 * cueList[CSct] + numfxdsolenoids]);
        // Serial.print('\n');
        // Serial.print(u);
        // Serial.print('\n');
        // Serial.print(CSprob[2 * cueList[CSct] + numfxdsolenoids]);
        // Serial.print('\n');
        // Serial.print(lickctforreq[golicktube[cueList[CSct]]]);
        // Serial.print('\n');
        // Serial.print(temp2);
        // Serial.print('\n');
      }
      solenoidOff = ts + CSopentime[2 * cueList[CSct] + numfxdsolenoids];      // set solenoid off time
      numfxdsolenoids++;                                     // Increase fixed solenoids given till now for this cue

      if (numfxdsolenoids == 1) { //set time for next fixed solenoid
        nextfxdsolenoid = ts + CS_t_fxd[2 * cueList[CSct] + numfxdsolenoids] - CS_t_fxd[2 * cueList[CSct]];  
      }
      else {
        nextfxdsolenoid = 0;
        nextvacuum = ts + CSopentime[2 * cueList[CSct] + 1] + maxdelaytovacuumfromcueonset - CS_t_fxd[2 * cueList[CSct] + 1];
      }
    }
    else if (intervaldistribution == 3) {
      digitalWrite(CSsolenoid[2 * cueList[fxdrwct] + 1], HIGH);      // turn on solenoid
      Serial.print(0);                       //   this indicates that the solenoid was actually given
      Serial.print('\n');
      
      if (Rewardlasercheck) {
        delieverlasertorewards();              // check whether to and deliver laser if needed
        }
          
      solenoidOff = ts + CSopentime[2 * cueList[fxdrwct] + 1];      // set solenoid off time
      fxdrwtime[fxdrwct] = 0;
      fxdrwct++;
      if (fxdrwct >= totalnumtrials) {
        nextfxdsolenoid = 0;
        sessionendtime = ts + 5000;
      }
      else {
        if (fxdrwct < CSct) {
          nextfxdsolenoid = fxdrwtime[fxdrwct];
        }
        else {
          nextfxdsolenoid = 0;
        }
      }
    }
    else if (intervaldistribution > 3) {
      digitalWrite(CSsolenoid[2 * cueList[fxdrwct] + 1], HIGH);      // turn on solenoid
      Serial.print(0);                       //   this indicates that the solenoid was actually given
      Serial.print('\n');
      if (Rewardlasercheck) {
        delieverlasertorewards();              // check whether to and deliver laser if needed
        }
        
      solenoidOff = ts + CSopentime[2 * cueList[fxdrwct] + 1];      // set solenoid off time
      if (intervaldistribution == 5) {
        cuetime[fxdrwct] = nextfxdsolenoid + CS_t_fxd[2 * cueList[CSct] + 1];
        if (nextcue == 0) {
          nextcue = cuetime[CSct];
        }
      }
      fxdrwct++;

      if (fxdrwct >= totalnumtrials && intervaldistribution == 4) {
        nextfxdsolenoid = 0;
        sessionendtime = ts + 5000;
      }
      else {
        tempITI = 0;
        while (tempITI <= minITI) {
          u = random(0, 10000);
          temp = (float)u / 10000;
          temp1 = (float)truncITI / meanITI;
          temp1 = exp(-temp1);
          temp1 = 1 - temp1;
          temp = temp * temp1;
          temp = -log(1 - temp);
          tempITI = (unsigned long)meanITI * temp;
        }
        nextfxdsolenoid  = ts + tempITI; // set timestamp of first poisson reward
      }
    }
  }

  if (ts >= nextbgdsolenoid && nextbgdsolenoid != 0 && r_bgd != 0) { // give background solenoid 
    if ((ITIflag && !isibgdsolenoidflag) || isibgdsolenoidflag){
      digitalWrite(backgroundsolenoid, HIGH);          // turn on solenoid
      solenoidOff = ts + r_bgd;              // set solenoid off time
      Serial.print(7);                       //   code data as background solenoid onset timestamp
      Serial.print(" ");
      Serial.print(ts);                      //   send timestamp of solenoid onset
      Serial.print(" ");
      Serial.print(0);
      Serial.print('\n');
    }

    u = random(0, 10000);
    temp = (float)u / 10000;
    temp = log(temp);
    nextbgdsolenoid = ts + r_bgd - T_bgd * temp;// next background solenoid can't be earlier than the offset of the solenoid
    if (nextbgdsolenoid > (nextcue - mindelaybgdtocue) && !isibgdsolenoidflag) {
      nextbgdsolenoid = 0;
    }
    if (bgdsolenoidcueflag) {
      nextbgdcue = nextbgdsolenoid - 500;
    }
  }

  if (reading == 65) {                 // MANUAL solenoid 1
    digitalWrite(solenoid1, HIGH);          // turn on solenoid
    solenoidOff = ts + CSopentime[1];              // set solenoid off time
    Serial.print(8);                   //   code data as solenoid1 onset timestamp
    Serial.print(" ");
    Serial.print(ts);                  //   send timestamp of solenoid onset
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
  }
  if (reading == 68) {                 // MANUAL solenoid 2
    digitalWrite(solenoid2, HIGH);          // turn on solenoid
    solenoidOff = ts + CSopentime[1];              // set solenoid off time
    Serial.print(9);                   //   code data as solenoid2 onset timestamp
    Serial.print(" ");
    Serial.print(ts);                  //   send timestamp of solenoid onset
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
  }
  if (reading == 71) {                 // MANUAL solenoid 3
    digitalWrite(solenoid3, HIGH);          // turn on solenoid
    solenoidOff = ts + CSopentime[1];              // set solenoid off time
    Serial.print(10);                   //   code data as solenoid3 onset timestamp
    Serial.print(" ");
    Serial.print(ts);                  //   send timestamp of solenoid onset
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
  }
  if (reading == 74) {                 // MANUAL solenoid 4
    digitalWrite(solenoid4, HIGH);          // turn on solenoid
    solenoidOff = ts + CSopentime[1];              // set solenoid off time
    Serial.print(11);                   //   code data as solenoid4 onset timestamp
    Serial.print(" ");
    Serial.print(ts);                  //   send timestamp of solenoid onset
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
  }
  if (reading == 86) {                 // Vacuum
    digitalWrite(vacuum, HIGH);          // turn on vacuum
    nextvacuumOff = ts + vacuumopentime;              // set vacuum off time
    Serial.print(14);                   //   code data as vacuum onset timestamp
    Serial.print(" ");
    Serial.print(ts);                  //   send timestamp of vacuum onset
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
  }


  if (ts >= solenoidOff && solenoidOff != 0) { // solenoid CESSATION
    digitalWrite(solenoid1, LOW);           // turn off solenoid
    digitalWrite(solenoid2, LOW);           // turn off solenoid
    digitalWrite(solenoid3, LOW);           // turn off solenoid
    digitalWrite(solenoid4, LOW);           // turn off solenoid
    digitalWrite(lickretractsolenoid1, LOW);
    digitalWrite(lickretractsolenoid2, LOW);
    solenoidOff = 0;
  }

  if (ts >= nextvacuum && nextvacuum != 0) { // vacuum onset
    digitalWrite(vacuum, HIGH);           // turn on vacuum
    Serial.print(14);                      //   code data as vacuum onset timestamp
    Serial.print(" ");
    Serial.print(ts);                     //   send timestamp of vacuum onset
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
    nextvacuumOff = ts + vacuumopentime;
    nextvacuum = 0;
  }

  if (ts >= nextvacuumOff && nextvacuumOff != 0) { // vacuum offset
    digitalWrite(vacuum, LOW);           // turn off vacuum
    nextvacuumOff = 0;

    if (ITIflag == false) {            // if exit was from a trial, move into ITI and set the next cue and bgdsolenoid times
      ITIflag = true;
      if (meanITI == maxITI) {
        nextcue = ts + meanITI;
      }
      else {
        if (intervaldistribution == 1 || intervaldistribution == 3) {
          tempITI = 0;
          while (tempITI <= minITI) {
            u = random(0, 10000);
            temp = (float)u / 10000;
            temp1 = (float)truncITI / meanITI;
            temp1 = exp(-temp1);
            temp1 = 1 - temp1;
            temp = temp * temp1;
            temp = -log(1 - temp);
            tempITI =  (unsigned long)mindelaybgdtocue + meanITI * temp;
          }
          nextcue    = (unsigned long)ts + tempITI; // set timestamp of next cue
        }
        else if (intervaldistribution == 2) {
          u = random(0, 10000);
          temp = (float)u / 10000;
          tempu = (unsigned long)(maxITI - minITI) * temp;
          nextcue = ts + minITI + tempu; // set timestamp of first cue
        }
      }

      if (randlaserflag == 1) {
        temp = nextcue - mindelaybgdtocue;
        nextlaser = random(ts, temp);
      }

      CSct++;                            // count total number of CSs
      lickctforreq[0] = 0;                 // reset lick1 count to zero at end of trial
      lickctforreq[1] = 0;                 // reset lick2 count to zero at end of trial
      lickctforreq[2] = 0;                 // reset lick3 count to zero at end of trial


      if (!isibgdsolenoidflag){
        u = random(0, 10000);
        temp = (float)u / 10000;
        temp = log(temp);
        nextbgdsolenoid = ts + r_bgd - T_bgd * temp;// next background solenoid can't be earlier than the offset of the solenoid
        if (nextbgdsolenoid > (nextcue - mindelaybgdtocue)) {
          nextbgdsolenoid = 0;
          }
      }
    }
  }
}


// Accept parameters from MATLAB
void getParams() {
  int pn = 159;                              // number of parameter inputs
  unsigned long param[pn];                  // parameters

  for (int p = 0; p < pn; p++) {
    reading = Serial.parseInt();           // read parameter
    param[p] = reading;                    // convert to int
  }
  reading = 0;

  numtrials[0]           = param[0];
  numtrials[1]           = param[1];
  numtrials[2]           = param[2];
  numtrials[3]           = param[3];
  CSfreq[0]              = param[4];
  CSfreq[1]              = param[5];
  CSfreq[2]              = param[6];
  CSfreq[3]              = param[7];
  for (int p = 0; p < 2 * numCS; p++) {
    CSsolenoid[p]        = param[8 + p];
  }
  for (int p = 0; p < 2 * numCS; p++) {
    CSprob[p]            = param[16 + p];
  }
  for (int p = 0; p < 2 * numCS; p++) {
    CSopentime[p]        = param[24 + p];
  }
  CSdur[0]               = param[32];
  CSdur[1]               = param[33];
  CSdur[2]               = param[34];
  CSdur[3]               = param[35];
  for (int p = 0; p < 2 * numCS; p++) {
    CS_t_fxd[p]          = param[36 + p];
  }
  CSpulse[0]             = param[44];
  CSpulse[1]             = param[45];
  CSpulse[2]             = param[46];
  CSpulse[3]             = param[47];
  CSspeaker[0]           = param[48];
  CSspeaker[1]           = param[49];
  CSspeaker[2]           = param[50];
  CSspeaker[3]           = param[51];
  golickreq[0]           = param[52];
  golickreq[1]           = param[53];
  golickreq[2]           = param[54];
  golickreq[3]           = param[55];
  golicktube[0]          = param[56];
  golicktube[1]          = param[57];
  golicktube[2]          = param[58];
  golicktube[3]          = param[59];
  CSsignal[0]            = param[60];
  CSsignal[1]            = param[61];
  CSsignal[2]            = param[62];
  CSsignal[3]            = param[63];
  meanITI                = param[64];                   // get meanITI, in ms
  maxITI                 = param[65];                   // get maxITI, in ms
  minITI                 = param[66];
  intervaldistribution   = (int)param[67];
  backgroundsolenoid     = (int)param[68];
  T_bgd                  = param[69];                   // get T=1/lambda, in ms
  r_bgd                  = param[70];                   // get r_bgd, ms open time for the solenoid
  mindelaybgdtocue       = param[71];                   // get minimum delay between a background solenoid and the next cue, in ms
  mindelayfxdtobgd       = param[72];                   // get minimum delay between a fixed solenoid and the next background solenoid, in ms
  experimentmode         = param[73];
  isibgdsolenoidflag = (boolean)param[74];
  totbgdsolenoid         = param[75];                   // total number of background solenoids to stop the session if the session just has Poisson solenoids, i.e. experimentmode==1
  reqlicknum[0]          = param[76];
  reqlicknum[1]          = param[77];
  licksolenoid[0]        = param[78];
  licksolenoid[1]        = param[79];
  lickprob[0]            = param[80];
  lickprob[1]            = param[81];
  lickopentime[0]        = param[82];
  lickopentime[1]        = param[83];
  delaytoreward[0]       = param[84];
  delaytoreward[1]       = param[85];
  delaytolick[0]         = param[86];
  delaytolick[1]         = param[87];
  minrewards[0]          = param[88];
  minrewards[1]          = param[89];
  signaltolickreq[0]     = param[90];
  signaltolickreq[1]     = param[91];
  soundsignalpulse[0]    = param[92];
  soundsignalpulse[1]    = param[93];
  soundfreq[0]           = param[94];
  soundfreq[1]           = param[95];
  sounddur[0]            = param[96];
  sounddur[1]            = param[97];
  lickspeaker[0]        = param[98];
  lickspeaker[1]        = param[99];
  laserlatency           = param[100];
  laserduration          = param[101];
  randlaserflag          = (boolean)param[102];          // Random laser flag
  laserpulseperiod       = param[103];
  laserpulseoffperiod    = param[104];
  lasertrialbytrialflag  = (boolean)param[105];          // laser on a trial-by-trial basis?
  maxdelaytovacuumfromcueonset = param[106];
  CSlight[0]             = param[107];
  CSlight[1]             = param[108];
  CSlight[2]             = param[109];
  CSlight[3]             = param[110];
  variableratioflag[0]      = param[111];
  variableratioflag[1]      = param[112];
  variableintervalflag[0]   = param[113];
  variableintervalflag[1]   = param[114];
  licklight[0]           = param[115];
  licklight[1]           = param[116];
  CSlasercheck[0]         = param[117];
  CSlasercheck[1]         = param[118];
  CSlasercheck[2]         = param[119];
  CSlasercheck[3]         = param[120];
  fixedsidecheck[0]      = param[121];
  fixedsidecheck[1]      = param[122];
  Rewardlasercheck       = param[123];
  CSrampmaxdelay[0]      = param[124];
  CSrampmaxdelay[1]      = param[125];
  CSrampmaxdelay[2]      = param[126];
  CSrampmaxdelay[3]      = param[127];
  CSrampexp[0]           = param[128];
  CSrampexp[1]           = param[129];
  CSrampexp[2]           = param[130];
  CSrampexp[3]           = param[131];
  CSincrease[0]          = param[132];
  CSincrease[1]          = param[133];
  CSincrease[2]          = param[134];
  CSincrease[3]          = param[135];
  delayforsecondcue[0]   = param[136];        // delay between sound cue and light cue if both present
  delayforsecondcue[1]   = param[137];
  delayforsecondcue[2]   = param[138];
  delayforsecondcue[3]   = param[139];
  CSsecondcue[0]             = param[140];
  CSsecondcue[1]             = param[141];
  CSsecondcue[2]             = param[142];
  CSsecondcue[3]             = param[143];
  CSsecondcuefreq[0]         = param[144];
  CSsecondcuefreq[1]         = param[145];
  CSsecondcuefreq[2]         = param[146];
  CSsecondcuefreq[3]         = param[147];
  CSsecondcuespeaker[0]          = param[148];
  CSsecondcuespeaker[1]          = param[149];
  CSsecondcuespeaker[2]          = param[150];
  CSsecondcuespeaker[3]          = param[151];
  CSsecondcuelight[0]            = param[152];
  CSsecondcuelight[1]            = param[153];
  CSsecondcuelight[2]            = param[154];
  CSsecondcuelight[3]            = param[155];
  progressivemultiplier[0]       = param[156];
  progressivemultiplier[1]       = param[157];
  bgdsolenoidcueflag             = (boolean)param[158];

  for (int p = 0; p < numCS; p++) {
    CSfreq[p] = CSfreq[p] * 1000;         // convert frequency from kHz to Hz
    CSsecondcuefreq[p] = CSsecondcuefreq[p] * 1000;
    golicktube[p]--;                      // Make go lick tube into a zero index for indexing lickctforreq
    if (CSspeaker[p] == 1) {
      CSspeaker[p] = speaker1;
    }
    else if (CSspeaker[p] == 2) {
      CSspeaker[p] = speaker2;
    }
    if (CSlight[p] == 1) {
      CSlight[p] = light1;
    }
    else if (CSlight[p] == 2) {
      CSlight[p] = light2;
    }
    if (CSsecondcuespeaker[p] == 1) {
      CSsecondcuespeaker[p] = speaker1;
    }
    else if (CSsecondcuespeaker[p] == 2) {
      CSsecondcuespeaker[p] = speaker2;
    }
    if (CSsecondcuelight[p] == 1) {
      CSsecondcuelight[p] = light1;
    }
    else if (CSsecondcuelight[p] == 2) {
      CSsecondcuelight[p] = light2;
    }
  }
  for (int p = 0; p < 2 * numCS; p++) {
    if (CSsolenoid[p] == 1) {
      CSsolenoid[p] = solenoid1;
      CSsolenoidcode[p] = 8;
    }
    else if (CSsolenoid[p] == 2) {
      CSsolenoid[p] = solenoid2;
      CSsolenoidcode[p] = 9;
    }
    else if (CSsolenoid[p] == 3) {
      CSsolenoid[p] = solenoid3;
      CSsolenoidcode[p] = 10;
    }
    else if (CSsolenoid[p] == 4) {
      CSsolenoid[p] = solenoid4;
      CSsolenoidcode[p] = 11;
    }
    else if (CSsolenoid[p] == 5) {
      CSsolenoid[p] = lickretractsolenoid1;
      CSsolenoidcode[p] = 12;
    }
    else if (CSsolenoid[p] == 6) {
      CSsolenoid[p] = lickretractsolenoid2;
      CSsolenoidcode[p] = 13;
    }
    else if (CSsolenoid[p] == 56) {
      CSsolenoidcode[p] = 35;
    }
    //    else if (CSsolenoid[p] == 55) {
    //      CSsolenoid[p] = lickretractsolenoid1or2;
    //      CSsolenoidcode[p] = 19;
    //    }
  }


  if (backgroundsolenoid == 1) {
    backgroundsolenoid = solenoid1;
  }
  else if (backgroundsolenoid == 2) {
    backgroundsolenoid = solenoid2;
  }
  else if (backgroundsolenoid == 3) {
    backgroundsolenoid = solenoid3;
  }
  else if (backgroundsolenoid == 4) {
    backgroundsolenoid = solenoid4;
  }

  for (int p = 0; p < numlicktube; p++) {
    if (licksolenoid[p] == 1) {
      licksolenoid[p] = solenoid1;
    }
    else if (licksolenoid[p] == 2) {
      licksolenoid[p] = solenoid2;
    }
    else if (licksolenoid[p] == 3) {
      licksolenoid[p] = solenoid3;
    }
    else if (licksolenoid[p] == 4) {
      licksolenoid[p] = solenoid4;
    }
    else if (licksolenoid[p] == 5) {
      licksolenoid[p] = lickretractsolenoid1;
    }
    else if (licksolenoid[p] == 6) {
      licksolenoid[p] = lickretractsolenoid2;
    }
  }
}

// Check lick status //////
void licking() {
  boolean prevLick;

  prevLick  = lickState[0];                // record previous lick1 state
  lickState[0] = digitalRead(lick1);       // record new lick1 state
  licked    = lickState[0] > prevLick;     // determine if lick1 occured
  lickwithdrawn = lickState[0] < prevLick; // determine if lick1 was withdrawn

  if (licked) {                            // if lick
    Serial.print(1);                       //   code data as lick1 timestamp
    Serial.print(" ");
    Serial.print(ts);                      //   send timestamp of lick
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
    if (licktubesactive) {
      lickctforreq[0]++;
    }

    digitalWrite(lickretractsolenoid1, LOW);
    digitalWrite(lickretractsolenoid2, LOW);
    if (golicktube[cueList[CSct]] == 0 && licktubesactive) {
      nextfxdsolenoid = ts;
    }
    licktubesactive = false;
  }

  if (lickwithdrawn) {                     // if lick withdrawn
    Serial.print(2);                       //   code data as lick1 withdrawn timestamp
    Serial.print(" ");
    Serial.print(ts);                      //   send timestamp of lick
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
  }

  prevLick  = lickState[1];                // record previous lick2 state
  lickState[1] = digitalRead(lick2);       // record new lick2 state
  licked    = lickState[1] > prevLick;     // determine if lick2 occured
  lickwithdrawn = lickState[1] < prevLick; // determine if lick2 was withdrawn

  if (licked) {                            // if lick
    Serial.print(3);                       //   code data as lick2 timestamp
    Serial.print(" ");
    Serial.print(ts);                      //   send timestamp of lick
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
    if (licktubesactive) {
      lickctforreq[1]++;
    }

    digitalWrite(lickretractsolenoid1, LOW);
    digitalWrite(lickretractsolenoid2, LOW);
    if (golicktube[cueList[CSct]] == 1 && licktubesactive) {
      nextfxdsolenoid = ts;
    }
    licktubesactive = false;
  }

  if (lickwithdrawn) {                     // if lick withdrawn
    Serial.print(4);                       //   code data as lick2 withdrawn timestamp
    Serial.print(" ");
    Serial.print(ts);                      //   send timestamp of lick
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
  }

  prevLick  = lickState[2];                // record previous lick3 state
  lickState[2] = digitalRead(lick3);       // record new lick3 state
  licked    = lickState[2] > prevLick;     // determine if lick3 occured
  lickwithdrawn = lickState[2] < prevLick; // determine if lick3 was withdrawn

  if (licked) {                            // if lick
    Serial.print(5);                       //   code data as lick3 timestamp
    Serial.print(" ");
    Serial.print(ts);                      //   send timestamp of lick
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
    lickctforreq[2]++;
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

void frametimestamp() {
  boolean prevframe;
  prevframe = framestate;
  framestate = digitalRead(framein);
  frameon = framestate > prevframe;

  if (frameon) {
    Serial.print(30);                       //   code data as frame timestamp
    Serial.print(" ");
    Serial.print(ts);                       //   send timestamp of frame
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
  }
}

// DELIVER CUE //////////////
void cues() {
  if (CSdur[cueList[CSct]] > 0) {
    if (secondcue == 0) {
      tone(CSspeaker[cueList[CSct]], CSfreq[cueList[CSct]]);               // turn on tone only when the CSdur is bigger than 0
    }
    else {
      tone(CSsecondcuespeaker[cueList[CSct]], CSsecondcuefreq[cueList[CSct]]);
    }
  }

  if (CSpulse[cueList[CSct]] == 1) {
    cuePulseOff = ts + 200;                  // Cue pulsing
    cuePulseOn = 0;
  }
  else if (CSpulse[cueList[CSct]] == 0) {
    cuePulseOff = 0;                         // No cue pulsing
    cuePulseOn = 0;                          // No cue pulsing
  }
  // Zero fixed solenoids given till now
  if (CSdur[cueList[CSct]] > 0) {
    cueOff  = ts + CSdur[cueList[CSct]];                   // set timestamp of cue cessation
  }
  else {
    cueOff = ts + 100;                  // just for the sync with fiber photometry
  }
  lickctforreq[0] = 0;                 // reset lick1 count to zero at cue onset
  lickctforreq[1] = 0;                 // reset lick2 count to zero at cue onset
  lickctforreq[2] = 0;                 // reset lick3 count to zero at cue onset
  licktubesactive = true;
  // Sync with fiber photometry
  digitalWrite(ttloutstoppin, HIGH);
}

void deliverlasertocues() {
  if (laserduration > 0 && lasertrialbytrialflag == 0 && randlaserflag == 0) {
    nextlaser = ts + laserlatency;
  }
  else if (laserduration > 0 && lasertrialbytrialflag == 1 && randlaserflag == 0) {
    if (Laserontrial[CSct] == 1) {
      nextlaser = ts + laserlatency;
    }
  }
}

void delieverlasertorewards() {
  if (laserduration > 0 && lasertrialbytrialflag ==0 && randlaserflag == 0) {
    nextlaser = ts + laserlatency;
  }
}

void lights() {
  //  Serial.print(21 + cueList[CSct]);           // code data as light1 ot light2 timestamp
  //  Serial.print(" ");
  //  Serial.print(ts);                         // send timestamp of light cue
  //  Serial.print(" ");
  //  Serial.print(0);
  //  Serial.print('\n');
  if (CSdur[cueList[CSct]] > 0) {
    if (secondcue == 0) {
      digitalWrite(CSlight[cueList[CSct]], HIGH);           // Turn on light when CSdur is bigger than 0
    }
    else {
      digitalWrite(CSsecondcuelight[cueList[CSct]], HIGH);
    }
  }
  lightOff = ts + lightdur;
  lickctforreq[0] = 0;
  lickctforreq[1] = 0;
  lickctforreq[2] = 0;
  licktubesactive = true;
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
  delete [] cueList;
  int *cueList = 0;
  delete [] Laserontrial;
  int *Laserontrial = 0;
  software_Reboot();

}
