/// \file gamedefines.h
/// \brief Game defines.

#pragma once

#include "defines.h"

/// \brief Object type.
///
/// Types of object that can appear in the game. 

enum GameObjectType{
  UNKNOWN_OBJECT, BALL_OBJECT, BOOK_OBJECT, CANNONBARREL_OBJECT, CANNONMOUNT_OBJECT, WHEEL_OBJECT,
  TEMPGUAGE_OBJECT, TEMPNEEDLE_OBJECT, TEMPMAXNEEDLE_OBJECT, CLOCKFACE_OBJECT, CLOCKNEEDLE_OBJECT,
  PIRATE_OBJECT  //easter egg
}; //GameObjectType

//Translate units between Render World and Physics World
const float fPRV = 10.0f; ///< Physics world rescale value
inline float PW2RW(float x){return x*fPRV;}; ///< Physics World to Render World units.
inline float RW2PW(float x){return x/fPRV;}; ///< Render World to Physics World units.
inline float RW2PW(int x){return (float)x/fPRV;}; ///< Render World to Physics World units.

/// State of game play, including whether the player has won or lost.

enum GameStateType{
  PLAYING_GAMESTATE, WON_GAMESTATE, LOST_GAMESTATE
}; //GameStateType

const int g_nMaxPlayingTime = 60; ///< Maximum allowable playing time per level.