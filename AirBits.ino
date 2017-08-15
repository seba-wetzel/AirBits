#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "header.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //LCD pins


config_u configuraciones;
states_u estado = hello;
uint16_t contador;
uint8_t salidas= MIN_OUTPUTS;

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
}

void loop() {
  estateMachine(&estado);
  contador = setTimeOut(&configuraciones);

  if (contador == 0){
    makeItBlink(salidas);
    salidas++;
    if (salidas > configuraciones.outputs){
      salidas = MIN_OUTPUTS;
    }
    delay(1000);
  }
}

void estateMachine (states_u * estadoP) {

  switch (*estadoP) {
    case hello: printHello();
      delay(500);
      *estadoP = counter;
      break;

    case counter: printCounter();
      delay(500);
      if (readButton(0) == right ) {
        *estadoP = menu_delay;
      }
      break;

    case menu_delay: printDelayEntry();
      delay(500);

      if (readButton(0) == left ) {
        *estadoP = counter;
      }
      if (readButton(0) == right ) {
        *estadoP = menu_pulse;
      }
      break;

    case menu_pulse: printPulseEntry();
      delay(500);
      if (readButton(0) == left ) {
        *estadoP = menu_delay;
      }
      if (readButton(0) == right ) {
        *estadoP = menu_outputs;
      }

      break;

    case menu_outputs: printOutputsEntry();
      delay(500);

      if (readButton(0) == left ) {
        *estadoP = menu_pulse;
      }
      if (readButton(0) == right ) {
        *estadoP = menu_save;
      }
      break;

    case menu_save: bool option = printSaveEntry();
      delay(500);
      if (readButton(0) == left ) {
        *estadoP = menu_outputs;
      }
      if (readButton(0) == select ) {
        if (option) {
          saveConfigs(&configuraciones);
          *estadoP = hello;
          lcd.clear();
          delay(250);
          asm("jmp 0x0000");
        }
        else {
          readConfigs(&configuraciones);
          *estadoP = hello;
          lcd.clear();
          delay(250);
          asm("jmp 0x0000");
        }
      }
      break;
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
  uint16_t adc_key_in = analogRead(analogPin);  //Leemos el pin analogico

  if (adc_key_in > 1000) return none;
  if (adc_key_in < 50)   return right;
  if (adc_key_in < 195)  return  up;
  if (adc_key_in < 380)  return  down;
  if (adc_key_in < 555)  return  left;
  if (adc_key_in < 790)  return select;
}

void makeItBlink (uint8_t salida){
  digitalWrite(pines [salida-1], HIGH);
  delay(configuraciones.pulse);
   digitalWrite(pines [salida-1], LOW);
}

//Funciones de la pantalla (menus)
void printHello(void) {
  lcd.clear();
  lcd.setCursor(3, 0);  //Cursor culumna/linea
  lcd.print("AirBlobs");
  lcd.setCursor(1, 1);  //Cursor culumna/linea
  lcd.print("by Electro-AR");
  delay(500);
}

void printCounter(void) {
  lcd.clear();
  lcd.setCursor(0, 0);  //Cursor culumna/linea
  lcd.print("Descarga en:");
  lcd.setCursor(12, 0);
  lcd.print(contador / 60 );
  lcd.setCursor(13, 0);
  lcd.print(":");
  lcd.setCursor(14, 0);
  if (contador % 60 == 0) {
    lcd.print("00");
  }
  else {
    lcd.print(contador % 60 );
  }
  lcd.setCursor(0, 1);  //Cursor culumna/linea
  lcd.print("en salida n:");
  lcd.print(salidas);
}

void printDelayEntry(void) {
  static uint16_t counter = configuraciones.delay;
  lcd.clear();
  lcd.noBlink();
  lcd.setCursor(0, 0);
  lcd.print("Tiempo entre");
  lcd.setCursor(0, 1);
  lcd.print("disparos");

  if (readButton(0) == up && counter < MAX_DELAY) {
    counter = counter + 30;
  }
  if (readButton(0) == down && counter > MIN_DELAY ) {
    counter = counter - 30;
  }
  configuraciones.delay = counter;
  lcd.setCursor(10, 1);
  lcd.print(configuraciones.delay / 60 );
  lcd.setCursor(11, 1);
  lcd.print(":");
  lcd.setCursor(12, 1);
  if (configuraciones.delay % 60 == 0) {
    lcd.print("00");
  }
  else {
    lcd.print(configuraciones.delay % 60 );
  }
}

void printPulseEntry(void) {
  static uint16_t counter = configuraciones.pulse;
  lcd.clear();
  lcd.noBlink();
  lcd.setCursor(0, 0);
  lcd.print("Duracion del");
  lcd.setCursor(0, 1);
  lcd.print("disparo:");

  if (readButton(0) == up && counter < MAX_PULSE) {
    counter = counter + 10;
  }
  if (readButton(0) == down && counter > MIN_PULSE ) {
    counter = counter - 10;
  }

  configuraciones.pulse = counter;
  lcd.setCursor(8, 1);
  lcd.print(configuraciones.pulse );
  lcd.setCursor(12, 1);
  lcd.print("ms");
  delay(250);
}

void printOutputsEntry(void) {
  static uint8_t counter = configuraciones.outputs;
  lcd.clear();
  lcd.setCursor(0, 0);  //Cursor culumna/linea
  lcd.print("Cantantidad de");
  lcd.setCursor(0, 1);
  lcd.print("salidas:");

  if (readButton(0) == up && counter < MAX_OUTPUTS) {
    counter++;

  }
  if (readButton(0) == down && counter > MIN_OUTPUTS ) {
    counter--;
  }
  configuraciones.outputs = counter;
  lcd.setCursor(10, 1);
  lcd.print(configuraciones.outputs );
  delay (500);
}

bool printSaveEntry(void) {
  static bool select = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Guardar conf?");
  lcd.setCursor(0, 1);
  lcd.print("Si{ }   No{ }");
  if (readButton(0) == up) {
    select = true;

  }
  if (readButton(0) == down) {
    select = false;
  }
  if (select) lcd.setCursor(3, 1);
  else lcd.setCursor(11, 1);
  lcd.print("*");
  return select;
}

//Funcion para recolectar los datos de la EEPROM y enviarlos por el serie
void serialPrintConfigs (void) {
  Serial.println(configuraciones.delay);
  Serial.println(configuraciones.pulse);
  Serial.println(configuraciones.outputs);
}

uint16_t setTimeOut(config_u * tempP) {
  static int16_t interval = tempP->delay;
  static uint32_t previousMillis;
  uint32_t currentMillis;

  currentMillis = millis();
  if ((currentMillis) - (previousMillis) > 1000) {
    previousMillis = currentMillis;
    if (interval > 0) {
      interval = interval - 1;
    }
    else {
      interval = tempP->delay;
    }
  }
  Serial.println(interval);
  return interval;
}

