// Nombre archivo: Debe llamarse cóomo el directorio en el que se encuentra por Arduino IDE
// CONEXIONES:
// DS1302 CLK/SCLK --> 7
// DS1302 DAT/IO --> 6
// DS1302 RST/CE --> 5
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#include <RtcDS1302.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
ThreeWire myWire(6, 7, 5);           // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
int pirSensor = 2;
int A_hour = 12;
int A_minute = 31;
int AlarmIsActive = 1;
int buzzer = 3;

// ♥️
byte customChar[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

void setupRTC() {
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);
  printDateTime(compiled);
  Serial.println();
  Rtc.Begin();

  if (!Rtc.IsDateTimeValid()) {
    // Common Causes:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected()) {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning()) {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    Serial.println("RTC is newer than compile time. (this is expected)");
  } else if (now == compiled) {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
}

void setupAlarm() {
}

void setupLCD() {
  lcd.init();  // initialize the lcd
  lcd.backlight();
  lcd.createChar(0, customChar);  // create a new custom character
}

void setup() {
  Serial.begin(57600);
  pinMode(pirSensor, INPUT);
  setupRTC();
  setupLCD();
}

void loop() {
  RtcDateTime now = Rtc.GetDateTime();

  printDateTime(now);
  Serial.println();

  if (!now.IsValid()) {
    // Common Causes:
    //    1) the battery on the device is low or even missing and the power line was disconnected
    Serial.println("RTC lost confidence in the DateTime!");
  }



  if (now.Hour() == A_hour && now.Minute() == A_minute && AlarmIsActive == 1 && now.Second() >= 0 && now.Second() <= 2) {
    tone(buzzer, 1);  //You can modify the tone or make your own sound
    delay(100);
    tone(buzzer, 2);
    delay(100);
    lcd.clear();
    // Message to show when the alarm is ringing
    lcd.print("Get up !!!");
    Serial.println("Get Up !!!");


    int value = digitalRead(pirSensor);

    Serial.println(value);
    if (value == 1) {
      AlarmIsActive = 0;
    }

    delay(5000);
  } else {
    noTone(buzzer);
  }

  delay(1000);  // one second
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt) {
  char datestring[26];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  lcd.home();
  Serial.print(datestring);
  lcd.print(datestring);

  lcd.setCursor(0, 1);  // move cursor to (2, 0)
  lcd.write((byte)0);   // print the custom char at (2, 0)

  lcd.setCursor(15, 1);  // move cursor to (2, 0)
  lcd.write((byte)0);    // print the custom char at (2, 0)
}