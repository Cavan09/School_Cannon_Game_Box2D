/// \file MyGame.cpp 
/// \brief Main file for your game.

/// \mainpage Box2D Cannon Game
/// This code was written by Ian Parberry to accompany his book
/// "Introduction to Game Physics with Box2D",  published by CRC Press in 2013.
///
/// The aim of the Cannon Game is to use the cannon to knock down a tower of books
/// in less than 60 seconds. If the cannon overheats, the cannon explodes and
/// you lose. If time runs out, you lose anyway. 
///
/// Use the space bar to fire the cannon, the left and right arrow keys to move it,
/// the up and down arrow keys to elevate the cannon barrel, the Enter key to restart
/// the game, and the ESC key to quit.


#include "gamedefines.h"
#include "SndList.h"

#include "Box2D\Box2D.h"
#include "RenderWorld.h"
#include "ObjectWorld.h"

//globals
char g_szGameName[256]; ///< Name of this game.
GameStateType g_nGameState; ///< Current game state.

CTimer g_cTimer; ///< The game timer.
CSoundManager* g_pSoundManager; ///< The sound manager.

//Physics World
b2World g_b2dPhysicsWorld(b2Vec2(0, RW2PW(-100))); ///< Box2D Physics World.

//Render and Object Worlds
CRenderWorld g_cRenderWorld; ///< The Render World.
CObjectWorld g_cObjectWorld; ///< The Object World.


//prototypes for Windows functions
int DefaultWinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK DefaultWindowProc(HWND, UINT, WPARAM, LPARAM);

//prototypes for functions in nonplayerobjects.cpp
void CreateWorldEdges();
void CreateTower();

/// Create all game objects.

void CreateObjects(){
  //HUD, in Object World but not in Physics World
  g_cObjectWorld.CreateHudObject(TEMPGUAGE_OBJECT);
  g_cObjectWorld.CreateHudObject(TEMPNEEDLE_OBJECT);
  g_cObjectWorld.CreateHudObject(TEMPMAXNEEDLE_OBJECT);
  g_cObjectWorld.CreateHudObject(CLOCKFACE_OBJECT);
  g_cObjectWorld.CreateHudObject(CLOCKNEEDLE_OBJECT);
  g_cObjectWorld.CreateHudObject(PIRATE_OBJECT);  //easter egg

  //stuff that is in both Object and Physics Worlds
  CreateWorldEdges(); //edges of screen
  CreateTower(); //tower of books
  g_cObjectWorld.m_cCannon.create(); //player character
} //CreateObjects

/// Start the game.

void BeginGame(){ 
  g_nGameState = PLAYING_GAMESTATE; //playing state
  g_cTimer.StartLevelTimer(); //starting level now     
  g_cObjectWorld.clear(); //clear old objects
  CreateObjects(); //create new objects
  g_cObjectWorld.m_cCannon.Reset(); //reset the cannon
} //BeginGame

/// Initialize and start the game.

void InitGame(){
  //set up Render World
  g_cRenderWorld.Initialize(); //bails if it fails
  g_cRenderWorld.LoadImages(); //load images from xml file list

  //set up Object World
  int w, h;
  g_cRenderWorld.GetWorldSize(w, h);
  g_cObjectWorld.SetWorldSize((float)w, (float)h);

  //now start the game
  BeginGame();
} //InitGame

/// Shut down game and release resources.

void EndGame(){
  g_cRenderWorld.End();
} //EndGame

/// Render a frame of animation.

void RenderFrame(){
  if(g_cRenderWorld.BeginScene()){ //start up graphics pipeline
    g_cRenderWorld.DrawBackground(); //draw the background
    g_cObjectWorld.draw(); //draw the objects
    g_cRenderWorld.DrawWinLoseMessage(g_cObjectWorld.m_cCannon.BallsFired(), 
      g_nGameState, g_cTimer.GetLevelElapsedTime()/1000); //win/loss text
    g_cRenderWorld.EndScene(); //shut down graphics pipeline
  } //if
} //RenderFrame

/// Process a frame of animation.
/// Called once a frame to animate game objects and take appropriate
/// action if the player has won or lost.

