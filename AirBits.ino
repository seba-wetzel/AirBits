#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "header.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //LCD pins


config_u configuraciones;
states_u estado = hello;
uint8_t analogPin = 0;


void setup() {

  Serial.begin(9600);
  lcd.begin(16, 2); //LCD init
  lcd.createChar(0, c1);
  lcd.createChar(1, c2);
  lcd.createChar(2, c3);
  lcd.createChar(3, c4);
  lcd.createChar(4, c5);
  readConfigs(&configuraciones);
  serialPrintConfigs();

  estateMachine(&estado);

}

void loop() {



  for (;;) {

  }

}
void estateMachine (states_u * estadoP) {
  while (true) {
    switch (*estadoP) {
      case hello:
        printHello();
        delay(500);
        *estadoP = counter;
        break;

      case counter:
        printCounter();
        delay(750);
        *estadoP = menu_entry;
        break;

      case menu_entry:
        printMenuEntry();
        delay(750);
        *estadoP = menu_delay;
        break;

      case menu_delay:
        printDelayEntry();
        delay(750);
        *estadoP = menu_pulse;
        break;

      case menu_pulse:
        printPulseEntry();
        delay(750);
        *estadoP = menu_outputs;
        break;

      case menu_outputs: printOutputsEntry();
        delay(750);
        *estadoP = menu_save;
        break;

      case menu_save: printSaveEntry();
        delay(500);
        break;
    }
  }

}

//Funciones de control logico
uint8_t readConfigs(config_u *temps) {
  temps->delay   = EEPROM.read(delayAddress);
  temps->pulse   = EEPROM.read(pulseAddress);
  temps->outputs = EEPROM.read(outputsAddress);

}

uint8_t saveConfigs(config_u *temps) {
  EEPROM.write(delayAddress, temps->delay);
  EEPROM.write(pulseAddress, temps->pulse);
  EEPROM.write(outputsAddress, temps-> outputs);
}

button_u readButton(uint8_t analogPin) {
  uint8_t adc_key_in = analogRead(analogPin);  //Leemos el pin analogico
  if (adc_key_in > 1000) return none;
  if (adc_key_in < 50)   return right;
  if (adc_key_in < 250)  return  up;
  if (adc_key_in < 450)  return  down;
  if (adc_key_in < 650)  return  left;
  if (adc_key_in < 850)  return select;
}

//Funciones de la pantalla (menus)
void printHello(void) {
  lcd.setCursor(4, 0);  //Cursor culumna/linea
  lcd.print("AirBlobs");
  lcd.setCursor(2, 1);  //Cursor culumna/linea
  lcd.print("by Electro-AR");
  delay(500);
}

void printCounter(void) {
  lcd.clear();
  lcd.setCursor(0, 0);  //Cursor culumna/linea
  lcd.print("Descarga en:");
  lcd.setCursor(0, 1);  //Cursor culumna/linea
  lcd.print("en salida n:");
}

void printMenuEntry(void) {
  lcd.clear();
  lcd.blink();
  lcd.setCursor(0, 0);
  lcd.print("delay{ }");
  lcd.setCursor(8, 0);
  lcd.print("pulso{ }");
  lcd.setCursor(0, 1);
  lcd.print("salidas{ }");
}

void printDelayEntry(void) {
  lcd.clear();
  lcd.noBlink();
  lcd.setCursor(0, 0);
  lcd.print("Delay");
  lcd.setCursor(1, 1);

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 5; j++) {
      lcd.setCursor(i, 1);
      lcd.write(j);
      delay(15);
    }
  }
}

void printPulseEntry(void) {
  lcd.clear();
  lcd.noBlink();
  lcd.setCursor(0, 0);
  lcd.print("Pulse");
  lcd.setCursor(1, 1);

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 5; j++) {
      lcd.setCursor(i, 1);
      lcd.write(j);
      delay(15);
    }
  }
}

void printOutputsEntry(void) {
  lcd.clear();
  lcd.setCursor(0, 0);  //Cursor culumna/linea
  lcd.print("Cantantidad de");
  lcd.setCursor(0, 1);
  lcd.print("salidas:");
  for (int i = MIN_OUTPUTS; i <= MAX_OUTPUTS; i++) {
    lcd.setCursor(10, 1);
    lcd.print(i);
    delay (500);
  }
}

void printSaveEntry(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Guardar conf?");
  lcd.setCursor(0, 1);
  lcd.print("Si{ }   No{ }");
}


//Funcion para recolectar los datos de la EEPROM y enviarlos por el serie
void serialPrintConfigs (void) {
  Serial.println(configuraciones.delay);
  Serial.println(configuraciones.pulse);
  Serial.println(configuraciones.outputs);
}

