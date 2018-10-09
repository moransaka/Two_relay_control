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

void Display(bool i);
void disabledAll();
void Tablet();
void startClock();
void initLCD();
void relay(short i);
bool elBoiler5_7(short i);

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
}

void loop()
{
  if (elBoiler5_7(times) == false) disabledAll(); // Отключить все кроме кнопки
  if (digitalRead(pin10) == HIGH){         // Запуск при удерживании кнопки
    relay(times);                          // Запускаем реле
  }
  startClock();                            //Запускаем часы
}

void startClock()
{
  if(millis() % 1000 == 0)                 // Проверка прошла ли одна секунда
  {
    lcd.setCursor(0, 1);                   // Переводим курсор на первую строку второго ряда
    lcd.print(time.gettime("H:i:s"));      // Получаем и выводим время на экран
    times = time.seconds;                  // Присваиваем время
    Display(index);
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
  if (i >= 7 && i <= 23){
    digitalWrite(relay12, HIGH);  
    digitalWrite(relay11, HIGH);
    index = 2;
  }
  else{
    digitalWrite(relay12, LOW);
    digitalWrite(relay11, LOW);
    index = 0;
  }
  
  if (i >= 23 && i <= 5){
    digitalWrite(relay11, HIGH);
    digitalWrite(relay21, HIGH);
    index = 1;
  }
  else{
    digitalWrite(relay11, LOW);
    digitalWrite(relay21, LOW);
    index = 0;
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
  switch (i)
  {
    case 0:
      lcd.print("DONT PANIC!");
    case 1:
      lcd.print("EL Boiler and Pomp");  
    case 2:
      lcd.print("GAS Boiler and Pomp");
  }
}

bool elBoiler5_7(short i)                 // Включить котел рано утром
{
  index = 0;
  if (i >= 5 && i <= 7){
    digitalWrite(relay11, HIGH);
    digitalWrite(relay21, HIGH);
    index = 1;
    return true;
  }
}