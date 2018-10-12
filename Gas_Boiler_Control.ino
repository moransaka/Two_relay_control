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
const short pin10 = 10;

short times;                               // Переменные которые хранят время
short index = 0;
short index1 = 0;

void Display(short i);
void disabledAll();
void Tablet();
void startClock();
void initLCD();
void relay(short i);

void setup()
{
  Serial.begin(9600);
  initLCD();                               // Инициализация Led 
  delay(300);
  time.begin();                            //Инициализация часов реального времени(запуск)
  //time.settime(0, 41, 16, 30, 9, 18, 7); //Установка времени
  pinMode(pin10, INPUT);
  pinMode(relay11, OUTPUT);
  pinMode(relay12, OUTPUT);
  pinMode(relay21, OUTPUT);
  disabledAll();
}

void loop()
{
//  if (digitalRead(pin10) == HIGH){         // Запуск при удерживании кнопки
//    relay(times);                          // Запускаем реле
//  }
  relay(times);
  startClock();                              //Запускаем часы
}

void startClock()
{
  if(millis() % 1000 == 0)                 // Проверка прошла ли одна секунда
  {
    lcd.setCursor(0, 1);                   // Переводим курсор на первую строку второго ряда
    lcd.print(time.gettime("H:i:s"));      // Получаем и выводим время на экран
    times = time.seconds;                  // Присваиваем время
    Serial.print(index);
    if (index1 != index){
      Display(index);
      index1 = index;
    }
    //Serial.print(times);                 // Тестирование время 
    delay(500);
  }
    
}

void initLCD()
{
  lcd.init();                             // Инициализация дисплея  
  lcd.backlight();                        // Подключение подсветки
  lcd.setCursor(0, 0);                    // Установка курсора в начало первой строки
  lcd.print("DONT PANIC!");               // Набор текста на первой строке
}

void disabledAll()
{
  digitalWrite(relay21, HIGH);            // Отключить все пины
  digitalWrite(relay11, HIGH);
  digitalWrite(relay12, HIGH);
}

void relay(short i)                       // Управление реле
{
   disabledAll();                      
  if (i >= 1 && i <= 10){
    digitalWrite(relay11, LOW);
    digitalWrite(relay21, LOW);
    index = 3;
  }
      if (i >= 10 && i <= 20){                 // Помпа + Газ
      digitalWrite(relay12, LOW);  
      digitalWrite(relay11, LOW);
      index = 2;
    }
      if (i >= 20 && i <= 30){                 // Помпа + Електро
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
  if (i == 0) lcd.print("Dont Panic!");
  if (i == 1) lcd.print("EL/Pomp");
  if (i == 2) lcd.print("GAS/Pomp");
  if (i == 3) lcd.print("Morning");
}
