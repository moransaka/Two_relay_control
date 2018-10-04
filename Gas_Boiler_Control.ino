#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 12);
#include <iarduino_RTC.h> 
iarduino_RTC time(RTC_DS3231);

const short relay11 = 2;
const short relay12 = 4;
const short relay21 = 7;
//const short relay22 = ;
const short pin10 = 10;

short times;
bool index;

void Display(bool i);
void Tablet();
void startClock();
void initLCD();
void relay(short i);

void setup()
{
  Serial.begin(9600);
  initLCD();
  delay(300);
  time.begin();                              //Инициализация часов реального времени(запуск)
//time.settime(0, 41, 16, 30, 9, 18, 7);   //Установка времени
  pinMode(pin10, INPUT);
  pinMode(relay11, OUTPUT);
  pinMode(relay12, OUTPUT);
  pinMode(relay21, OUTPUT);
}

void loop()
{
  digitalWrite(relay21, HIGH);
  digitalWrite(relay11, HIGH);
  digitalWrite(relay12, HIGH);
  if (digitalRead(pin10) == HIGH){ //
    relay(times);
  }
  startClock();                    //Запускаем часы
}

void startClock()
{
  if(millis() % 1000 == 0)
  {
    lcd.setCursor(0, 1);                   //Переводим курсор на первую строку второго ряда
    lcd.print(time.gettime("H:i:s"));      //Получаем и выводим время на экран
    times = time.seconds;
    Display(index);
    //Serial.print(times);
    delay(500);
  }
    
}

void initLCD()
{
  lcd.init();                      // Инициализация дисплея  
  lcd.backlight();                 // Подключение подсветки
  lcd.setCursor(0, 0);             // Установка курсора в начало первой строки
  lcd.print("DONT PANIC!");        // Набор текста на второй строке
}

void relay(short i){
  if (i > 5 && i < 10){
    digitalWrite(relay21, HIGH);
    index = true;  
  }
  else{
    digitalWrite(relay21, LOW);
  }
  
  if (i < 5 || i > 10){
    digitalWrite(relay12, HIGH);  
    index = false;
  }
  else{
    digitalWrite(relay12, LOW);
  }
  
  if (i < 5 || i > 10){
    digitalWrite(relay11, HIGH);
    index = false;
  }
  else{
    digitalWrite(relay11, LOW);
  }
}

void Tablet(){
  digitalWrite(relay12, HIGH);
  //Serial.print("yes");
  delay(500);
  digitalWrite(relay12, LOW);
  //Serial.print("no");
  delay(500);
}

void Display(bool i){
  if (i == true){
    lcd.setCursor(0,0);
    lcd.print("GAS Boiler is works");
  }
  else{
    lcd.setCursor(0,0);
    lcd.print("EL Boiler is works");
  }
}