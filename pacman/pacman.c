//--------------------------------------------------------------
// File     : pacman.c
// Datum    : 22.11.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F746
// IDE      : OpenSTM32
// GCC      : 4.9 2015q2
// Module   : keine
// Funktion : Pacman Gameplay
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "pacman.h"
#include <stdio.h>


//--------------------------------------------------------------
// Header fuer BMP-Transfer
// (fix als einen kompletten Screen (480x272) im Landscape-Mode)
//--------------------------------------------------------------
uint8_t BMP_HEADER[54]={
0x42,0x4D,0x36,0xFA,0x05,0x00,0x00,0x00,0x00,0x00, // ID=BM, Filsize=(480x272x3+54)
0x36,0x00,0x00,0x00,0x28,0x00,0x00,0x00,           // Offset=54d, Headerlen=40d
0xE0,0x01,0x00,0x00,0x10,0x01,0x00,0x00,0x01,0x00, // W=480d, H=272d (landscape)
0x18,0x00,0x00,0x00,0x00,0x00,0x00,0xFA,0x05,0x00, // 24bpp, unkomprimiert, Data=(480x272x3)
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,           // nc
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};          // nc
void p_Basic_send_Screen(void);


//--------------------------------------------------------------
// level settings
// speed in ms [Player, Blinky, Pinky, Inky, Clyde]
//--------------------------------------------------------------
Level_t Level[] = {
  {30,40,50,60,70}, // level 1
  {26,35,44,52,60}, // level 2
  {22,30,38,44,50}, // level 3
  {18,25,32,36,40}, // level 4
  {14,20,26,28,30}, // level 5
  {10,18,24,26,28}, // level 6
  {10,16,22,24,26}, // level 7
  {10,14,20,22,24}, // level 8
  {10,12,18,20,22}, // level 9
  {10,10,16,18,20}, // level 10...99
};


//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
uint32_t pacman_hw_init(void);
uint32_t pacman_play(void);
void pacman_dec_mode_timer(void);
void pacman_init(uint32_t mode);
void pacman_set_level(void);


//--------------------------------------------------------------
// init and start endless pacman gameplay
//--------------------------------------------------------------
void pacman_start(void)
{
  uint32_t check;
  char buf[10];

  pacman_init(GAME_OVER);

  check=pacman_hw_init();
  gui_debug_uart("PACMAN 4 STM32F746 [UB]");
  if(check!=0) {
    // hardware init error
    UB_LCD_FillLayer(BACKGROUND_COL);
    UB_Font_DrawString(10,180,"Touch ERR",&Arial_7x10,FONT_COL,BACKGROUND_COL);
    while(1);
  }
  skin_init();
  player_init(GAME_OVER);
  blinky_init(GAME_OVER);
  pinky_init(GAME_OVER);  
  inky_init(GAME_OVER);
  clyde_init(GAME_OVER);

  // show menu
  menu_start();
  pacman_set_level();

  maze_build();

  // gui_draw_debugmaze();  // only for debug to view the maze

  check=maze_generate_check();
  if(check==0) {
    // ok
    while(1) {
      //-------------------
      // play one round
      //-------------------
      gui_draw_maze();
      gui_draw_bots();
      GUI.refresh_value=GUI_REFRESH_VALUE;
      GUI.refresh_buttons=GUI_REFRESH_VALUE;
      gui_draw_gui(GUI_JOY_NONE);
      UB_Font_DrawString(320,55,"GET READY",&Arial_7x10,FONT_COL2,BACKGROUND_COL);
      HAL_Delay(1000);
      UB_Font_DrawString(320,55,"   SET   ",&Arial_7x10,FONT_COL2,BACKGROUND_COL);
      HAL_Delay(1000);
      UB_Font_DrawString(320,55,"    GO   ",&Arial_7x10,FONT_COL2,BACKGROUND_COL);
      HAL_Delay(1000);
      UB_Font_DrawString(320,55,"         ",&Arial_7x10,FONT_COL2,BACKGROUND_COL);
      check=pacman_play();
      HAL_Delay(5000);
      if(check==GAME_PLAYER_WIN) {
        // player win
        Player.level++;
      }
      else {
        // player lose
        Player.lives--;
        if(Player.lives==0) {
          // game over
          check=GAME_OVER;
        }
      }

      //-------------------
      // play again
      //-------------------
      pacman_init(check);
      player_init(check);
      blinky_init(check);
      pinky_init(check);
      inky_init(check);
      clyde_init(check); 
      if(check==GAME_OVER) {
        // show menu
        menu_start();
        pacman_set_level();
      }
      if((check==GAME_PLAYER_WIN) || (check==GAME_OVER)) {
        // reinit maze
        maze_build();
      }      
    }
  }
  else {
    //-------------------
    // error in maze
    //-------------------
    gui_draw_errmaze();
    UB_Font_DrawString(10,180,"MAZE ERR",&Arial_7x10,FONT_COL,BACKGROUND_COL);
    sprintf(buf,"last error nr = %d",(int)(check));
    UB_Font_DrawString(10,190,buf,&Arial_7x10,FONT_COL,BACKGROUND_COL);
  }
}


