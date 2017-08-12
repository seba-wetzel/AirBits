#include "header.h"

config_u configuraciones;
uint8_t analogPin = 0;


void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

uint8_t readConfigs(config_u *temps) {

}

uint8_t saveConfigs(config_u *temp) {

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

