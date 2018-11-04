#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 12);
#include <iarduino_RTC.h> 
iarduino_RTC time(RTC_DS3231);

// Создание пинов
const short relay11 = 2;                   // Помпа
const short relay12 = 4;                   // Газовый котёл
const short relay21 = 7;                   // Электро котёл
//const short relay22 = ;
const short PIN10 = 10;

int times;                                 // Переменные которые хранят время

bool pin10 = false;
short index = 0;
short index1 = 0;

void Display(short i);
void disablell();
void Tablet();
void startClock();
void initLCD();
void relay(int i);

void setup()
{
  Serial.begin(9600);
  initLCD();                               // Инициализация Led 
  delay(300);
  time.begin();                            // Инициализация часов реального времени(запуск)
  //time.settime(0, 0, 18, 30, 9, 18, 7); // Установка времени
  pinMode(PIN10, INPUT);
  pinMode(relay11, OUTPUT);
  pinMode(relay12, OUTPUT);
  pinMode(relay21, OUTPUT);
  disableAll();
}

void loop()
{
  pin10 = digitalRead(PIN10);
  relay(times);                              // Запускаем реле
  startClock();                              // Запускаем часы
}

void startClock()
{
  if(millis() % 1000 == 0)                 // Проверка прошла ли одна секунда
  {
    lcd.setCursor(0, 1);                   // Переводим курсор на первую строку второго ряда
    lcd.print(time.gettime("H:i:s"));      // Получаем и выводим время на экран
    times = (time.Hours * 3600) + (time.minutes * 60) + time.seconds;
    //Serial.print(index);
    if (index1 != index){
      Display(index);
      index1 = index;
    }
  }
}

void initLCD()
{
  lcd.init();                             // Инициализация дисплея  
  lcd.backlight();                        // Подключение подсветки
}

void disableAll()
{
  digitalWrite(relay21, HIGH);            // Отключить все пины
  digitalWrite(relay11, HIGH);
  digitalWrite(relay12, HIGH);
}

void relay(int i)                       // Управление реле
{     
   disableAll();
   if (i >= 18000 && i <= 25199){                 // Ранее утро
     digitalWrite(relay11, LOW);
     digitalWrite(relay21, LOW);
     index = 3;           
   }     
   if (i >= 25200 && i <= 75599 && pin10 == true){            // Помпа + Газ
     digitalWrite(relay12, LOW);  
     digitalWrite(relay11, LOW);
     index = 2;
   }
   if (i >= 75600 || i <= 17999 && pin10 == true){            // Помпа + Електро
     digitalWrite(relay11, LOW);
     digitalWrite(relay21, LOW);
     index = 1;
   }
}

void Tablet()
{                                         // Если что-то сломаеться)))
  digitalWrite(relay12, HIGH);
  //Serial.print("yes");
  delay(500);
  digitalWrite(relay12, LOW);
  //Serial.print("no");
  delay(500);
}

void Display(short i)                     // Вывод на экран
{                      
  lcd.setCursor(0,0);
  lcd.print("           ");
  lcd.setCursor(0,0);
  if (i == 1) lcd.print("EL/Pomp");
  if (i == 2) lcd.print("GAS/Pomp");
  if (i == 3) lcd.print("Morning");
}