//--------------------------------------------------------------
// init complete hardware
//--------------------------------------------------------------
uint32_t pacman_hw_init(void)
{
  uint32_t ret_wert=0;

  if(UB_Touch_Init()!=SUCCESS) {
    ret_wert=1; // error
  }

  UB_Uart_Init();

  UB_Button_Init();

  UB_LCD_Init();
  UB_LCD_LayerInit_Fullscreen();

  UB_USB_HID_HOST_Init();

  gui_clear_screen();

  return(ret_wert);
}


//--------------------------------------------------------------
// init values
//--------------------------------------------------------------
void pacman_init(uint32_t mode)
{

  if(mode==GAME_OVER) {
    Game.collision=BOOL_TRUE;
    Game.controller=GAME_CONTROL_TOUCH;
  }
  Game.mode=GAME_MODE_SCATTER;
  Game.mode_timer=GAME_SCATTER_TIME;
  Game.frightened=BOOL_FALSE;
  Game.frightened_timer=GAME_FRIGHTENED_TIME;
  Game.frightened_points=GAME_FRIGHTENED_START_POINTS;
}


//--------------------------------------------------------------
// init Level
//--------------------------------------------------------------
void pacman_set_level(void)
{
  if(Player.level<=GAME_MAX_LEVEL) {
    Player.akt_speed_ms=Level[Player.level-1].player_speed;
    Blinky.akt_speed_ms=Level[Player.level-1].blinky_speed;
    Pinky.akt_speed_ms=Level[Player.level-1].pinky_speed;
    Inky.akt_speed_ms=Level[Player.level-1].inky_speed;
    Clyde.akt_speed_ms=Level[Player.level-1].clyde_speed;
  }
  else {
    Player.akt_speed_ms=Level[GAME_MAX_LEVEL-1].player_speed;
    Blinky.akt_speed_ms=Level[GAME_MAX_LEVEL-1].blinky_speed;
    Pinky.akt_speed_ms=Level[GAME_MAX_LEVEL-1].pinky_speed;
    Inky.akt_speed_ms=Level[GAME_MAX_LEVEL-1].inky_speed;
    Clyde.akt_speed_ms=Level[GAME_MAX_LEVEL-1].clyde_speed;
  }
}

