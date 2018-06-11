//--------------------------------------------------------------
// File     : main.c
// Datum    : 06.08.2015
// Version  : 1.1
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F746
// Board    : STM32F746-Discovery-Board
// IDE      : OpenSTM32
// GCC      : 4.9 2015q2
// Module   : CubeHAL
// Funktion : Hauptprogramm
//--------------------------------------------------------------


#include "stm32_ub_system.h"
#include "pacman.h"

// Controller-1 : Touch
// Controller-2 : 4 Buttons  [CN5/6, CN5/5, CN5/4, CN5/3]
// Controller-3 : USB-Keyboard CN13  [W,A,S,D]
// User-Button  : ScreenShot [VCP @ 115200 Baud]

int main(void)
{
  // init vom System
  UB_System_Init();

  pacman_start();

  while(1) {
    // nothing to do
  }
}
