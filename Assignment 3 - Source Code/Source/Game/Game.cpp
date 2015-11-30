//
//  Game.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "Game.h"
#include "GameObject.h"
#include "DeviceUtils.h"
#include "MathUtils.h"
#include "PhysicsEditorWrapper.h"
#include <vector>


Game* Game::m_Instance = NULL;

Game* Game::getInstance()
{
    //Singleton design pattern ensures that there is only 1 instance of the game
    if(m_Instance == NULL)
    {
        m_Instance = new Game();
    }
    return m_Instance;
}

Game::Game() :
    m_LoadStep(0),
    m_World(NULL),
    m_DebugDraw(NULL)
{
    
}

Game::~Game()
{
    //Delete the debug draw instance
    if(m_DebugDraw != NULL)
    {
        delete m_DebugDraw;
        m_DebugDraw = NULL;
    }
    //Delete the Box2D world instance, MAKES SURE this is the last object deleted
    if(m_World != NULL)
    {
        //Destroy all the bodies in the world
        b2Body* body = m_World->GetBodyList();
        while(body != NULL)
        {
            //Destroy the body and get the next body (if there is one)
            b2Body* nextBody = body->GetNext();
            destroyPhysicsBody(body);
            body = nextBody;
        }
        
        //Finally delete the world
        delete m_World;
        m_World = NULL;
    }
}

void Game::load()
{
    switch(m_LoadStep)
    {
        case GameLoadStepInitial:
        {
            //TODO: Load game content required for future load steps here
        }
        break;
            
        case GameLoadStepWorld:
        {
            //Define the gravity vector.
            b2Vec2 gravity;
            gravity.Set(GAME_GRAVITY_X, GAME_GRAVITY_Y);
            
            //Construct the Box2d world object, which will
            //holds and simulates the rigid bodies
            m_World = new b2World(gravity);
            m_World->SetContinuousPhysics(GAME_PHYSICS_CONTINUOUS_SIMULATION);
            
            #if DEBUG
            //Create the debug draw for Box2d
            m_DebugDraw = new b2DebugDraw(b2Helper::box2dRatio());
            
            //Set the debug draw flags
            uint32 flags = 0;
            flags += b2Draw::e_shapeBit;
            flags += b2Draw::e_jointBit;
            flags += b2Draw::e_centerOfMassBit;
            m_DebugDraw->SetFlags(flags);
            
            //Set the Box2d world debug draw instance
            m_World->SetDebugDraw(m_DebugDraw);
            #endif

            //Define the ground body.
            b2BodyDef groundBodyDef;
            groundBodyDef.position.Set(0.0f, 0.0f); // bottom-left corner
            
            //Call the body factory which allocates memory for the ground body
            //from a pool and creates the ground box shape (also from a pool).
            //The body is also added to the world.
            b2Body* groundBody = createPhysicsBody(&groundBodyDef);
            
            //Convert the screen width and height to meters
            float width = b2Helper::screenSpaceToBox2dSpace(DeviceUtils::getScreenResolutionWidth());
            float height = b2Helper::screenSpaceToBox2dSpace(DeviceUtils::getScreenResolutionHeight());
            
            //Define the ground box shape.
            b2EdgeShape groundShape;
            groundShape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(width, 0.0f));
            groundBody->CreateFixture(&groundShape, 0.0f);
            
            groundShape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, height));
            groundBody->CreateFixture(&groundShape, 0.0f);
            
            groundShape.Set(b2Vec2(width, 0.0f), b2Vec2(width, height));
            groundBody->CreateFixture(&groundShape, 0.0f);
        }
        break;
            
        case GameLoadStepTower:
        {
            b2PolygonShape box;
            box.SetAsBox(RW2PW(26), RW2PW(32));
            b2FixtureDef boxfd;
            boxfd.shape = &box;
            boxfd.density = 1.0f;
            boxfd.restitution = 0.0f;
            
            for( int i = 0; i < 10; i++)
            {
                float x = RW2PW(0.7f * DeviceUtils::getScreenResolutionWidth());
                float y = RW2PW(32 + 64 * i);
                
                if(i & 1)
                {
                    PlaceBlock(x + RW2PW(30), y, boxfd);
                }
                else
                {
                    PlaceBlock(x, y, boxfd);
                    PlaceBlock(x + RW2PW(60), y, boxfd);
                }
            }
            
            
            
        }
        break;
            
        case GameLoadStepCannon:
        {
            m_Cannon = new Cannon();
            m_Cannon->Create();
        }
        break;
            
        case GameLoadStepFinal:
        {
            reset();
        }
        break;
            
        default:
            break;
    }
    
    //Increment the load step
    m_LoadStep++;
}