//--------------------------------------------------------------
// play one round pacman (until pacman die or level complete)
// to avoid graphic flicker : 
//   1. clear all bots
//   2. move all bots
//   3. redraw all bots
//   4. show new screen
//   5. Goto 1
//--------------------------------------------------------------
uint32_t pacman_play(void)
{
  uint32_t ret_wert=GAME_RUN;
  uint32_t joy=GUI_JOY_NONE;
  uint32_t movement=0;
  int32_t pl_speed;  // signed int !!
  USB_HID_HOST_STATUS_t usb_status;

  // make a copy from the screen to the background
  UB_LCD_Copy_Layer2_to_Layer1();

  do {
	// USB-Keyboard
    usb_status=UB_USB_HID_HOST_Do();

    //----------------------------------------
    // Touch/Button Timer
    //----------------------------------------
    if(Gui_Touch_Timer_ms==0) {
      Gui_Touch_Timer_ms=GUI_TOUCH_INTERVALL_MS;
      if(Game.controller==GAME_CONTROL_TOUCH) {
        joy=gui_check_touch();
      }
      else if(Game.controller==GAME_CONTROL_4BUTTON) {
        joy=gui_check_button();
      }
      else {
        if(usb_status==USB_HID_KEYBOARD_CONNECTED) {
          joy=gui_check_usbkey();
        }
    	else {
          joy=GUI_JOY_NONE;
        }
      }
    }
    if(UB_Button_OnClick(BTN_USER)) {
    	p_Basic_send_Screen();
    }

    movement=MOVE_NOBODY;
    //----------------------------------------
    // Player Timer
    //----------------------------------------
    if(Player_Systick_Timer_ms==0) {
      if(Game.frightened==BOOL_FALSE) {
        Player_Systick_Timer_ms=Player.akt_speed_ms;
      }
      else {
        pl_speed=Player.akt_speed_ms-Player.frightened_buf;
        if(pl_speed<PLAYER_MAX_SPEED) pl_speed=PLAYER_MAX_SPEED;
        Player_Systick_Timer_ms=pl_speed;
      }
      movement|=MOVE_PLAYER;
    }

    //----------------------------------------
    // Blinky Timer
    //----------------------------------------
    if(Blinky_Systic_Timer_ms==0) {
      if(Blinky.status==GHOST_STATUS_ALIVE)	{
        if(Game.frightened==BOOL_FALSE) {
          Blinky_Systic_Timer_ms=Blinky.akt_speed_ms;
        }
        else {
          Blinky_Systic_Timer_ms=Blinky.akt_speed_ms+Blinky.frightened_buf;
        }
      }
      else {
        Blinky_Systic_Timer_ms=GHOST_DEAD_DELAY_MS;
      }
      movement|=MOVE_BLINKY;
    }

    //----------------------------------------
    // Pinky Timer
    //----------------------------------------
    if(Pinky_Systic_Timer_ms==0) {
      if(Pinky.status==GHOST_STATUS_ALIVE)	{
        if(Game.frightened==BOOL_FALSE) {
          Pinky_Systic_Timer_ms=Pinky.akt_speed_ms;
        }
        else {
          Pinky_Systic_Timer_ms=Pinky.akt_speed_ms+Pinky.frightened_buf;
        }
      }
      else {
        Pinky_Systic_Timer_ms=GHOST_DEAD_DELAY_MS;
      }
      movement|=MOVE_PINKY;
    }

    //----------------------------------------
    // Inky Timer
    //----------------------------------------
    if(Inky_Systic_Timer_ms==0) {
      if(Inky.status==GHOST_STATUS_ALIVE)	{
        if(Game.frightened==BOOL_FALSE) {
          Inky_Systic_Timer_ms=Inky.akt_speed_ms;
        }
        else {
          Inky_Systic_Timer_ms=Inky.akt_speed_ms+Inky.frightened_buf;
        }
      }
      else {
        Inky_Systic_Timer_ms=GHOST_DEAD_DELAY_MS;
      }
      movement|=MOVE_INKY;
    }

    //----------------------------------------
    // Clyde Timer
    //----------------------------------------
    if(Clyde_Systic_Timer_ms==0) {
      if(Clyde.status==GHOST_STATUS_ALIVE)	{
        if(Game.frightened==BOOL_FALSE) {
          Clyde_Systic_Timer_ms=Clyde.akt_speed_ms;
        }
        else {
          Clyde_Systic_Timer_ms=Clyde.akt_speed_ms+Clyde.frightened_buf;
        }
      }
      else {
        Clyde_Systic_Timer_ms=GHOST_DEAD_DELAY_MS;
      }
      movement|=MOVE_CLYDE;
    }

    //----------------------------------------
    // Mode Timer
    //----------------------------------------
    if(Mode_Systic_Timer_ms==0) {
      Mode_Systic_Timer_ms=GAME_MODE_TIMER;
      pacman_dec_mode_timer();
    }    

    //----------------------------------------
    // 1. clear all Bots (before movement)
    //----------------------------------------
    if(movement!=MOVE_NOBODY) {
      gui_clear_bots();
    }

    //----------------------------------------
    // 2a. Player movement
    //----------------------------------------
    if((movement&MOVE_PLAYER)!=0) {
      player_move();
      player_change_direction(joy);
    }

    //----------------------------------------
    // 2b. Blinky movement
    //----------------------------------------
    if((movement&MOVE_BLINKY)!=0) {
      blinky_move();
    }

    //----------------------------------------
    // 2c. Pinky movement
    //----------------------------------------
    if((movement&MOVE_PINKY)!=0) {
      pinky_move();
    }

    //----------------------------------------
    // 2d. Inky movement
    //----------------------------------------
    if((movement&MOVE_INKY)!=0) {
      inky_move();
    }

    //----------------------------------------
    // 2d. Clyde movement
    //----------------------------------------
    if((movement&MOVE_CLYDE)!=0) {
      clyde_move();
    }

    //----------------------------------------
    // 3. redraw all Bots (after movement)
    // 4. LCD refresh
    //----------------------------------------
    if(movement!=MOVE_NOBODY) {
      gui_draw_bots();
      gui_draw_gui(joy);
      UB_LCD_Refresh();
    }

    //----------------------------------------
    // check if game over
    //----------------------------------------
    if(Player.status==PLAYER_STATUS_DEAD) {
      ret_wert=GAME_PLAYER_LOSE;
    }
    if(Player.status==PLAYER_STATUS_WIN) {
      ret_wert=GAME_PLAYER_WIN;
    }
  }while(ret_wert==GAME_RUN);

  return(ret_wert);
}


