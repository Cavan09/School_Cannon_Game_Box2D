/// \file Cannon.h
/// \brief Interface for the cannon class CCannon.

#pragma once

#include "gamedefines.h"
#include "Box2D\Box2D.h"
#include "ObjectManager.h"

const float CANNONEXPLODETEMP = 150.0f; ///< Temperature at which the cannon explodes when next fired.

/// \brief The cannon object.
///
/// The cannon object is a composite made up of several sprites in Render World
/// and several bodies in Physics World. It is intended to represent the player.

class CCannon{

  friend class CObjectWorld; //because the Object World needs to know cannon temp to draw guage.

  private:
    b2Body* m_pCannonBarrel; ///< Pointer to cannon barrel in Physics World.
    b2Body* m_pCannonBase; ///< Pointer to cannon base in Physics World.
    b2Body* m_pWheel1; ///< Pointer to cannon wheel in Physics World.
    b2Body* m_pWheel2; ///< Pointer to cannon wheel in Physics World.

    b2WheelJoint* m_pCannonWheelJoint1; ///< Pointer to cannon wheel joint in Physics World.
    b2WheelJoint* m_pCannonWheelJoint2;  ///< Pointer to cannon wheel joint in Physics World.
    b2RevoluteJoint* m_pCannonBarrelJoint;  ///< Pointer to cannon barrel body in Physics World.

    float m_fCannonTemp; ///< Cannon temperature.
    float m_fCannonMaxTemp; ///< Cannon maximum allowable temperature.
    int m_nCannonBallsFired; ///< Number of cannonballs fired in current level.
    BOOL m_bCannonExploded; ///< Whether cannon has exploded.

    b2Body* CreateCannonMount(int x, int y, int nIndex); ///< Create a cannon mount in Physics World.
    b2Body* CreateCannonBarrel(int x, int y, int nIndex);  ///< Create a cannon barrel in Physics World.
    b2Body* CreateWheel(int x, int y, int nIndex);  ///< Create a cannon wheel in Physics World.

    void Impulse(b2Body* b, b2Vec2& v, b2Vec2& ds=b2Vec2(0,0)); ///< Apply an impulse in Physics World.
    void ResetCollisionGroupIndex(b2Body* b); ///< Make cannon parts collide-able in Physics World
    void StopMoving(); ///< Stop the cannon moving under its own power.

  public:
    CCannon(); ///< Constructor.
    void create(); ///< Create a cannon.
    void Explode(); ///< Make cannon explode.

    //user control over cannon
    BOOL Fire(); ///< Fire the cannon.
    void BarrelUp(float angle); ///< Rotate the cannon barrel.
    void StartMovingLeft(float speed); ///< Start the cannon moving.

    int BallsFired(); ///< Get number of cannonballs fired in this level.
    BOOL IsDead(); ///< Whether cannon has exploded.
    void CoolDown(); ///< Cannon temperature drops over time.
    void Reset(); ///< Reset cannon to initial conditions.
}; //CCannon