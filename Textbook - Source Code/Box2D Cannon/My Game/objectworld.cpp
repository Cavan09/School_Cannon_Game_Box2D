/// \file objectworld.cpp
/// \brief Code for the object world CObjectWorld.

#include "objectworld.h"
#include "Timer.h"

extern CTimer g_cTimer;

CObjectWorld::CObjectWorld(){
  m_pObjectManager = new CObjectManager(256);
  m_pHeadsUpDisplay = new CHeadsUpDisplay(32);
  m_pHeadsUpDisplay->m_bEasterEgg = FALSE; //easter egg
} //constructor

CObjectWorld::~CObjectWorld(){
  delete m_pObjectManager;
  delete m_pHeadsUpDisplay;
} //destructor

/// Create an object in the Object World.
/// \param t Object type.
/// \return Pointer to the object created.

CGameObject* CObjectWorld::create(GameObjectType t){ // Create new object.
  return m_pObjectManager->create(t);
} //create

/// Create a HUD object in the Object World.
/// \param t Object type.
/// \return Pointer to the object created.

CGameObject* CObjectWorld::CreateHudObject(GameObjectType t){ // Create new HUD object.
  return m_pHeadsUpDisplay->create(t);
} //create

/// Set the Object World's width and height.
/// \param width Object World width.
/// \param height Object World height.

void CObjectWorld::SetWorldSize(float width, float height){
  m_fWidth = width; m_fHeight = height;
} //SetWorldSize

/// Get the Object World's width and height.
/// \param width Calling parameter will be set to Object World width.
/// \param height Calling parameter will be set to Object World height.

void CObjectWorld::GetWorldSize(float& width, float& height){
  width = m_fWidth; height = m_fHeight;
} //SetWorldSize

/// Determine whether player has won.
/// \param level Proportion of Object World height that the blocks must be below to win.
/// \return TRUE if player has won.

BOOL CObjectWorld::PlayerHasWon(float level){
  BOOL result=TRUE;
  CGameObject* p = NULL;
  for(int i=0; i<m_pObjectManager->m_nCount; i++){ //for each object
    p = m_pObjectManager->m_pObjectList[i];
    if(p &&  p->m_pBody && p->m_nObjectType == BOOK_OBJECT) 
      result = result && PW2RW(p->m_pBody->GetPosition().y) < m_fHeight/level;
  } //for
  return result;
} //PlayerHasWon

/// Clear objects.

void CObjectWorld::clear(){
  m_pObjectManager->clear();
} //clear

/// Draw everything in the Object World.
/// Draw the game objects, then the HUD in that order using Painter's
/// Algorithm.

void CObjectWorld::draw(){
  m_pObjectManager->draw(); //draw objects next
  m_pHeadsUpDisplay->draw( //draw HUD on top of that
    g_cTimer.GetLevelElapsedTime()/1000, //time
    m_cCannon.m_fCannonTemp, m_cCannon.m_fCannonMaxTemp); //temperature
} //draw

/// Move objects.

void CObjectWorld::move(){
  m_pObjectManager->move(); //move objects
} //draw

/// Make sounds for all objects in the Object World.

void CObjectWorld::MakeSound(){ 
  m_pObjectManager->MakeSound();
} //MakeSound