//--------------------------------------------------------------
// decrement mode timer and change mode if nessesary
//--------------------------------------------------------------
void pacman_dec_mode_timer(void)
{
  if(Game.frightened==BOOL_FALSE) {
    Game.mode_timer--;
    if(Game.mode_timer==0) {
      GUI.refresh_value=GUI_REFRESH_VALUE; // (only for debug)
      if(Game.mode==GAME_MODE_SCATTER) {
        Game.mode=GAME_MODE_CHASE;
        Game.mode_timer=GAME_CHASE_TIME;
      }
      else {
        Game.mode=GAME_MODE_SCATTER;
        Game.mode_timer=GAME_SCATTER_TIME;
      }
      // set marker of all alive bots
      if(Blinky.status==GHOST_STATUS_ALIVE) Blinky.new_mode=1;
      if(Pinky.status==GHOST_STATUS_ALIVE) Pinky.new_mode=1;
      if(Inky.status==GHOST_STATUS_ALIVE) Inky.new_mode=1;
      if(Clyde.status==GHOST_STATUS_ALIVE) Clyde.new_mode=1;
    }
  }
  else {
    Game.frightened_timer--;
    if(Game.frightened_timer==0) {
      Game.frightened=BOOL_FALSE;
      Game.frightened_points=GAME_FRIGHTENED_START_POINTS;
    }
  }
}


//--------------------------------------------------------------
// interne Funktion
// sendet ein ScreenCopy per UART als "BMP-File"
//--------------------------------------------------------------
void p_Basic_send_Screen(void)
{
  uint32_t n,adr;
  uint16_t x,y,color;
  uint8_t r,g,b;

  // BMP-Header senden
  for(n=0;n<54;n++) {
    UB_Uart_SendByte(COM1,BMP_HEADER[n]);
  }

  // den richigen Buffer zum senden raussuchen
  adr=LCD_CurrentFrameBuffer;

  // alle Farb-Daten senden
  for(y=0;y<LCD_MAXY;y++) {
    for(x=0;x<LCD_MAXX;x++) {
      n=(LCD_MAXY-y-1)*(LCD_MAXX*2)+(x*2);
      color=*(volatile uint16_t*)(adr+n);
      r=((color&0xF800)>>8);  // 5bit rot
      g=((color&0x07E0)>>3);  // 6bit gruen
      b=((color&0x001F)<<3);  // 5bit blau
      UB_Uart_SendByte(COM1,b);
      UB_Uart_SendByte(COM1,g);
      UB_Uart_SendByte(COM1,r);
    }
  }
}
