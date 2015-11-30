//
//  Cannon.cpp
//  GameDevFramework
//
//  Created by Cavan Macphail on 2014-03-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Cannon.h"
#include "DeviceUtils.h"
#include "Game.h"
#include "Constants.h"


Cannon::Cannon()
{
    m_CannonBarrel = m_CannonBase = NULL;
    m_Wheel1 = m_Wheel2 = NULL;
    m_CannonWheelJoint1 = m_CannonWheelJoint2 = NULL;
    reset();
}

b2Body* Cannon::CreateCannonMount(int x, int y, int Index)
{
    RW2PW(x);
    RW2PW(y);
    
    b2PolygonShape shape;
    b2Vec2 verticies[3];
    const float s = RW2PW(64);
    verticies[0].Set(-s, -s);
    verticies[1].Set(s, -s);
    verticies[2].Set(0.0f, 0.0f);
    shape.Set(verticies, 3);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.restitution = 0.4f;
    fixtureDef.density = 1.0f;
    fixtureDef.filter.groupIndex = Index;
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(RW2PW(x), RW2PW(y));
    
    b2Body* body = Game::getInstance()->createPhysicsBody(&bodyDef);
    body->CreateFixture(&fixtureDef);
    return body;
}

b2Body* Cannon::CreateCannonBarrel(int x, int y, int Index)
{
    x = RW2PW(x);
    y = RW2PW(y);
    
    b2PolygonShape shape;
    shape.SetAsBox(RW2PW(70), RW2PW(16));
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.restitution = 0.2f;
    fixtureDef.density = 1.0f;
    fixtureDef.filter.groupIndex = Index;
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    
    b2Body* body = Game::getInstance()->createPhysicsBody(&bodyDef);
    body->CreateFixture(&fixtureDef);
    return body;
    
}

b2Body* Cannon::CreateWheel(int x,  int y, int Index)
{
    x = RW2PW(x);
    y = RW2PW(y);
    
    b2CircleShape shape;
    shape.m_radius = RW2PW(26);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.restitution = 0.6f;
    fixtureDef.density = 2.8f;
    fixtureDef.filter.groupIndex = Index;
    fixtureDef.friction = 1.0f;
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    
    b2Body* body = Game::getInstance()->createPhysicsBody(&bodyDef);
    body->CreateFixture(&fixtureDef);
    return body;
}

void Cannon::Create()
{
    const int Index = -42;
    const int x = 300;
    const int y = 62;
    
    m_CannonBase = CreateCannonMount(x - 10, y + 50, Index);
    m_CannonBarrel = CreateCannonBarrel(x + 30, y+ 64, Index);
    m_Wheel1 = CreateWheel(x - 30, y, Index);
    m_Wheel2 = CreateWheel(x + 30, y, Index);
    
    b2WheelJointDef wheelJointDef;
    b2Vec2 axis(1.0f,0.8f);
    wheelJointDef.Initialize(m_CannonBase, m_Wheel1, m_Wheel1->GetPosition(), axis);
    
    wheelJointDef.dampingRatio = 0.9f;
    wheelJointDef.motorSpeed = 0.0f;
    wheelJointDef.maxMotorTorque = 100.0f;
    wheelJointDef.enableMotor = true;
    
    m_CannonWheelJoint1 = (b2WheelJoint*)Game::getInstance()->createJoint(&wheelJointDef);
    
    wheelJointDef.Initialize(m_CannonBase, m_Wheel2, m_Wheel2->GetPosition(), axis);
    m_CannonWheelJoint2 = (b2WheelJoint*)Game::getInstance()->createJoint(&wheelJointDef);
    
    b2RevoluteJointDef jointDef;
    jointDef.Initialize(m_CannonBarrel, m_CannonBase, m_CannonBarrel->GetWorldCenter());
    jointDef.maxMotorTorque = 1.0f;
    jointDef.motorSpeed = 0.0f;
    jointDef.enableMotor = true;
    
    jointDef.lowerAngle = -b2_pi/4.0f;
    jointDef.upperAngle = 0.0f;
    jointDef.enableLimit = true;
    
    m_CannonBarrelJoint = (b2RevoluteJoint*)Game::getInstance()->createJoint(&jointDef);
    
    
    
}
void Cannon::Explode()
{
    if(m_CannonWheelJoint1)
    {
        Game::getInstance()->destroyJoint(m_CannonWheelJoint1);
        m_CannonWheelJoint1 = NULL;
    }
    if(m_CannonWheelJoint2)
    {
        Game::getInstance()->destroyJoint(m_CannonWheelJoint2);
        m_CannonWheelJoint2 = NULL;
    }
    if(m_CannonBarrelJoint)
    {
        Game::getInstance()->destroyJoint(m_CannonBarrelJoint);
        m_CannonBarrelJoint = NULL;
    }
    

    Impulse(m_CannonBase, b2Vec2(0.0f, 50.0f),b2Vec2(80.0f, 80.0f));
    Impulse(m_CannonBarrel, b2Vec2(0.0f, 100.0f), b2Vec2(40.0f,40.0f));
    Impulse(m_Wheel1, b2Vec2(-50.0f,200.0f), b2Vec2(1.0f,1.0f));
    Impulse(m_Wheel2, b2Vec2 (50.0f,220.0f), b2Vec2(-1.0f,-1.0f));
    
    ResetCollisionGroupIndex(m_CannonBarrel);
    ResetCollisionGroupIndex(m_CannonBase);
    ResetCollisionGroupIndex(m_Wheel1);
    ResetCollisionGroupIndex(m_Wheel2);
    
    m_CannonExploded = true;
    
    
}