void ProcessFrame(){
  //stuff that gets done on every frame
  g_cTimer.beginframe(); //capture current time
  g_pSoundManager->beginframe(); //no double sounds
  g_cObjectWorld.move(); //move all objects
  g_cObjectWorld.MakeSound(); //all objects play sounds
  g_cObjectWorld.m_cCannon.CoolDown(); //cannon cools down

  RenderFrame(); //render a frame of animation

  //Stuff that's done only when playing
  if(g_nGameState == PLAYING_GAMESTATE){ 
    int t = g_cTimer.GetLevelElapsedTime()/1000;

    //clock tick sound once per second
    static int lasttick=0;
    if(lasttick != t){
      g_pSoundManager->play(TICK_SOUND);
      lasttick = t;
    }

    //Has the player won or lost?
    if(g_cObjectWorld.m_cCannon.IsDead() || t >= g_nMaxPlayingTime){ //lost
      g_nGameState = LOST_GAMESTATE;
      g_pSoundManager->play(LOSE_SOUND);
      g_cTimer.StopLevelTimer();
    } //if lost
    else
      if(g_cObjectWorld.PlayerHasWon(4.0f)){ //won
        g_nGameState = WON_GAMESTATE;
        g_pSoundManager->play(WIN_SOUND);
        g_cTimer.StopLevelTimer();
      } //else if won
  } //if playing
} //ProcessFrame

/// Keyboard handler.
/// Take the appropriate action when the user mashes a key on the keyboard.
///  \param k Virtual key code for the key pressed
///  \return TRUE if the game is to exit

BOOL KeyboardHandler(WPARAM k){ //keyboard handler
  const float CANNONBARREL_DELTA_ANGLE = 0.01f;
  const float CANNONMOVE_DELTA = 2.0f;
  CCannon* cannon = &(g_cObjectWorld.m_cCannon);

  //keystrokes that work in any state
  if(k ==  VK_ESCAPE)return TRUE; //quit

  if(g_nGameState == PLAYING_GAMESTATE) //keystrokes while playing
    switch(k){
      case VK_UP: //barrel up
        cannon->BarrelUp(CANNONBARREL_DELTA_ANGLE);
        break;
      case VK_DOWN: //barrel down
       cannon->BarrelUp(-CANNONBARREL_DELTA_ANGLE);
        break;
      case VK_LEFT: //move left
        cannon->StartMovingLeft(CANNONMOVE_DELTA);
        break;
      case VK_RIGHT: //move right
        cannon->StartMovingLeft(-CANNONMOVE_DELTA);
        break;
      case VK_SPACE: //fire cannon
        if(cannon->Fire())
          g_pSoundManager->play(CANNONFIRE_SOUND);
        break;
    } //switch

  else  //keystrokes while not playing
    if(k == VK_RETURN)
      BeginGame(); //restart

  //Easter egg!
  if(k == 'R'){ //arrrr!
    if(!g_cObjectWorld.m_pHeadsUpDisplay->m_bEasterEgg)
      g_pSoundManager->play(R_SOUND);
    g_cObjectWorld.m_pHeadsUpDisplay->m_bEasterEgg = TRUE;
  }

  return FALSE;
} //KeyboardHandler

// Windows functions.
// Dont mess with these unless you really know what you're doing.
// I've written default functions in the Engine library to take
// care of the boring details of Windows housekeeping.

/// Window procedure.
/// Handler for messages from the Windows API. Dont mess with these unless you really know what you're doing.
///  \param h window handle
///  \param m message code
///  \param w parameter for message 
///  \param l second parameter for message
///  \return 0 if message is handled

LRESULT CALLBACK WindowProc(HWND h, UINT m, WPARAM w, LPARAM l){
  return DefaultWindowProc(h, m, w, l);
} //WindowProc

/// Winmain.  
/// Main entry point for this application. Dont mess with these unless you really know what you're doing.
///  \param hI handle to the current instance of this application
///  \param hP unused
///  \param lpC unused 
///  \param nCS specifies how the window is to be shown
///  \return TRUE if application terminates correctly

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nCS){                  
  return DefaultWinMain(hI, hP, lpC, nCS);
} //WinMain