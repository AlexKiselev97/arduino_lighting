const int LEFT_PIR = 2;
const int RIGHT_PIR = 3;
const int FIRST = 5;
const int LAST = 12;
const int lamps[4] = {7, 8, 10, 11};
volatile bool noOne;
volatile bool leftPirActive;
volatile bool rightPirActive;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  noOne = true;
  leftPirActive = false;
  rightPirActive = false;
  pinMode(LEFT_PIR, INPUT);
  pinMode(RIGHT_PIR, INPUT);
  for (int i=0; i < 4; ++i)
    pinMode(lamps[i], OUTPUT);
  delay(10000); //sensor calibration
  attachInterrupt(digitalPinToInterrupt(LEFT_PIR), leftPirInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_PIR), rightPirInterrupt, RISING);
  
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

void leftPirLight()
{
  analogWrite(FIRST, 255);
  analogWrite(LAST, 255);
  for (int i=0; i < 4; ++i)
  {
    digitalWrite(lamps[i], HIGH);
    delay(700);
  }
  delay(15000);
  for (int i=3; i >= 0; --i)
  {
    digitalWrite(lamps[i], LOW);
    delay(700);
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
  {
    digitalWrite(lamps[i], HIGH);
    delay(700);
  }
  delay(15000);
  for (int i=0; i < 4; ++i)
  {
    digitalWrite(lamps[i], LOW);
    delay(700);
  }
  leftPirActive = false;
  rightPirActive = false;
  noOne = true;
}

void loop() {
  if (noOne)
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
  else
  {
    if (leftPirActive)
      leftPirLight();
    else
      rightPirLight();
  }
}
