/*

This code was used to bench test a 50A rated speed control shield
with a small DC fan. It maps an analog input (in this case a photoresistor) 
to an analog output (pin 9, fanspeed
 */


int fanPin = 9;    // LED connected to digital pin 9
int lightLevel; 
int fanSpeed;

void setup() {

    Serial.begin(9600);

}

void loop() {

  lightLevel = analogRead(A0);
  fanSpeed = map(lightLevel, 0, 1023, 0, 255);
 
  analogWrite(fanPin, fanSpeed);
  


}


