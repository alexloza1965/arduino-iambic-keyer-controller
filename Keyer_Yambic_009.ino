
//  Автоматический телеграфный ключ Iambic B, Iambic A
//  Arduino Nano (или любая другая плата)
//  VER 2025.03.08


#define DOT 3              // D5 - вход Точка / Dot / Dit - можно поменять местами
#define DASH 2             // D6 - вход Тире / Dash / Dah - можно поменять местами

#define OUT 4              // D4 - выход ключа Key OUT
#define TX 5               // D2 - выход TX
#define YEL 11             // D11 - желтый светодиод Key OUT
#define RED 12             // D12 - красный светодиод TX
#define BUZZER 8           // D8 - выход звукового сигнала самоконтроля

#define TONE A0            // A0 - вход регулятора тона (потенциометр 20К)
#define SPEED A1           // A1 - вход регулятора скорости (потенциометр 20К)



unsigned long t11, t22;

long t0, t1, t2, t3, t4, deltaT;

unsigned long ton, zumTon, spd, ddot, ddash, rat, pause, dt0, dtConst;

boolean out, tx, yaType;

boolean dot, dash, dotBlock, dashBlock, dotMem, dashMem;



void setup() {
  
  // initialize digital pin LED_BUILTIN as an output.

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TONE, INPUT);
  pinMode(SPEED, INPUT);
  pinMode(DOT, INPUT_PULLUP);         // Подтянутый к питанию вход
  pinMode(DASH, INPUT_PULLUP);        // Подтянутый к питанию вход
  pinMode(OUT, OUTPUT);
  pinMode(TX, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(YEL, OUTPUT);


  
  t0 = 0;                     // [...t0] - начало цикла формирования знака  OUT=0, TX=0
  t1 = 0;                     // [t0-t1] - начальная задержка  OUT=0, TX=1
  t2 = 0;                     // [t1-t2] - знак точка или тире  OUT=1, TX=1
  t3 = 0;                     // [t2-t3] - пауза в конце знака   OUT=0, TX=1
  t4 = 0;                     // [t2-t4] - продолжение TX после завершения манипуляции  OUT=0, TX=1
                              // [t4...] - ожидание манипуляции  OUT=0, TX=0

  yaType = 1;                 // Iambic A = 0, Iambic B = 1. Я предпочитаю Yambic B !!!
  spd = 0;                    // Рабочая переменная - текущая скорость
  rat = 3;                    // Соотношение тире/точка = 3
  ddot = 1E3 * 50;            // Начальная длительность точки, 50 мс (~30 WPM)
  ddash = rat * ddot;         // Начальная длительность тире
  pause = 1E6 * 0.8;          // Длительность сигнала TX после завершения манипуляции, 0.8 сек
  dtConst = 1E3 * 50;         // Задержка первого знака. Cигнал TX опережеает OUT на 50 мс
  dt0 = dtConst;              // Задержка первого знака. Рабочая переменная
  zumTon = 1000;              // Начальный тон зуммера в Гц


  digitalWrite(OUT, LOW);
  digitalWrite(TX, LOW);

  out, tx = 0;
  dot, dash = 0;
  dotBlock, dashBlock = 0;
  dotMem, dashMem = 0;

  delay(5);                   // Задержка 5 мс, что бы успели зарядиться емкости, шунтирующие входы DIT, DASH

}


