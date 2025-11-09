#include "GyverButton.h"
#include <Wire.h> // библиотека
#include <Keypad.h>
#include <Q2HX711.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
//GyverHX711 sensor(50, 51, HX_GAIN64_A); // первый сенсор
//Filter fil;
Q2HX711 hx4(A3, A4);  //комментарий
Q2HX711 hx3(A5, A6);
Q2HX711 hx2(A7, A8);
Q2HX711 hx1(A9, A10);
//int ves1=hx1.read()/1000;
//int ves2=(hx2.read()/1000)+124;
//int ves3=(hx3.read()/1000)+436;
//int ves4=(hx4.read()/1000)+155;
Q2HX711* sensors[]={&hx1,&hx2,&hx3,&hx4};
bool pouring[]={false,false,false,false};
int add[]={117,63,372,97};
int count=0;
long stabelweight(Q2HX711* sensor, int readingCount = 10){
  long sum=0;
  for (int i =0; i<readingCount; i++) {
    //Serial.println("Идут выычисления");
    sum += sensors[count] -> read();
    delay(20);
  }
  return sum /readingCount;
}

#define BUTTON1_PIN 2
#define BUTTON2_PIN 6
#define BUTTON3_PIN 7
#define klapan1_PIN 3   //water
#define klapan2_PIN 4   //juice
#define klapan3_PIN 5   //green
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей
//GButton butt1 (BUTTON1_PIN);
//GButton butt2 (BUTTON2_PIN);
//GButton butt3 (BUTTON3_PIN);
const byte ROWS = 4; // число строк клавиатуры
const byte COLS = 4; // число столбцов клавиатуры
char key;
char keys[ROWS][COLS] = {
      {'1','2','3','A'},
      {'4','5','6','B'},
      {'7','8','9','C'},
      {'*','0','#','D'}
};
byte rowPins[ROWS] = {6, 7, 8, 9}; // к каким выводам подключаем управление строками
byte colPins[COLS] = {10, 11, 12, 13}; // к каким выводам подключаем управление столбцами
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo servo1; //кран
Servo servo2; //колесо
uint32_t tmr;
boolean flag;



void setup() {
  Serial.begin(9600);
  servo1.attach(28);
  lcd.init(); // Инициализируем экран
  lcd.backlight();
  //butt1.setType(HIGH_PULL);
  pinMode(klapan1_PIN,OUTPUT);
  digitalWrite(klapan1_PIN,HIGH);
  pinMode(klapan2_PIN,OUTPUT);
  digitalWrite(klapan2_PIN,HIGH);
  pinMode(klapan3_PIN,OUTPUT);
  digitalWrite(klapan3_PIN,HIGH);
  lcd.init();                     
  lcd.backlight(); // Включаем подсветку дисплея
  lcd.setCursor(0, 0);
  //sensor.tare(); //калибровка нуля
  servo1.attach(A0);    //sifon
  servo2.attach(2);  // whiil
  servo1.write(80);    
}


