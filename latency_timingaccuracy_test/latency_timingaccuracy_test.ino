int ttloutpin = 42;   // ttl out pin for starting imaging
int lick3     = 26;
boolean lickState;


void setup() {
  // put your setup code here, to run once:
  // initialize arduino states
  Serial.begin(57600);
  pinMode(lick3, INPUT);
  pinMode(ttloutpin, OUTPUT);
  //  start = millis();                    // start time
}

void loop() {
  // put your main code here, to run repeatedly:
  //  ts = millis() - start;               // find time since start
  lickState = digitalRead(lick3);       // record new lick1 state
  if (lickState) {
    //    delay(1);
    delayMicroseconds(100);
    digitalWrite(ttloutpin, HIGH);
    delay(100);
    digitalWrite(ttloutpin, LOW);
  }
}
