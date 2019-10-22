#include <DS3231.h>
#define uint8_t uint

DS3231 rtc(SDA, SCL);
Time currentTime;

/* SR# Shift Register array
 0 = Data
 1 = Latch 
 2 = Clock
 */
const uint SR1[] = {11, 12, 13};
const uint SR2[] = {8, 9, 10};
const uint hourPins[] = {3, 4, 5, 6, 7};

// buttons
const uint minuteButton = 2;
const uint hourButton = 0;
// state will change
uint buttonState = 0;

void setup() 
{
  rtc.begin();
  
  // sets minute and second pins to output
  for (uint i = 0; i < 3; i++)
  {
    pinMode(SR1[i], OUTPUT);
    pinMode(SR2[i], OUTPUT);
  }
  
  // sets hour pins to output
  for (uint i = 0; i < 5; i++)
    pinMode(hourPins[i], OUTPUT);

  // button pins
  pinMode(2, INPUT);
  pinMode(0, INPUT);
    
  // sets baud output
  Serial.begin(115200);
}

void serialTime()
{
  uint hours = currentTime.hour;
  if (hours > 12)
    hours = hours - 12;
  Serial.print(hours);
  Serial.print(":");
  Serial.print(currentTime.min);
  Serial.print(":");
  Serial.print(currentTime.sec);
}

void serialOutput(uint value)
{
  uint index = 0;
  for (uint i = 1 << 6; i > 0; i=i/2)
  {
    if (index == 3)
      Serial.print("| ");
    (value & i)? Serial.print("1"): Serial.print("0");
    Serial.print(" ");
    index++;
  }
  Serial.print("\n");
}

uint calculateShiftOutput(uint value)
{
  uint output = 0;
  output = (value / 10) << 4;
  output = output + (value % 10);
  return output;
}

void pushShiftOutput(uint shiftArray[], uint value)
{
  digitalWrite(shiftArray[1], LOW);
  shiftOut(shiftArray[0], shiftArray[2], MSBFIRST, value);
  digitalWrite(shiftArray[1], HIGH);
}

void outputHours(uint value)
{
  uint index = 0;
  for (uint i = 1 << 4; i > 0; i=i/2)
  {
    (value & i)? digitalWrite(hourPins[index], HIGH): digitalWrite(hourPins[index], LOW);
    index++;
  }
  Serial.print("\n");
}

void ledOutput()
{
  // get values for units of time, reduces need to call time functions more than once
  uint hours = currentTime.hour%12;
  if (hours == 0)
    hours = 12;
  hours = calculateShiftOutput(hours);
  
  uint minutes = calculateShiftOutput(currentTime.min);
  uint seconds = calculateShiftOutput(currentTime.sec);
  
  // hours
  outputHours(hours);
  Serial.print("H: ");
  serialOutput(hours);
  
  // minutes
  pushShiftOutput(SR2, minutes);
  Serial.print("M: ");
  serialOutput(minutes);
  
  // seconds
  pushShiftOutput(SR1, seconds);
  Serial.print("S: ");
  serialOutput(seconds);
}

/*
void buttonCheck()
{
  buttonState = digitalRead(minuteButton);
  Serial.print(buttonState);
  Serial.print("\n");
  if (buttonState == HIGH)
    setTime(hour(currentTime), minute(currentTime) + 1, second(currentTime), 0, 0, 0);

  // reset button state
  buttonState = 0;
  
  buttonState = digitalRead(hourButton);
  Serial.print(buttonState);
  Serial.print("\n");
  if (hourButton == HIGH)
    setTime(hour(currentTime) + 1, minute(currentTime), second(currentTime), 0, 0, 0);
}
*/

void loop()
{ 
  Serial.print("\n");
  currentTime = rtc.getTime(); // fetches current time from DS3231
  //buttonCheck();             // checks to see if buttons are pushed
  serialTime();                // outputs current time to serial output
  ledOutput();                 // pushes calculated current time values to LEDs
  delay(1000);
}
