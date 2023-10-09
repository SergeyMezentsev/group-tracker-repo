#include <SPI.h>  // Подключаем библиотеку для работы с SPI-интерфейсом
#include <nRF24L01.h> // Подключаем файл конфигурации из библиотеки RF24
#include <RF24.h> // Подключаем библиотеку для работа для работы с модулем NRF24L01

#define ADDR 1
#define PIN_BUZZER 2  // Pin for buzzer
#define PIN_LED 3  // Номер пина Arduino, к которому подключен светодиод
#define PIN_CE  10  // Номер пина Arduino, к которому подключен вывод CE радиомодуля
#define PIN_CSN 9 // Номер пина Arduino, к которому подключен вывод CSN радиомодуля
RF24 radio(PIN_CE, PIN_CSN); // Создаём объект radio с указанием выводов CE и CSN

//int potValue[1]; // Создаём массив для приёма значений потенциометра

long timeBorder, timeToReceive;
//uint8_t payLoad = ADDR;

void setupRadio(){
  radio.begin();  // Инициализация модуля NRF24L01
  radio.setDataRate (RF24_1MBPS); // Скорость обмена данными 1 Мбит/сек
  radio.setPALevel(RF24_PA_HIGH); // Выбираем низкую мощность передатчика
  radio.setPayloadSize(sizeof(uint8_t));  // float datatype occupies 4 bytes

  
  
  //radio.openWritingPipe("host"); // Открываем трубу с уникальным ID
  //radio.openReadingPipe("child");
  //radio.setChannel(1); // Обмен данными будет вестись на пятом канале (2,405 ГГц)
}


void setRadioChannel(uint8_t ch){
  radio.setChannel(ch); // Обмен данными будет вестись на пятом канале (2,405 ГГц)
  radio.openWritingPipe(2); // Открываем трубу с уникальным ID
  radio.openReadingPipe(1, 1);
  radio.startListening();
  
  Serial.print("Set channel to: ");
  Serial.println(ch);
}




void setup() {
  pinMode(PIN_LED, OUTPUT); // Настраиваем на выход пин светодиода
  pinMode(PIN_BUZZER, OUTPUT);
  setupRadio();
  setRadioChannel(ADDR);
  Serial.begin(9600);
}

void loop() {

  timeToReceive = 10000;
  timeBorder = millis();
  while(timeToReceive > 0){
  
    if (radio.available()){              // is there a payload? get the pipe number that recieved it
        uint8_t gotLoad;
        radio.read(&gotLoad, sizeof(uint8_t));             // fetch payload from FIFO

        if (gotLoad == 0){
          Serial.print("Got number: ");
          Serial.println(gotLoad);

          radio.stopListening();
          delay(500);
          uint8_t payLoad = ADDR;
          radio.write(&payLoad, sizeof(uint8_t));
          digitalWrite(PIN_LED, LOW);
          digitalWrite(PIN_BUZZER, LOW);
          //delay(200);
          radio.startListening();
          break;
        }else{
          Serial.println("gotLoad is invalid");
        }
      }


    if (millis() - timeBorder > 70){
      timeToReceive -= 70;
      timeBorder = millis();
    }
    
  }


  if (timeToReceive <= 0){
    digitalWrite(PIN_LED, HIGH);
    digitalWrite(PIN_BUZZER, HIGH);
  }
  
}
