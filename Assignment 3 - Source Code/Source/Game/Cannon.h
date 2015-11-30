//
//  Cannon.h
//  GameDevFramework
//
//  Created by Cavan Macphail on 2014-03-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDevFramework__Cannon__
#define __GameDevFramework__Cannon__

#include <iostream>
#include "Box2D.h"

class Cannon
{
public:
    Cannon();
    ~Cannon();
    
    void Create();
    void Explode();
    
    bool fire();
    void BarrelUp(float angle);
    void StartMovingLeft(float speed);
    void StopMoving();
    float getBarrelX();
    float getBarrelY();
    
    int BallsFired();
    bool checkLocation(float x, float y);
    bool IsDead();
    float cannonTemp();
    void CoolDown();
    void reset();
    
private:
    b2Body* CreateCannonMount(int x, int y, int Index);
    b2Body* CreateCannonBarrel(int x, int y, int Index);
    b2Body* CreateWheel(int x, int y, int Index);
    
    void Impulse(b2Body* body, b2Vec2 velocity, b2Vec2 point);
    void ResetCollisionGroupIndex(b2Body* body);

    
    b2Body* m_CannonBarrel;
    b2Body* m_CannonBase;
    b2Body* m_Wheel1;
    b2Body* m_Wheel2;
    
    b2WheelJoint* m_CannonWheelJoint1;
    b2WheelJoint* m_CannonWheelJoint2;
    b2RevoluteJoint* m_CannonBarrelJoint;
    
    float m_CannonTemp;
    float m_CannonMaxTemp;
    int m_CannonBallsFired;
    bool m_CannonExploded;
    
};


#endif /* defined(__GameDevFramework__Cannon__) */
