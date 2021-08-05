int interval;
unsigned long reading;
unsigned long ts;
unsigned long start;
unsigned long tsct;
unsigned long nextevent;
boolean tsctflag;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  reading = 0;
  interval = 3000;
  tsct = 0;
  tsctflag = true;
  nextevent = 0;
//  while (reading != 84) {
//    reading = Serial.read();
//  }
  start = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
  //  reading = Serial.read();
  ts = micros() - start;
  if (tsct <= 500 && ts >= nextevent) {
    Serial.print(25);
    Serial.print(" ");
    Serial.print(ts);
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
    nextevent = ts + interval;
    tsct = tsct + 1;
  }
  if (tsctflag == true && tsct > 500) {
    Serial.print(0);                       //   signal end of timestamp collection
    Serial.print(" ");
    Serial.print(ts);                      //   send timestamp
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
    tsctflag = false;
  }
}
