int interval;
unsigned long reading;
unsigned long ts;
unsigned long start;
unsigned long tsct;
unsigned long nextevent;
boolean tsctflag;
const int numvec = 500;
unsigned long tsvec[numvec];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  reading = 0;
  interval = 3;
  tsct = 0;
  tsctflag = true;
  nextevent = 0;
  for (int p = 0; p < numvec; p++) {
    tsvec[p] = p*interval;
  }
  while (reading != 84) {
    reading = Serial.read();
  }
  start = millis();
}

void loop() {
  //   put your main code here, to run repeatedly:
  reading = Serial.read();
  ts = millis() - start;
  if (tsct <= 500 && ts >= tsvec[tsct]) {
    Serial.print(25);
    Serial.print(" ");
    Serial.print(ts);
    Serial.print(" ");
    Serial.print(0);
    Serial.print('\n');
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
