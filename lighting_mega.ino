 const int LEFT_PIR = 2;
const int RIGHT_PIR = 3;
const int FIRST = 5;
const int LAST = 12;
const int lamps[4] = {7, 8, 10, 11};
const int BUTTON_PULSE_ENDS = 18;
const int BUTTON_PULSE_ALL = 19;
volatile bool noOne = true;
volatile bool leftPirActive = false;
volatile bool rightPirActive = false;
volatile int mode = 1;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(LEFT_PIR, INPUT);
  pinMode(RIGHT_PIR, INPUT);
  pinMode(BUTTON_PULSE_ALL, INPUT_PULLUP);
  pinMode(BUTTON_PULSE_ENDS, INPUT_PULLUP);
  delay(15000); //sensor calibration
  attachInterrupt(digitalPinToInterrupt(LEFT_PIR), leftPirInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_PIR), rightPirInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PULSE_ENDS), buttonPulseEndsInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PULSE_ALL), buttonPulseAllInterrupt, RISING);
}

void leftPirInterrupt()
{
  noOne = false;
  leftPirActive = true;
}

void rightPirInterrupt()
{
  noOne = false;
  rightPirActive = true;
}

void buttonPulseEndsInterrupt()
{
  mode = 1;
  if (noOne)
    for (int i=0; i < 4; ++i)
      analogWrite(lamps[i], 0);
}

void buttonPulseAllInterrupt()
{
  mode = 2;
}

void leftPirLight()
{
  analogWrite(FIRST, 255);
  analogWrite(LAST, 255);
  for (int i=0; i < 4; ++i)
    for (int j=1; j < 256; ++j)
    {
      analogWrite(lamps[i], j);
      delay(5);
    }
  delay(15000);
  for (int i=3; i >= 0; --i)
    for (int j=255; j >= 0; --j)
    {
      analogWrite(lamps[i], j);
      delay(5);
    }
  leftPirActive = false;
  rightPirActive = false;
  noOne = true;
}

void rightPirLight()
{
  analogWrite(FIRST, 255);
  analogWrite(LAST, 255);
  for (int i=3; i >= 0; --i)
    for (int j=1; j < 256; ++j)
    {
      analogWrite(lamps[i], j);
      delay(5);
    }
  delay(15000);
  for (int i=0; i < 4; ++i)
    for (int j=255; j >= 0; --j)
    {
      analogWrite(lamps[i], j);
      delay(5);
    }
  leftPirActive = false;
  rightPirActive = false;
  noOne = true;
}

void mode1()
{
  for (int i=1; i < 256 && noOne; ++i)
  {
      analogWrite(FIRST, i);
      analogWrite(LAST, i);
      delay(12);
  }
  for (int i=255; i >= 1 && noOne; --i)
  {
      analogWrite(FIRST, i);
      analogWrite(LAST, i);
      delay(12);
  }
}

void mode2()
{
  for (int i=1; i < 256 && noOne; ++i)
  {
      analogWrite(FIRST, i);
      analogWrite(LAST, i);
      for (int j=0; j < 4; ++j)
        analogWrite(lamps[j], i);
      delay(12);
  }
  for (int i=255; i >= 1 && noOne; --i)
  {
      analogWrite(FIRST, i);
      analogWrite(LAST, i);
      for (int j=0; j < 4; ++j)
        analogWrite(lamps[j], i);
      delay(12);
  }
}

void loop() {
  if (noOne)
  {
    switch(mode)
    {
      case 1: mode1(); break;
      case 2: mode2(); break;
    }
  }
  else
  {
    for (int i=0; i < 4; ++i)
      analogWrite(lamps[i], 0);
    if (leftPirActive)
      leftPirLight();
    else if (rightPirActive)
      rightPirLight();
  }
}
