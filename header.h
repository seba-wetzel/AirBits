#ifndef _HEADER_H
#define _HEADER_H

#define MAX_DELAY 360
#define MIN_DELAY 30

#define MAX_PULSE 1000
#define MIN_PULSE 100

#define MAX_OUTPUTS 7
#define MIN_OUTPUTS 1

typedef struct {
  uint8_t delay;
  uint8_t pulse;
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
  menu_entry,
  menu_delay,
  menu_pulse,
  menu_outputs,
  menu_save
} states_u;

typedef enum{
  delayAddress,
  pulseAddress,
  outputsAddress
} eepromAddress_u;

byte c1[8] = {B10000,  B10000,  B10000,  B10000,  B10000,  B10000,  B10000,  B10000};
byte c2[8] = {B11000,  B11000,  B11000,  B11000,  B11000,  B11000,  B11000,  B11000};
byte c3[8] = {B11100,  B11100,  B11100,  B11100,  B11100,  B11100,  B11100,  B11100};
byte c4[8] = {B11110,  B11110,  B11110,  B11110,  B11110,  B11110,  B11110,  B11110};
byte c5[8] = {B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111};


//Funciones de ccontrol logico
uint8_t readConfigs(config_u*);
uint8_t saveConfigs(config_u*);
button_u readButton( uint8_t );

//Funcion de la maquina de estados
void estateMachine (states_u *);

//Funciones manejadoras de la pantalla
void printHello(void);
void printCounter(void);
void printMenuEntry(void);
void printDelayEntry(void);
void printPulseEntry(void);
void printOutputsEntry(void);
void printSaveEntry(void);

void serialPrintConfigs(void);


#endif