void loop() {
  //fil.tick();   // фильтр
  //sensor.setOffset(-fil.get());

  Serial.println("Выберите количество напитков");
  lcd.clear();                  
  lcd.print("Enter the number");
  lcd.setCursor(0,1);
  lcd.print("of drinks");
  lcd.setCursor(0,0);
  int numberglass=0;
  bool flag1=false;
  while (flag1==false){
    key=keypad.getKey();
    if (key != NO_KEY) {      
      if (key == '1') { 
        numberglass=1;
        flag1==true;
        break;               
      }
      else if (key == '2') {        
        numberglass = 2;  
        flag1==true;
        break;         
      }
      else if (key == '3') {        
        numberglass = 3;  
        flag1==true;
        break;           
      }
      else if (key == '4') {        
        numberglass = 4;
        flag1==true;
        break;           
      }
      else {        
        lcd.clear();               
        lcd.print("Uncorrect input");
        lcd.setCursor(0,1);
        lcd.print(String("Enter correct number"));
        lcd.setCursor(0,0);
        Serial.println("Выберите другое количество");
        flag1==false;
      }
    }
  }

  lcd.clear();              
  lcd.print("Number of drinks:");
  lcd.setCursor(0,1);
  lcd.print(String(numberglass));
  lcd.setCursor(0,0);

  Serial.println("Вами выбрано "+String(numberglass)+" напитка");
  delay(5000);

  //Serial.println("Выберите рецепты напитков");
  int drink=0;
  int count=0;
  int numberdrink=1;
  int menuarr[] = {0,0,0,0,};
  //for (int i=0;i<=numberglass; i++)
  while (count< numberglass) {
  //выбор напитка
    count++;
    delay(100);
    lcd.clear();              
    lcd.print("Choose a drink "+String(numberdrink));
    Serial.println("Выберите рецепт для "+String(numberdrink)+" напитка");
    while(1){
      key=keypad.getKey();
      if (key != NO_KEY) {        // если была нажата
        if (key == '1') { 
          drink=1;
          Serial.println("Выбрали 1 напиток Газированная вода");
          numberdrink=numberdrink+1;
          menuarr[count-1]=1;
          break;
        }
        else if (key == '2') {        
         drink = 2;  
          Serial.println("Выбрали 2 напиток Мятный сироп");
          numberdrink=numberdrink+1; 
          menuarr[count-1]=2;
          break;    
        }
        else if (key == '3') {        
          drink = 3;  
          Serial.println("Выбрали 3 напиток");
          numberdrink=numberdrink+1;
          menuarr[count-1]=3;
          break;       
        }
        else if (key == '4') {        
          drink = 4;  
          Serial.println("Выбрали 4 напиток");
          numberdrink=numberdrink+1; 
          menuarr[count-1]=4;
          break;  
        }
        else if (key == '5') {        
          drink = 5;  
          Serial.println("Выбрали 5 напиток");
          numberdrink=numberdrink+1;
          menuarr[count-1]=5;
          break;  
        }
        else if (key == '6') {        
          drink = 6;  
          Serial.println("Выбрали 6 напиток");
          numberdrink=numberdrink+1;
          menuarr[count-1]=6;
          break;
        }
       else {        
        lcd.clear();               

        lcd.print("Uncorrect input");
        break;
        }
      }
    }
   delay(3000);
  }
  //Serial.println(menuarr[0]);
  //Serial.println(menuarr[1]);
  //Serial.println(menuarr[2]);
  //Serial.println(menuarr[3]);
  //Serial.println("Выбор рецептов закончен");
  
  while(1){
      key=keypad.getKey();
      if (key != NO_KEY) {        // если была нажата
        if (key == 'A') { 
          lcd.clear();
  lcd.print("Please waite");
  Serial.println("Поехал круг");
    servo2.write(90); // ставим стаканы
    delay(4000); 
    servo2.write(100); // перемещаемся в зону налива
    delay(2000); 
    servo2.write(90); // наливается
    delay(1000); 
  count=0;
  int pos=80;
  int angle=155;
  for(pos = 80; pos<angle; pos+=1)
    {                                
    servo1.write(pos); 
    delay(20); 
    } 
  while (count<= numberglass) {
    count++;
  //Serial.println(count);   
     if (numberglass != 0) {
      servo1.write(pos);
      delay(3000);
      switch (menuarr[count-1]) {
        case 1:
          drink_sparkling();
          delay(2000);
          break;
        case 2:
          drink_mint();
          delay(2000);
          break;
        case 3:
          drink_juice();
          delay(2000);
          break;
        case 4:
          cocktail_mint();
          delay(2000);
          break;
        case 5:
          cocktail_orange();
          delay(2000);
          break;
        case 6:
          cocktail_triple();
          delay(2000);
          break;
        
        }
      }
      
      switch (count){
        case 1:
          angle=95;
          break;
        case 2:
          angle=49;
          break;
        case 3:
          angle=1;
          break;
      }
      delay(3000);
      Serial.println(menuarr[count-1]); 
      if (menuarr[count-1]=!0){
      for(pos = pos; pos>angle; pos-=1)
        {                                
        servo1.write(pos); 
        delay(20); 
        } 
      }
    }
    for(pos = pos; pos<79; pos+=1)
    {                                
    servo1.write(pos); 
    delay(20); 
    } 
    servo2.write(80); // в зону выдачи
    delay(1600); 
    servo2.write(90); // забираем стаканчики
    delay(4000); // 
    break;
      }

      }
  }
  lcd.clear();
}
/*if (ves1,ves2,ves3,ves4==8515):{
  if (menuarr[count-1] == 1): {
    Serial.println(hx1.read()/1000); //до 8565
  }
  else if (menuarr[count-1] == 2) {
    Serial.println(hx1.read()/1000); //до 8525
  }
  else if (menuarr[count-1] == 3) {
    Serial.println(hx1.read()/1000); //до 8555
  }
  else if (menuarr[count-1] == 4) {
    Serial.println(hx1.read()/1000); //до 8615    в. до 8595 + м. до 8615
  }
  else if (menuarr[count-1] == 5) {
    Serial.println(hx1.read()/1000); //до 8595    в. 8545 + а. 8595
  }
  else if (menuarr[count-1] == 6) {
    Serial.println(hx1.read()/1000); //до 8605    в. 8550 + а. 8595 + м. 8605
  }
}
*/
//1. Газированная вода (50 мл.)
void drink_sparkling(){ 
    //Водичка
    Serial.println("Сейчас готовится Газированная вода");
    delay(500);
    digitalWrite(klapan1_PIN,LOW);
    Serial.println("Водичка пошла");
    delay(17000);
    digitalWrite(klapan1_PIN,HIGH);
    Serial.println("Водичка не пошла");
}