void Game::update(double aDelta)
{
    //While the game is loading, the load method will be called once per update
    if(isLoading() == true)
    {
        load();
        return;

    }
    //Step the Box2D world this update cycle
    if(m_World != NULL)
    {
        m_World->Step(aDelta, GAME_PHYSICS_VELOCITY_ITERATIONS, GAME_PHYSICS_POSITION_ITERATIONS);
    }
    m_Cannon->CoolDown();
}

void Game::paint()
{
    //While the game is loading, the load method will be called once per update
    if(isLoading() == true)
    {
        paintLoading();
        return;
    }
    
#if DEBUG
    if(m_World != NULL)
    {
        m_World->DrawDebugData();
    }
#endif
}

void Game::paintLoading()
{
    //Cache the screen width and height
    float screenWidth = getScreenWidth();
    float screenHeight = getScreenHeight();

    //Draw a black background, you could replace this
    //in the future with a full screen background texture
    OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorBlack());
    OpenGLRenderer::getInstance()->drawRectangle(0.0f, 0.0f, screenWidth, screenHeight);
    
    //Calculate the bar width and height, don't actually hard-code these
    float barWidth = 200.0f * getScale();
    float barHeight = 40.0f * getScale();
    float barX = (screenWidth - barWidth) / 2.0f;
    float barY = (screenHeight - barHeight) / 2.0f;
    
    float percentageLoaded = (float)m_LoadStep / (float)(GameLoadStepCount - 1);
    float loadedWidth = barWidth * percentageLoaded;
    OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorYellow());
    OpenGLRenderer::getInstance()->drawRectangle(barX, barY, loadedWidth, barHeight);
    
    OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorWhite());
    OpenGLRenderer::getInstance()->drawRectangle(barX, barY, barWidth, barHeight, false);
}

void Game::touchEvent(TouchEvent touchEvent, float locationX, float locationY, float previousX, float previousY)
{
    if(touchEvent == TouchEventBegan || touchEvent == TouchEventMoved)
    {
        if(m_Cannon->checkLocation(locationX, locationY) == true)
        {
            float angle = locationY - previousY;
            m_Cannon->BarrelUp(RW2PW(angle));
        }
        else
        {
            if(RW2PW(locationX) > m_Cannon->getBarrelX())
            {
                
                m_Cannon->StartMovingLeft(-1);
            }
        else if(RW2PW(locationX) < m_Cannon->getBarrelX())
            {
                m_Cannon->StartMovingLeft(1);
            }
        }
        
    }
    if(touchEvent == TouchEventEnded)
    {
        m_Cannon->StartMovingLeft(0.0f);
    }
}

void Game::reset()
{
    m_Cannon->reset();
}

void Game::fire()
{
    m_Cannon->fire();
}

int Game::getNumberOfBallsFired()
{
    
    return m_Cannon->BallsFired();
}

float Game::getTemperature()
{
    //TODO: Return the temperature of the cannon
    return m_Cannon->cannonTemp();
}

bool Game::isGameOver()
{
    //TODO: Return wether the game is over

    return m_Cannon->IsDead();
}

b2Body* Game::createPhysicsBody(const b2BodyDef* bodyDef, const b2FixtureDef* fixtureDef)
{
    if(bodyDef != NULL)
    {
        b2Body* body = m_World->CreateBody(bodyDef);
        
        if(fixtureDef != NULL)
        {
            body->CreateFixture(fixtureDef);
        }
        
        return body;
    }
    return NULL;
}

void Game::destroyPhysicsBody(b2Body* body)
{
    //Safety check that aBody isn't NULL
    if(body != NULL)
    {
        //Destroy all the fixtures attached to the body
        b2Fixture* fixture = body->GetFixtureList();
        while(fixture != NULL)
        {
            b2Fixture* nextFixture = fixture->GetNext();
            body->DestroyFixture(fixture);
            fixture = nextFixture;
        }
        
        //Destroy the body
        m_World->DestroyBody(body);
    }
}

b2Joint* Game::createJoint(const b2JointDef* jointDef)
{
    if(jointDef != NULL)
    {
        return m_World->CreateJoint(jointDef);
    }
    return NULL;
}

void Game::destroyJoint(b2Joint* joint)
{
    if(joint != NULL)
    {
        m_World->DestroyJoint(joint);
    }
}

void Game::PlaceBlock(float x, float y, const b2FixtureDef& fd)
{
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(x,y);
    
    b2Body* box = createPhysicsBody(&bd);
    box->CreateFixture(&fd);
    
}

float Game::getScreenWidth()
{
    return DeviceUtils::getScreenResolutionWidth();
}

float Game::getScreenHeight()
{
    return DeviceUtils::getScreenResolutionHeight();
}

float Game::getScale()
{
    return DeviceUtils::getContentScaleFactor();
}

bool Game::isLoading()
{
    return m_LoadStep < GameLoadStepCount;
}
