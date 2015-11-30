/// \file objectworld.h
/// \brief Interface for the object world class CObjectWorld.

#pragma once

#include "ObjectManager.h"
#include "hud.h"
#include "Cannon.h"


BOOL KeyboardHandler(WPARAM k); //to enable easter egg

/// \brief The object world.

class CObjectWorld{
  friend BOOL KeyboardHandler(WPARAM k); //to enable easter egg

  private:
    CObjectManager* m_pObjectManager; ///< Object manager.
    CHeadsUpDisplay* m_pHeadsUpDisplay; ///< HUD
    float m_fWidth, m_fHeight; ///< Object world width and height.

  public:
    CCannon m_cCannon; ///< The cannon.

    CObjectWorld(); //< Constructor.
    ~CObjectWorld(); //< Denstructor.

    CGameObject* create(GameObjectType t); ///< Create new object.
    CGameObject* CreateHudObject(GameObjectType t); ///< Create new object.

    void SetWorldSize(float width, float height); ///< Set the Object World width and depth.
    void GetWorldSize(float &width, float &height); ///< Get the Object World width and depth.

    BOOL PlayerHasWon(float level); ///< Detect whether the player has won.

    void clear(); //< Reset to initial conditions.
    void move(); //< Move all objects. 
    void draw(); //< Draw all objects.
    void MakeSound(); //< Make sounds for all objects.
};