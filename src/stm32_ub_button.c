//--------------------------------------------------------------
// File     : stm32_ub_button.c
// Datum    : 12.07.2015
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F746
// IDE      : OpenSTM32
// GCC      : 4.9 2015q2
// Module   : CubeHAL
// Funktion : Button Funktionen
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_button.h"







//--------------------------------------------------------------
// Definition aller Buttons
// Reihenfolge wie bei BUTTON_NAME_t
//
// Widerstand : [GPIO_NOPULL,GPIO_PULLUP,GPIO_PULLDOWN]
// Status     : [GPIO_PIN_RESET, GPIO_PIN_SET]
//--------------------------------------------------------------
BUTTON_t BUTTON[] = {
  // Name    ,PORT , PIN        ,Widerstand      , Status, TIC
  {BTN_USER  ,GPIOI,GPIO_PIN_11 ,GPIO_NOPULL, GPIO_PIN_RESET,0},  // PI11=User-Button auf dem Discovery-Board
  {BTN_UP    ,GPIOF,GPIO_PIN_6  ,GPIO_PULLUP, GPIO_PIN_RESET,0},  // CN5/6
  {BTN_RIGHT ,GPIOF,GPIO_PIN_7  ,GPIO_PULLUP, GPIO_PIN_RESET,0},  // CN5/5
  {BTN_DOWN  ,GPIOF,GPIO_PIN_8  ,GPIO_PULLUP, GPIO_PIN_RESET,0},  // CN5/4
  {BTN_LEFT  ,GPIOF,GPIO_PIN_9  ,GPIO_PULLUP, GPIO_PIN_RESET,0},  // CN5/3
};
static int BUTTON_ANZ = sizeof(BUTTON)/sizeof(BUTTON[0]);  // Anzahl der Eintraege


//--------------------------------------------------------------
// Init aller Buttons
//--------------------------------------------------------------
void UB_Button_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  BUTTON_NAME_t btn_name;
  
  for(btn_name=0;btn_name<BUTTON_ANZ;btn_name++) {
    // Clock Enable
    UB_System_ClockEnable(BUTTON[btn_name].BUTTON_PORT);
  
    // Config als Digital-Eingang
    GPIO_InitStruct.Pin = BUTTON[btn_name].BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = BUTTON[btn_name].BUTTON_R;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(BUTTON[btn_name].BUTTON_PORT, &GPIO_InitStruct);
  }
}


//--------------------------------------------------------------
// Status von einem Button auslesen (nicht entprellt)
// Return Wert :
//  -> wenn Button losgelassen = BTN_RELEASED
//  -> wenn Button gedrueckt   = BTN_PRESSED
//--------------------------------------------------------------
BUTTON_STATUS_t UB_Button_Read(BUTTON_NAME_t btn_name)
{
  uint8_t wert;

  wert=HAL_GPIO_ReadPin(BUTTON[btn_name].BUTTON_PORT, BUTTON[btn_name].BUTTON_PIN);
  if(wert==GPIO_PIN_RESET) {
    return(BTN_RELEASED);
  }
  else {
    return(BTN_PRESSED);
  }
} 



//--------------------------------------------------------------
// Button OnPressed Auswertung (entprellt)
// ret_wert, ist solange true wie der Button betätigt ist
//--------------------------------------------------------------
bool UB_Button_OnPressed(BUTTON_NAME_t btn_name)
{
  uint8_t wert;
  uint32_t tic;

  tic=HAL_GetTick();
  if(tic-BUTTON[btn_name].BUTTON_TIC>BUTTON_INTERVALL) {
    BUTTON[btn_name].BUTTON_TIC=tic;
    BUTTON[btn_name].BUTTON_AKT=UB_Button_Read(btn_name);
  }

  wert=BUTTON[btn_name].BUTTON_AKT;

  if(wert==GPIO_PIN_RESET) {
    return(false);
  }
  else {
    return(true);
  }
}




//--------------------------------------------------------------
// Button OnClick Auswertung (entprellt)
// ret_wert, ist nur einmal true wenn der Button betätigt wurde
//--------------------------------------------------------------
bool UB_Button_OnClick(BUTTON_NAME_t btn_name)
{
  uint8_t wert=GPIO_PIN_RESET,old=GPIO_PIN_RESET;
  uint32_t tic;

  tic=HAL_GetTick();
  if(tic-BUTTON[btn_name].BUTTON_TIC>BUTTON_INTERVALL) {
    BUTTON[btn_name].BUTTON_TIC=tic;
    wert=UB_Button_Read(btn_name);
    old=BUTTON[btn_name].BUTTON_AKT;
    BUTTON[btn_name].BUTTON_AKT=wert;
  }

  if(wert==GPIO_PIN_RESET) {
    return(false);
  }
  else if(old!=GPIO_PIN_RESET) {
    return(false);
  }
  else {
    return(true);
  }
}




//--------------------------------------------------------------
// Button OnRelease Auswertung (entprellt)
// ret_wert, ist nur einmal true wenn der Button losgelassen wurde
//--------------------------------------------------------------
bool UB_Button_OnRelease(BUTTON_NAME_t btn_name)
{
  uint8_t wert=GPIO_PIN_SET,old=GPIO_PIN_RESET;
  uint32_t tic;

  tic=HAL_GetTick();
  if(tic-BUTTON[btn_name].BUTTON_TIC>BUTTON_INTERVALL) {
    BUTTON[btn_name].BUTTON_TIC=tic;
    wert=UB_Button_Read(btn_name);
    old=BUTTON[btn_name].BUTTON_AKT;
    BUTTON[btn_name].BUTTON_AKT=wert;
  }

  if(wert!=GPIO_PIN_RESET) {
    return(false);
  }
  else if(old==GPIO_PIN_RESET) {
    return(false);
  }
  else {
    return(true);
  }
}










