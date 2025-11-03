#include "PT6961.h"


// Pin 13 = CS
// Pin 15 = CLK
// Pin 14 = DIN
PT6961 LED(14, 15, 13);


void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY); //только отправка
  
  LED.initDisplay();
}



// Код в блоке loop() будет повторяться вновь и вновь:
void loop() {
  uint8_t key;

  //count through 0-9,a-f
     for(int i=0; i<18; i++)
     {
          key=LED.readKey();
          LED.sendDigits(i,i,i,key,0);
          Serial.println(i);
          delay(500);
     }
}
