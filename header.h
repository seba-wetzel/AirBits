#ifndef _HEADER_H
#define _HEADER_H

#define MAX_DELAY 360
#define MIN_DELAY 30

#define MAX_PULSE 1000
#define MIN_PULSE 100

#define MAX_OUTPUTS sizeof(pines)
#define MIN_OUTPUTS 1
#define DRIVEN_STATE LOW


//Definiciones de pines
#define analogPin A0

typedef struct {
  int16_t delay;
  uint16_t pulse;
  uint8_t outputs;
} config_u;

typedef enum {
  right,
  up,
  down,
  left,
  select,
  none
} button_u;

typedef enum {
  hello,
  counter,
  menu_delay,
  menu_pulse,
  menu_outputs,
  menu_save
} states_u;

typedef enum {
  delayAddressB0,
  delayAddressB1,
  pulseAddressB0,
  pulseAddressB1,
  outputsAddress
} eepromAddress_u;

byte c1[8] = {B10000,  B10000,  B10000,  B10000,  B10000,  B10000,  B10000,  B10000};
byte c2[8] = {B11000,  B11000,  B11000,  B11000,  B11000,  B11000,  B11000,  B11000};
byte c3[8] = {B11100,  B11100,  B11100,  B11100,  B11100,  B11100,  B11100,  B11100};
byte c4[8] = {B11110,  B11110,  B11110,  B11110,  B11110,  B11110,  B11110,  B11110};
byte c5[8] = {B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111};

uint8_t pines [] = {0, 1, 2, 3, 11, 12, 13}; // Colocar aqui los nuevos pines de salida (solo quedan los analogicos libres)

//Funciones de ccontrol logico
uint8_t readConfigs(config_u*);
uint8_t saveConfigs(config_u*);
button_u readButton( uint8_t );

//Funcion de la maquina de estados
void estateMachine (states_u *);

//Funciones manejadoras de la pantalla
void printHello(void);
void printCounter(void);
void printDelayEntry(void);
void printPulseEntry(void);
void printOutputsEntry(void);
bool printSaveEntry(void);
uint16_t setTimeOut(config_u * );
void serialPrintConfigs(void);
void makeItBlink(uint8_t);


#endif
