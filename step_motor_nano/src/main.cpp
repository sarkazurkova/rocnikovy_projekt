#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Stepper.h>                    //load library
#include <IRremote.hpp>

#define STEPS 100                       //establish number of steps
#define RX 11
#define TX 10
#define LED 9
#define IRs 2
#define IRDioda 8
#define BTN 13

Stepper stepper(STEPS, 4, 6, 5, 7);
SoftwareSerial bluetooth(TX, RX);

const int oneStep = 10;
bool state;
bool ledState;

void setup() {
  ledState = LOW;
  pinMode(LED, OUTPUT);
  pinMode(IRs, INPUT);
  pinMode(BTN, INPUT_PULLUP);
  stepper.setSpeed(60);
  IrReceiver.begin(IRDioda, ENABLE_LED_FEEDBACK);
  bluetooth.begin(9600);
  Serial.begin(9600);
  bluetooth.println("Arduino zapnuto, test Bluetooth..");

}


/*funkce podle bool dir rozhodne o směru rotace a udělá jeden krok*/
void motorStep(bool dir){
  if(dir){
    stepper.step(oneStep);
    digitalWrite(LED, !ledState);
  }else{
    stepper.step(-oneStep);
      digitalWrite(LED, !ledState);

  }
}


/*podle přijmutých dat z bluetooth rozhodne která funkce proběhne*/
void signalB(byte BluetoothData){
  switch (BluetoothData){
    case '0':
        bluetooth.println("Value 0");
        while(digitalRead(IRs) || digitalRead(BTN)){
          motorStep(true);
        }
        break;
      case '1':
        bluetooth.println("Value 1");
        while(digitalRead(IRs)){
          motorStep(false);
        }
        break;
      case '\r':
        // přesun na začátek řádku - znak CR
        break;
      case '\n':
        // odřádkování - znak LF
        break;
      default:
        // v případě přijetí ostatních znaků
        // vytiskneme informaci o neznámé zprávě
        bluetooth.println("Neznamy prikaz.");
        Serial.println(BluetoothData);
  }
  ledState = LOW;
  digitalWrite(LED, ledState);
}

void loop() {  
  byte BluetoothData;
  if (IrReceiver.decode()){
    unsigned long receivedValue = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(receivedValue, HEX);
    bluetooth.println("Byl prijat kod.");
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.resume();
  }
  if (bluetooth.available() > 0) {
    // načtení prvního znaku ve frontě do proměnné
    BluetoothData=bluetooth.read();
    // dekódování přijatého znaku
    signalB(BluetoothData);
    
    }
  if (digitalRead(BTN)){
    bluetooth.println("BTN");
  }
  
}