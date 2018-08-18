// PINS
//const int LEFT_PIR = 21; //temporarily
//const int RIGHT_PIR = 20; //temporarily
const int lamps[] = {7, 8, 9, 10, 11, 12};
const int FIRST_LAMP = 7; // on the first floor
const int LAST_LAMP = 12; // on the second floor
const int BUTTON_MODE_CHANGE = 18;
// CONST
const int WAIT_PEOPLE = 15000; // time to move up the stairs
const int MAX_BRIGHTNESS = 255;
const int MIN_BRIGHTNESS = 0;

unsigned long startButtonDown = 0;

volatile bool noOne = true;
volatile bool leftPirActive = false;
volatile bool rightPirActive = false;
volatile int mode = 1;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  //pinMode(LEFT_PIR, INPUT);
  //pinMode(RIGHT_PIR, INPUT);
  pinMode(BUTTON_MODE_CHANGE, INPUT);
  //delay(15000); // calibration time
  //attachInterrupt(digitalPinToInterrupt(LEFT_PIR), leftPirInterrupt, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(RIGHT_PIR), rightPirInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_MODE_CHANGE), buttonModeChangeInterrupt, CHANGE);
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

void buttonModeChangeInterrupt()
{
  if (startButtonDown == 0 || (millis() - startButtonDown) > 2000)
    startButtonDown = millis();
  else
    return;
  switch(mode)
  {
    case 1:
    {
      mode = 2;
      break;
    }
    case 2: 
    {
      mode = 3;
      break;
    }
    case 3:
    {
      mode = 4;
      break;
    }
    case 4:
    {
      mode = 1;
      break;
    }
  }
}

void leftPirLight()
{
  analogWrite(FIRST_LAMP, MAX_BRIGHTNESS);
  analogWrite(LAST_LAMP, MAX_BRIGHTNESS);
  for (int i=1; i < 5; ++i)
    for (int j=MIN_BRIGHTNESS; j <= MAX_BRIGHTNESS; ++j)
    {
      analogWrite(lamps[i], j);
      delay(5);
    }
  delay(WAIT_PEOPLE);
  for (int i=4; i > 0; --i)
    for (int j=MAX_BRIGHTNESS; j >= MIN_BRIGHTNESS; --j)
    {
      analogWrite(lamps[i], j);
      delay(5);
    }
  leftPirActive = false;
  rightPirActive = false;
  noOne = true;
  digitalWrite(13, LOW);
}

void rightPirLight()
{
  analogWrite(FIRST_LAMP, 255);
  analogWrite(LAST_LAMP, 255);
  for (int i=4; i >= 1; --i)
    for (int j=MIN_BRIGHTNESS; j <= MAX_BRIGHTNESS; ++j)
    {
      analogWrite(lamps[i], j);
      delay(5);
    }
  delay(WAIT_PEOPLE);
  for (int i=1; i < 5; ++i)
    for (int j=MAX_BRIGHTNESS; j >= MIN_BRIGHTNESS; --j)
    {
      analogWrite(lamps[i], j);
      delay(5);
    }
  leftPirActive = false;
  rightPirActive = false;
  noOne = true;
}

void modeFirstLastPulse()
{
  for (int i=MIN_BRIGHTNESS; i <= MAX_BRIGHTNESS && noOne; ++i)
  {
      analogWrite(FIRST_LAMP, i);
      analogWrite(LAST_LAMP, i);
      delay(12);
  }
  for (int i=MAX_BRIGHTNESS; i >= MIN_BRIGHTNESS && noOne; --i)
  {
      analogWrite(FIRST_LAMP, i);
      analogWrite(LAST_LAMP, i);
      delay(12);
  }
}

void modeAllPulse()
{
  for (int i=MIN_BRIGHTNESS; i <= MAX_BRIGHTNESS && noOne; ++i)
  {
      for (int j=0; j < 6; ++j)
        analogWrite(lamps[j], i);
      delay(12);
  }
  for (int i=MAX_BRIGHTNESS; i >= MIN_BRIGHTNESS && noOne; --i)
  {
      for (int j=0; j < 6; ++j)
        analogWrite(lamps[j], i);
      delay(12);
  }
}

void modeWave()
{
  for (int i=0; i < 8 && noOne; ++i)
  { // волна вперед
      if (i < 6)
        analogWrite(lamps[i], MAX_BRIGHTNESS);
      if (i - 1 >= 0)
        analogWrite(lamps[i-1], MAX_BRIGHTNESS/4);
      if (i - 2 >= 0)
        analogWrite(lamps[i-2], MIN_BRIGHTNESS);
      delay(200);
  }
  delay(500);
  for (int i=7; i >= -1 && noOne; --i)
  { // волна назад
      if (i < 6 && i >= 0)
        analogWrite(lamps[i], MAX_BRIGHTNESS);
      if (i + 1 < 6)
        analogWrite(lamps[i+1], MAX_BRIGHTNESS/4);
      if (i + 2 < 6)
        analogWrite(lamps[i+2], MIN_BRIGHTNESS);
      delay(200);
  }
}

void modeEven()
{
  for (int i=MIN_BRIGHTNESS; i <= MAX_BRIGHTNESS; ++i)
  {
    for (int j=0; j < 6; ++j)
    {
      if (j%2 != 0)
        analogWrite(lamps[j], i);
      else
        analogWrite(lamps[j], MAX_BRIGHTNESS-i);
    }
    delay(12);
  }
  
  for (int i=MIN_BRIGHTNESS; i <= MAX_BRIGHTNESS; ++i)
  {
    for (int j=0; j < 6; ++j)
    {
      if (j%2 == 0)
        analogWrite(lamps[j], i);
      else
        analogWrite(lamps[j], MAX_BRIGHTNESS-i);
    }
    delay(12);
  }
}

void loop() {
  for (int i=0; i < 6; ++i)
      analogWrite(lamps[i], MIN_BRIGHTNESS);
  if (noOne)
  {
    switch(mode)
    {
      case 1:
        modeFirstLastPulse(); break;
      case 2:
        modeAllPulse(); break;
      case 3:
        modeWave(); break;
      case 4:
        modeEven(); break;
    }
  }
  else
  {
    if (leftPirActive)
      leftPirLight();
    else if (rightPirActive)
      rightPirLight();
  }
}