void loop() {


  t11 = micros();

//  t11 &= 0x00ffffff;              // Проверка работы при переполнении micros() - 32 бита, 71.58 мин; укорачиваем до 24 бит, 16.77 сек

  deltaT = t11 - t22;

//  deltaT &= 0x00ffffff;           // Проверка работы при переполнении micros()

  t22 = t11;

//  if ( (t11 &= 0x00800000) == 0)  digitalWrite(LED_BUILTIN, LOW); else digitalWrite(LED_BUILTIN, HIGH);    // Тестовый индикатор цикла micros()



//  Считываем сигналы телеграфного манипулятора
  dot = !digitalRead(DOT);
  dash = !digitalRead(DASH);


//  Отрабатываем данные из памяти знака (если есть): точка
  if ( dotMem && !dotBlock && !dashBlock && ( dot |= yaType ) ) {
    dotBlock = 1; dotMem = 0;
    t0 = 0; t1 = t0 + dt0; t2 = t1 + ddot; t3 = t2 + ddot; t4 = t2 + pause;
  }

//  Отрабатываем данные из памяти знака (если есть): тире
  if ( dashMem && !dashBlock && !dotBlock  && ( dash |= yaType ) ) {
    dashBlock = 1; dashMem = 0;
    t0 = 0; t1 = t0 + dt0; t2 = t1 + ddash; t3 = t2 + ddot; t4 = t2 + pause;
  }

//  Обрабатываем нажатие точки
  if ( dot && !dotBlock && !dashBlock ) {
    dotBlock = 1;
    t0 = 0; t1 = t0 + dt0; t2 = t1 + ddot; t3 = t2 + ddot; t4 = t2 + pause; dt0 = 0;
  }

//  Обрабатываем нажатие тире
  if ( dash && !dashBlock && !dotBlock ) {
    dashBlock = 1;
    t0 = 0; t1 = t0 + dt0; t2 = t1 + ddash; t3 = t2 + ddot; t4 = t2 + pause; dt0 = 0;
  }

//  Записываем данные в память противоположного знака
  if ( dot && dashBlock ) dotMem = 1;
  if ( dash && dotBlock ) dashMem = 1;



//  ======== Формируем текущий знак по частям: точка/тире + пауза ========

// Задержка OUT для опережающего сигнала TX
  if ( t0 <= 0 && t1 > 0 ) {
    t1 = t1 - deltaT; t2 = t2 - deltaT; t3 = t3 - deltaT; t4 = t4 - deltaT;
    out = 0; tx = 1;
  }

// Знак точка или тире
  if ( t1 <= 0 && t2 > 0 ) {
    t2 = t2 - deltaT; t3 = t3 - deltaT; t4 = t4 - deltaT;
    out = 1; tx = 1;
  }

// Пауза
  if ( t2 <= 0 && t3 > 0 ) {
    t3 = t3 - deltaT; t4 = t4 - deltaT;
    out = 0; tx = 1;
  }

// Сброс признаков блокировки точки/тире
  if ( t3 <= 0 && t4 > 0 ) {
    t4 = t4 - deltaT;
    dotBlock = 0; dashBlock = 0;
  }

  // Сброс всех сигналов, признаков, установка начальной задержки
  if ( t4 <= 0 ) {
    out = 0; tx = 0; dt0 = dtConst;
    dotMem = 0; dashMem = 0;
  }



//  Выводим сформированные сигналы

  digitalWrite(OUT, out);
  digitalWrite(YEL, out);
  digitalWrite(TX, tx);
  digitalWrite(RED, tx);
  digitalWrite(LED_BUILTIN, out);



//  Регулируем частоту звукового сигнала самоконтроля

  ton = analogRead(TONE);                         // Прямой потенциометр
// ton = 1023 - analogRead(TONE);                 // Инверсный потенциометр
  zumTon = 200 + 4 * ton;


//  Выводим звуковой сигнал самокотроля через цифровой выход BUZZER

  if (out) tone(BUZZER, zumTon); else noTone(BUZZER);



//  Регулируем скорость работы, задаем длительной точки, считаывая напряжение с аналогового входа SPEED

  spd = 1023 - analogRead(SPEED);                 // Прямой потенциометр
// spd = analogRead(SPEED);                       // Инверсный потенциометр

  if (spd >=0   && spd < 256)  ddot = 1E3 * ( 24  + ( spd - 0 ) / 32 );
  if (spd >=256 && spd < 512)  ddot = 1E3 * ( 32  + ( spd - 256 ) / 16 );
  if (spd >=512 && spd < 768)  ddot = 1E3 * ( 48  + ( spd - 512 ) / 4 );
  if (spd >=768 && spd < 1024) ddot = 1E3 * ( 112 + ( spd - 768 ) / 2 );

  ddash = rat * ddot;                             // Вычисляем длительность тире


}
