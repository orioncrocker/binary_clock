// sketch which assigns time to DS3231

#include <DS3231.h>
DS3231 rtc(SDA, SCL);

void setup() {
  Serial.begin(115200);
  rtc.begin();
  rtc.setTime(18, 50, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  Time current = rtc.getTime();
  
  Serial.print(current.hour);
  Serial.print(":");
  Serial.print(current.min);
  Serial.print(":");
  Serial.print(current.sec);
  Serial.print("\n");
  
  delay(1000);
}