bool Cannon::fire()
{
    
    if(!m_CannonExploded)
    {
        m_CannonTemp += 20.0f;
        
        b2CircleShape ball;
        ball.m_radius = RW2PW(16);
        
        b2FixtureDef ballFixtureDef;
        ballFixtureDef.shape = &ball;
        ballFixtureDef.density = 0.5f;
        ballFixtureDef.restitution = 0.3f;
        
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        
        b2Vec2 v = m_CannonBarrel->GetPosition() + b2Mul(b2Rot(m_CannonBarrel->GetAngle()), b2Vec2(RW2PW(45.0f),0.0f));
        
        bodyDef.position.Set(v.x,v.y);
        b2Body* cannonBall = Game::getInstance()->createPhysicsBody(&bodyDef);
        cannonBall->CreateFixture(&ballFixtureDef);
        
        StopMoving();
        b2Vec2 impulse = b2Mul(b2Rot(m_CannonBarrel->GetAngle()), b2Vec2(50.0f,0.0f));
        
        Impulse(cannonBall,impulse,b2Vec2(0.0f,0.0f));
        
        impulse *= -0.2;
        Impulse(m_CannonBarrel, impulse, b2Vec2(0.0f,0.0f));
        
        m_CannonBallsFired++;
        return true;
        
    }
    return false;
}
void Cannon::BarrelUp(float angle)
{
    m_CannonBarrel->SetTransform(m_CannonBarrel->GetPosition(), m_CannonBarrel->GetAngle() + angle);
}
void Cannon::StartMovingLeft(float speed)
{
    if(m_CannonWheelJoint1)
    {
        m_CannonWheelJoint1->SetMotorSpeed(speed);
        m_CannonWheelJoint1->EnableMotor(true);
    }
    if(m_CannonWheelJoint2)
    {
        m_CannonWheelJoint2->SetMotorSpeed(speed);
        m_CannonWheelJoint2->EnableMotor(true);
    }
    
}
void Cannon::StopMoving()
{
    if(m_CannonWheelJoint1)
    {
        m_CannonWheelJoint1->SetMotorSpeed(0.0f);
        m_CannonWheelJoint1->EnableMotor(false);
    }
    if(m_CannonWheelJoint2)
    {
        m_CannonWheelJoint2->SetMotorSpeed(0.0f);
        m_CannonWheelJoint2->EnableMotor(false);
    }
}

int Cannon::BallsFired()
{
    return m_CannonBallsFired;
}
bool Cannon::IsDead()
{
    return m_CannonExploded;
}
float Cannon::cannonTemp()
{
    return m_CannonTemp;
}
float Cannon::getBarrelX()
{
    return m_CannonBarrel->GetPosition().x;
    
}
float Cannon::getBarrelY()
{
    return m_CannonBarrel->GetPosition().y;
}
bool Cannon::checkLocation(float x, float y)
{
    b2Fixture* fixture = m_CannonBarrel->GetFixtureList();
    b2Vec2 position = b2Vec2(RW2PW(x), RW2PW(y));
    
    bool touch = fixture->TestPoint(position);
    return touch;
}
void Cannon::CoolDown()
{
    if(m_CannonTemp > m_CannonMaxTemp)
    {
        m_CannonMaxTemp = m_CannonTemp;
    }
    if(m_CannonTemp > 0.0f && !m_CannonExploded)
    {
        m_CannonTemp -= m_CannonTemp / 64.0f;
    }
    if(m_CannonTemp >= CANNONOVERHEAT && !m_CannonExploded)
    {
        Explode();
    }
}
void Cannon::reset()
{
    m_CannonTemp = m_CannonMaxTemp = 0;
    m_CannonBallsFired = 0;
    m_CannonExploded = false;
}
void Cannon::Impulse(b2Body* body, b2Vec2 velocity, b2Vec2 point)
{
    body->ApplyLinearImpulse(velocity, body->GetPosition() + point);
}
void Cannon::ResetCollisionGroupIndex(b2Body* body)
{
    b2Filter filter = body->GetFixtureList()->GetFilterData();
    filter.groupIndex = 0;
    body->GetFixtureList()->SetFilterData(filter);
}







