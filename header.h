#ifndef _HEADER_H
#define _HEADER_H
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
  counter,
  menu_entry,
  menu_delay,
  manu_pulse,
  manu_outputs,
  menu_save
} states;

uint8_t readConfigs(config_u*);
uint8_t saveConfigs(config_u*);
button_u readButton( uint8_t );

#endif
