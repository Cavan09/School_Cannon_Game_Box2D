/// \file object.h
/// \brief Interface for the game object class CGameObject.

#pragma once

#include "gamedefines.h"
#include "Box2D\Box2D.h"

/// \brief The game object.
///
/// Game objects are responsible for remembering information about themselves,
/// in particular, their representations in Render World and Physics World.

class CGameObject{ //class for a game object

  //CGameObject has a lot of friends
  friend class CObjectManager; //because the Object World manages objects
  friend class CHeadsUpDisplay; //because the HUD has objects in it too
  friend class CObjectWorld; //...and the Object World too, obviously
  friend class CRenderWorld; //because Render World need to draw them

  protected:
    GameObjectType m_nObjectType; ///< Object type.
    b2Body* m_pBody; ///< Physics World body.
    b2Vec2 m_b2vOldV; ///< Old velocity vector.

  public:
    CGameObject(GameObjectType objecttype); ///< Constructor.
    ~CGameObject(); ///< Destructor.
    void SetPhysicsBody(b2Body* b); ///< Set pointer to physics world body.
    void MakeSound(); ///< Make the appropriate sounds
}; //CGameObject