//Alarm Clock Project for EECS2032
//Jonah Ottini 
//ID: 2148945030 
// https://github.com/MarkusLange/RTCDue for RTC library
// https://github.com/marcoschwartz/LiquidCrystal_I2C   for Arduino due library for LCD
// http://playground.arduino.cc/Main/I2cScanner  for Arduino I2C address scanner sketch

#include <RTCDue.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>   // make sure to change return value to 1 , in LiquidCrystal_I2C.cpp file of this library to avoid problem of only first character being displayed

RTCDue rtc(XTAL);
LiquidCrystal_I2C lcd(0x27,16,2); // put here correct address of lcd...  if not known, use Arduino I2CScanner given in above link

//Varriables for day names, and time and date parameters
const char* daynames[]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
int hh,mm,ss,dow,dd,mon;
uint16_t yyyy;

//Booleans used to Start, Stop or Snooze the alarm
bool alarmTimeCheck = false; 
bool alarmStopCheck = true;
bool snoozeCheck = false;

//Alarm time variables
int alarmHrs = 0; 
int alarmMin = 0;
int alarmSec = 0;

//Variable to define how long the Snooze function extends the alarm in minutes
const int snoozeOffset = 15;


void setup() {
  #define STOP 47
  #define ALARM 31 
  #define SNOOZE 27

  pinMode(STOP, INPUT);
  pinMode(ALARM, OUTPUT); 
  pinMode(SNOOZE, INPUT);
  
  rtc.begin();
  rtc.setClock(__DATE__, __TIME__);
  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.home();
  lcd.clear();                      // ckear the lcd 

//Used to set RTC time, useful for testing
  rtc.setTime(23, 59, 56);  // set RTC starting time ...  later buttons can be used to set the time settings  
  //rtc.setDate(31, 12,2023);  //set RTC starting date ...  later buttons can be used to set the date settings
}

void loop() {
  rtc.getTime(&hh,&mm,&ss);  // get time from Arduino RTC
  rtc.getDate(&dow,&dd,&mon,&yyyy);   // get date from Arduino RTC
  lcd.home();
  lcd.setCursor(1, 0);                  // column 1 , row 0
  lcd.print("Time: ");
  
  // Print time
  if(hh <10) {lcd.print("0"); }      // to display in 01,02,03... format
    lcd.print(hh);                      // display hours
    lcd.print(":");

  if(mm <10) {lcd.print("0"); }
    lcd.print(mm);                     // display minutes
    lcd.print(":");
    
  if(ss <10) {lcd.print('0'); }
  lcd.print(ss);                       // display seconds

  // Print day and date 
  lcd.setCursor(1, 1);
  lcd.print(daynames[dow]);  // display Day name
  lcd.print(" ");
  if(dd <10) {lcd.print("0"); }  
  lcd.print(dd);                // display date
  lcd.print("/");
  if(mon <10) {lcd.print("0"); }
  lcd.print(mon);              // display month
  lcd.print("/");
  lcd.print(yyyy);            // display year

  //Makes sure you cant increase the alarm sec or mins above 60 when Snoozing
  if(alarmSec >= 60){ 
    alarmSec = alarmSec-60; 
    alarmMin = alarmMin + 1;
  }
  if(alarmMin >= 60){ 
    alarmMin = alarmMin-60; 
    alarmHrs = alarmHrs+1;
  }

  //Set alarm time for Sunday
  if (dow == 0 && snoozeCheck == false){ 
    alarmHrs = 9; 
    alarmMin = 0;
    alarmSec = 0;
  }
  //Set alarm time for Monday
  else if (dow == 1 && snoozeCheck == false){ 
    alarmHrs = 9; 
    alarmMin = 0;
    alarmSec = 0;
  }
  //Set alarm time for Tuesday
  else if (dow == 2 && snoozeCheck == false){ 
    alarmHrs = 9; 
    alarmMin = 0;
    alarmSec = 0;
  }
  //Set alarm time for Wednesday
  else if (dow == 3 && snoozeCheck == false){ 
    alarmHrs = 9; 
    alarmMin = 0;
    alarmSec = 0;
  }
  //Set alarm time for Thursday
  else if (dow == 4 && snoozeCheck == false){ 
    alarmHrs = 9; 
    alarmMin = 0;
    alarmSec = 0;
  }
  //Set alarm time for Friday
  else if (dow == 5 && snoozeCheck == false){ 
    alarmHrs = 9; 
    alarmMin = 0;
    alarmSec = 0;
  }
  //Set alarm time for Saturday
  else if (dow == 6 && snoozeCheck == false){ 
    alarmHrs = 0; 
    alarmMin = 0;
    alarmSec = 0;
  }

  //Changes bool varaibles to run the alarm when the specified time passes
  if(hh == alarmHrs && mm == alarmMin && ss == alarmSec){
    alarmTimeCheck = true;
    alarmStopCheck = false; 
  }

  if (alarmTimeCheck == true){

    //When stop button is pressed
    if(digitalRead(STOP) == HIGH){ 
      for(int i = 0; i  < 3; i++){ 
        lcdMessage("Alarm stopped");
        delay(500);
        lcd.clear();
        delay(500);
      }
      alarmTimeCheck = false;
      alarmStopCheck = true;  
      snoozeCheck = false;
    } 

    //When Snooze button is pressed
    else if(digitalRead(SNOOZE) == HIGH){ 
      for(int i = 0; i  < 3; i++){ 
        lcdMessage("Alarm snoozed");
        delay(500);
        lcd.clear();
        delay(500);
      }
      alarmMin = alarmMin + snoozeOffset; 
      alarmTimeCheck = false; 
      alarmStopCheck = true; 
      snoozeCheck = true;
    }  

    //Run the alarm if neither button is being pressed
    //Has various melodies
    else if(alarmStopCheck == false){ 
      lcdMessage("Alarm");
      //alarm1();
      //alarm2();
      alarm2();
   }
  }
}

//Method to print a message to the LCD screen
void lcdMessage(String message){ 
  lcd.clear(); 
  lcd.print(message);
}

// Various alarm melodies that can be swapped between
void alarm1(){ 
  digitalWrite(ALARM, HIGH); 
  delay(500); 
  digitalWrite(ALARM, LOW);
  delay(500);
}

void alarm2(){ 
  digitalWrite(ALARM, HIGH); 
  delay(100); 
  digitalWrite(ALARM, LOW);
  delay(100);
}

void alarm3(){ 
  digitalWrite(ALARM, HIGH); 
  delay(50); 
  digitalWrite(ALARM, LOW);
  delay(50);

  digitalWrite(ALARM, HIGH); 
  delay(50); 
  digitalWrite(ALARM, LOW);
  delay(50);

  digitalWrite(ALARM, HIGH); 
  delay(50); 
  digitalWrite(ALARM, LOW);
  delay(50);

  digitalWrite(ALARM, HIGH); 
  delay(50); 
  digitalWrite(ALARM, LOW);
  delay(750);
}
