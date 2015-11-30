/// \file object.cpp
/// \brief Code for the game object class CGameObject.

#include "object.h"
#include "Sound.h"
#include "Sndlist.h"

extern b2World g_b2dPhysicsWorld;
extern CSoundManager* g_pSoundManager;

/// Constructor for a game object.
/// \param objecttype Object type.

CGameObject::CGameObject(GameObjectType objecttype){ //constructor
  m_nObjectType = objecttype; 
  m_pBody = NULL;
} //constructor

/// Destructor for a game object. It takes
/// care of destroying the object's body in Physics World.

CGameObject::~CGameObject(){ //destructor
  if(m_pBody)
    g_b2dPhysicsWorld.DestroyBody(m_pBody);
} //destructor

/// Set the physics body pointer (from Physics World) of the game object.
/// \param b Pointer to the physics body.

void CGameObject::SetPhysicsBody(b2Body* b){
  m_pBody = b;
} //SetPhysicsBody

void CGameObject::MakeSound(){ 
  if(!m_pBody)return; //safety

  b2Vec2 vNewV = m_pBody->GetLinearVelocityFromWorldPoint(b2Vec2(0, 0));
  b2Vec2 vDelta = m_b2vOldV - vNewV; //delta vee

  if(vDelta.LengthSquared() > 25000.0f) //big delta vee
    switch(m_nObjectType){ //make correct sound
      case BALL_OBJECT:
      case WHEEL_OBJECT:
        g_pSoundManager->play(THUMP_SOUND);
        break;

      case BOOK_OBJECT:
      case CANNONMOUNT_OBJECT:
        g_pSoundManager->play(THUMP2_SOUND);
        break;

      case CANNONBARREL_OBJECT:
        g_pSoundManager->play(CLANG_SOUND);
        break;

    } //switch

  m_b2vOldV = vNewV;
} //MakeSound