//2. Мятный сироп (10 мл.)
void drink_mint(){
    //Тархун
    Serial.println("Сейчас готовится Мятный сироп");
    delay(500); 
    digitalWrite(klapan3_PIN,LOW);
    Serial.println("Тархун пошел");
    delay(15645);
    digitalWrite(klapan3_PIN,HIGH);
    Serial.println("Тархун не пошел");
}

//3. Апельсиновый сок (40 мл.)
void drink_juice(){
    //Сок
    delay(500);
    Serial.println("Сейчас готовится Апельсиновый сок");
    digitalWrite(klapan2_PIN,LOW);
    Serial.println("Сок пошел");
    delay(14400);
    digitalWrite(klapan2_PIN,HIGH);
    Serial.println("Сок не пошел");
}

//4. Лимонад “Мятный” (80 мл. газированной воды + 20 мл. мятного сиропа)
void cocktail_mint(){ 
    //Водичка
    Serial.println("Сейчас готовится лимонад Мятный");
    delay(500);
    digitalWrite(klapan1_PIN,LOW);
    Serial.println("Водичка пошла");
    delay(28800);
    digitalWrite(klapan1_PIN,HIGH);
    Serial.println("Водичка не пошла");
    //Тархунчик
    digitalWrite(klapan3_PIN,LOW);
    Serial.println("Тархун пошел");
    delay(7200);
    digitalWrite(klapan3_PIN,HIGH);
    Serial.println("Тархун не пошел");
}

//5. Лимонад “Заводной апельсин” (30 мл. газированной воды + 50 мл. апельсинового сока).
void cocktail_orange(){
    //Водичка
    Serial.println("Сейчас готовится Заводной апельсин");
    delay(500);
    digitalWrite(klapan1_PIN,LOW);
    Serial.println("Водичка пошла");
    delay(10800);
    digitalWrite(klapan1_PIN,HIGH);
    Serial.println("Водичка не пошла");
    //Сок
    digitalWrite(klapan2_PIN,LOW);
    Serial.println("Сок пошел");
    delay(18000);
    digitalWrite(klapan2_PIN,HIGH);
    Serial.println("Сок не пошел");
}
///6. Лимонад ‘Тройной” (35 мл. газированной воды + 45 мл. апельсинового сока + 10 мл. мятного сиропа)
void cocktail_triple(){ 
    //Водичка
    Serial.println("Сейчас готовится Тройной");
    delay(500);
    digitalWrite(klapan1_PIN,LOW);
    Serial.println("Водичка пошла");
    delay(12600);
    digitalWrite(klapan1_PIN,HIGH);
    Serial.println("Водичка не пошла");
    //Сок
    digitalWrite(klapan2_PIN,LOW);
    Serial.println("Сок пошел");
    delay(16200);
    digitalWrite(klapan2_PIN,HIGH);
    Serial.println("Сок не пошел");
    //Тархунчик
    digitalWrite(klapan3_PIN,LOW);
    Serial.println("Тархун пошел");
    delay(3600);
    digitalWrite(klapan3_PIN,HIGH);
    Serial.println("Тархун не пошел");
}

