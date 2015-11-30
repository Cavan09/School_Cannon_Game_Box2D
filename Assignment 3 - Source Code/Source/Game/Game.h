//
//  Game.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef GAME_H
#define GAME_H

#include "Constants.h"
#include "OpenGL.h"
#include "Box2D.h"
#include "Cannon.h"

class GameObject;
class Game
{
public:
    //Singleton instance methods
    static Game* getInstance();
    
    //Update, paint and touch event (lifecycle) methods
    void update(double delta);
    void paint();
    void touchEvent(TouchEvent touchEvent, float locationX, float locationY, float previousX, float previousY);
    
    //Reset methods
    void reset();
    
    //Assignment 3 methods
    void fire();
    int getNumberOfBallsFired();
    float getTemperature();
    bool isGameOver();
    
    //Conveniance methods to access the screen's width and height
    float getScreenWidth();
    float getScreenHeight();
    float getScale();

    //Loading methods
    bool isLoading();
    
    //Box2D helper methods
    b2Body* createPhysicsBody(const b2BodyDef* bodyDef, const b2FixtureDef* fixtureDef = NULL);
    void destroyPhysicsBody(b2Body* body);
    
    b2Joint* createJoint(const b2JointDef* jointDef);
    void destroyJoint(b2Joint* joint);

private:
    //Private constructor and destructor ensures the singleton instance
    Game();
    ~Game();
    
    //Load method, called once every load step
    void load();
    void paintLoading();
    void PlaceBlock(float x, float y, const b2FixtureDef& fd);
    
    //Singleton instance static member variable
    static Game* m_Instance;
    
    //Load step member variable
    int m_LoadStep;
    
    //Box2D members
    b2World* m_World;
    b2DebugDraw* m_DebugDraw;
    
    //cannon
    Cannon* m_Cannon;
    std::vector<GameObject*> m_cubes;
};

#endif
