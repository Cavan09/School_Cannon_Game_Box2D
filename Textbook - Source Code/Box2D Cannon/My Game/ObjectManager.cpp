/// \file ObjectManager.cpp
/// \brief Code for the object manager class CObjectManager.

#include "ObjectManager.h"

#include "gamedefines.h"
#include "renderworld.h"

extern b2World g_b2dPhysicsWorld;
extern CRenderWorld g_cRenderWorld;

CObjectManager::CObjectManager(int size){ //constructor 
  m_pObjectList = new CGameObject* [size];
  m_nSize = size;
  m_nCount = 0; //no objects
  for(int i=0; i<size; i++) //null our object list
    m_pObjectList[i] = NULL;
} //constructor

CObjectManager::~CObjectManager(){ //destructor
  for(int i=0; i<m_nSize; i++) //for each object
    delete m_pObjectList[i]; //delete it
  delete [] m_pObjectList;
} //destructor

/// Create a new instance of a game object.
/// \param objecttype The type of the new object

CGameObject* CObjectManager::create(GameObjectType objecttype){
  if(m_nCount < m_nSize){ //if room, create object
    int i=0; while(m_pObjectList[i])i++; //find first free slot
    m_pObjectList[i] = new CGameObject(objecttype);
    m_nCount++; //one more object
    return m_pObjectList[i];
  } //if
  else return NULL;
} //create

/// Ask the Physics World to move all game objects.

void CObjectManager::move(){ 
  g_b2dPhysicsWorld.Step(1.0f/60.0f, 6, 2);
} //move

/// Ask the Render World to draw all game objects.

void CObjectManager::draw(){ 
  CGameObject* p;
  for(int i=0; i<m_nSize; i++){ //for each object slot
    p = m_pObjectList[i]; //handy object pointer
    if(p){ //if there's an object there
      float a = p->m_pBody->GetAngle(); //orientation
      b2Vec2 v = p->m_pBody->GetPosition(); //position in Physics World units
      g_cRenderWorld.draw(p->m_nObjectType, PW2RW(v.x), PW2RW(v.y), a);
    } //if
  } //for
} //draw

/// Clear out all game objects from the object list. 

void CObjectManager::clear(){ 
  m_nCount = 0; //no objects
  for(int i=0; i<m_nSize; i++){ //for each object
    delete m_pObjectList[i]; //delete it
    m_pObjectList[i] = NULL; //safe delete
  } //for
} //clear

/// Make sounds for all objects in the object list.

void CObjectManager::MakeSound(){ 
  for(int i=0; i<m_nSize; i++) //for each object slot
    if(m_pObjectList[i]) //if there's an object there
      m_pObjectList[i]->MakeSound(); //ask it to make its sound
} //MakeSound