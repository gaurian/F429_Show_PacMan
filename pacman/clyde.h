//--------------------------------------------------------------
// File     : clyde.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_CLYDE_H
#define __STM32F4_UB_CLYDE_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_system.h"
#include "bot.h"
#include "skin.h"
#include "gui.h"


//--------------------------------------------------------------
#define  CLYDE_START_X     16
#define  CLYDE_START_Y     14     

//--------------------------------------------------------------
#define  CLYDE_HOME_X      16
#define  CLYDE_HOME_Y      14




//--------------------------------------------------------------
#define  CLYDE_FRIGHTENED_BUF  30  // slow down bot
#define  CLYDE_DOT_CNT_MAX     20


//--------------------------------------------------------------
#define  CLYDE_SCATTER_X   0 
#define  CLYDE_SCATTER_Y   ROOM_CNT_Y-1





//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void clyde_init(uint32_t mode);
void clyde_move(void);


//--------------------------------------------------------------
#endif // __STM32F4_UB_CLYDE_H
