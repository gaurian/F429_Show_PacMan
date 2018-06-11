//--------------------------------------------------------------
// File     : stm32_ub_button.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F7_UB_BUTTON_H
#define __STM32F7_UB_BUTTON_H

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_system.h"
#include <stdbool.h>





//--------------------------------------------------------------
// Liste aller Buttons
// (keine Nummer doppelt und von 0 beginnend)
//--------------------------------------------------------------
typedef enum 
{
  BTN_USER = 0,    // BTN auf dem STM32F746-Discovery
  BTN_UP,
  BTN_RIGHT,
  BTN_DOWN,
  BTN_LEFT
}BUTTON_NAME_t;



//--------------------------------------------------------------
// Status eines Buttons
//--------------------------------------------------------------
typedef enum {
  BTN_RELEASED = 0,  // Button losgelassen
  BTN_PRESSED        // Button gedrueckt
}BUTTON_STATUS_t;




//--------------------------------------------------------------
// Struktur eines Buttons
//--------------------------------------------------------------
typedef struct {
  BUTTON_NAME_t BUTTON_NAME;  // Name
  GPIO_TypeDef* BUTTON_PORT;  // Port
  const uint16_t BUTTON_PIN;  // Pin
  const uint32_t BUTTON_R;    // Widerstand
  uint8_t BUTTON_AKT;         // Istwert
  uint32_t BUTTON_TIC;        // Timerwert
}BUTTON_t;


//--------------------------------------------------------------
#define   BUTTON_INTERVALL    50   // 50ms zum entprellen

//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_Button_Init(void);
BUTTON_STATUS_t UB_Button_Read(BUTTON_NAME_t btn_name);
bool UB_Button_OnPressed(BUTTON_NAME_t btn_name);
bool UB_Button_OnClick(BUTTON_NAME_t btn_name);
bool UB_Button_OnRelease(BUTTON_NAME_t btn_name);

//--------------------------------------------------------------
#endif // __STM32F7_UB_BUTTON_H
