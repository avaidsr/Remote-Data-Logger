/*
  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

float vout = 0.0;
float vin = 0.0;
float vina = 0.0;
float R1 = 1019000.0; // resistance of R1 (1M) -see text!
float R2 = 97800.0;  // resistance of R2 (100K) - see text!
float R1a = 1000000.0; // resistance of R1 (1M) -see text!
float R2a = 100000.0;  // resistance of R2 (100K) - see text!
int value = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
//  int sensorValue = analogRead(A2);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
//  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
//  Serial.println(voltage*11.4272727);
  //Serial.println(voltage);
  //delay(1000);

    // read the value at analog input
   value = analogRead(A2);
   vout = value * (5.0 / 1023.0); // see text
   vin = vout / (R2/(R1+R2)); 
   vina = vout / (R2a/(R1a+R2a));
   if (vin<0.09) {
   vin=0.0;//statement to quash undesired reading !
} 
//Serial.println(vin*11.4272727);
  Serial.println(vin);  //most accurate reading for now
  Serial.println(vina);
  Serial.println(vout);
  Serial.println(value);
  Serial.println(value/17.899);
  Serial.println(value/16.855);
  Serial.println(" ");
  delay(1000);
}
