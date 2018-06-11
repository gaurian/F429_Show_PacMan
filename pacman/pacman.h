//--------------------------------------------------------------
// File     : pacman.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_PACMAN_H
#define __STM32F4_UB_PACMAN_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_system.h"
#include "stm32_ub_lcd_480x272.h"
#include "stm32_ub_graphic2d.h"
#include "stm32_ub_touch_480x272.h"
#include "maze.h"
#include "maze_generate.h"
#include "gui.h"
#include "stm32_ub_font.h"
#include "menu.h"


//--------------------------------------------------------------
// level structur
//--------------------------------------------------------------
typedef struct {
  uint32_t player_speed;
  uint32_t blinky_speed;
  uint32_t pinky_speed;
  uint32_t inky_speed;
  uint32_t clyde_speed;
}Level_t;

#define  GAME_MAX_LEVEL   10


//--------------------------------------------------------------
#define  GAME_MODE_TIMER       100  // 100ms
#define  GAME_SCATTER_TIME     70   // (70*100ms) 7sec
#define  GAME_CHASE_TIME       200  // (200*100ms) 20sec
#define  GAME_FRIGHTENED_TIME  60   // (60*100ms) 6sec
#define  GAME_FRIGHTENED_BLINK 20   // (20*100ms) 2sec


//--------------------------------------------------------------
#define  GAME_POINTS_NORMAL  10
#define  GAME_POINTS_ENERGY  50



//--------------------------------------------------------------
#define  GAME_FRIGHTENED_START_POINTS  200


//--------------------------------------------------------------
#define  GAME_RUN          0
#define  GAME_PLAYER_WIN   1
#define  GAME_PLAYER_LOSE  2
#define  GAME_OVER         3

//--------------------------------------------------------------
#define  MOVE_NOBODY  0x00  
#define  MOVE_PLAYER  0x01  // Bit0
#define  MOVE_BLINKY  0x02  // Bit1
#define  MOVE_PINKY   0x04  // Bit2
#define  MOVE_INKY    0x08  // Bit3
#define  MOVE_CLYDE   0x10  // Bit4


//--------------------------------------------------------------
#define  BOOL_FALSE   0
#define  BOOL_TRUE    1    


//--------------------------------------------------------------
#define  GAME_CONTROL_TOUCH    0
#define  GAME_CONTROL_4BUTTON  1
#define  GAME_CONTROL_USBKEY   2
 

//--------------------------------------------------------------
#define  GAME_MODE_SCATTER     0
#define  GAME_MODE_CHASE       1


//--------------------------------------------------------------
// Main Game Structure
//--------------------------------------------------------------
typedef struct {
  uint32_t collision;
  uint32_t controller;
  uint32_t mode;
  uint32_t mode_timer;
  uint32_t frightened;
  uint32_t frightened_timer;
  uint32_t frightened_points;
}Game_t;
Game_t Game;



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void pacman_start(void);



//--------------------------------------------------------------
#endif // __STM32F4_UB_PACMAN_H
