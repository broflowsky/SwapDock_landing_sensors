/*
  Valentin Puyfourcat
  SwapDock Landing pad sensors

  Determine whether a drone has landed on the platform

  using 3 FSR sensors and an arduino nano
*/

//Input pins
const byte NUM_SENSOR = 1;
const byte sensor_pin0 = A0;
//const byte sensor_pin1 = A1;
//const byte sensor_pin2 = A2;
//const byte sensor_pins[NUM_SENSOR] = {sensor_pin0, sensor_pin1, sensor_pin2};
const byte sensor_pins[NUM_SENSOR] = {A0};
//average filter
const byte NUM_READINGS = 8;
short readings[NUM_READINGS];
short readingsTotal = 0;
short readIndex = 0;

//threshold that means a drone has landed
// 970/1023 = 95%
const short _THRESH_PERCENT = 800;//
const short THRESH = _THRESH_PERCENT*NUM_READINGS;//TBD with actual platform


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  sensorInit();
  Serial.flush();
  Serial.print("Threshold value: ");
  Serial.println(THRESH);
  Serial.print("Threshold percent: ");
  Serial.println(_THRESH_PERCENT);
}

void loop() {

  ReadSensors();
  Serial.println(readingsTotal);
  if (readingsTotal > THRESH)
  {
    digitalWrite(LED_BUILTIN,HIGH);
  }
  else digitalWrite(LED_BUILTIN,LOW);

  delay(1);

}
void sensorInit() {

  for (int i = 0; i < NUM_SENSOR; ++i)
    pinMode(sensor_pins[i], INPUT);
  for (int i = 0; i < NUM_READINGS; ++i) { //Do X readings
    short tempRead = 0;
    for (int j = 0; j < NUM_SENSOR; ++j) { //for each sensor, read its value
      tempRead += analogRead(sensor_pins[j]);
      delay(10);
    }
    readings[i] = tempRead / NUM_SENSOR; //average out
    readingsTotal += readings[i];
  }

}

void ReadSensors() {
  short value = 0;
  for (int i = 0; i < NUM_SENSOR; ++i) {
    value += analogRead(sensor_pins[i]);
    delay(10);
  }

  readingsTotal -= readings[readIndex];
  readings[readIndex] = value / NUM_SENSOR;
  readingsTotal += readings[readIndex++];
  if (readIndex >= NUM_READINGS)
    readIndex = 0;

}
