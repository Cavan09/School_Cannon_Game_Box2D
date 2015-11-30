/// \file renderworld.cpp
/// \brief Code for the render world CRenderWorld.

#include "renderworld.h"

/// Load game images. Gets file list from gamesettings.xml.

void CRenderWorld::LoadImages(){ //load images
  LoadBackground(); 
  
  //Load sprite for each object
  Load(BALL_OBJECT, "ball"); 
  Load(BOOK_OBJECT, "book");
  Load(CANNONBARREL_OBJECT, "cannonbarrel");
  Load(CANNONMOUNT_OBJECT, "cannonmount");
  Load(WHEEL_OBJECT, "wheel");
  Load(TEMPGUAGE_OBJECT, "tempguage");
  Load(TEMPNEEDLE_OBJECT, "tempneedle");
  Load(TEMPMAXNEEDLE_OBJECT, "tempmaxneedle");
  Load(CLOCKFACE_OBJECT, "clockface");
  Load(CLOCKNEEDLE_OBJECT, "clockneedle");
  Load(PIRATE_OBJECT, "pirate");  //easter egg
} //LoadImages
 
/// Tell the player whether they've won or lost by plastering a text banner across the screen.
/// \param shots Number of shots fired.
/// \param state The game state, which tells whether the player has won or lost.
/// \param secs Time to completion, in seconds.

void CRenderWorld::DrawWinLoseMessage(int shots, GameStateType state, int secs){
  const int BUFSIZE = 256;
  char buffer[BUFSIZE];
  switch(state){
    case WON_GAMESTATE:
      if(shots == 1)
        sprintf_s(buffer, BUFSIZE, "Win in 0:%d with 1 Shot!", secs, shots);
      else
        sprintf_s(buffer, BUFSIZE, "Win in 0:%d with %d Shots!", secs, shots);
        TextWrite(buffer, RGB(255, 255, 255));
      break;
    case LOST_GAMESTATE:
      TextWrite("Loser!", RGB(192, 0, 0));
      break;
  } //switch
} //DrawWinLoseMessage