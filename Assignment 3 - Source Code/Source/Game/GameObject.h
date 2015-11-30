//
//  GameObject.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Game.h"
#include "OpenGL.h"


class OpenGLTexture;

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();
    
    virtual const char* getType() = 0;
    
    virtual void update(double delta);
    virtual void paint();
    
    virtual void reset();
    
    virtual void setX(float x);
    virtual void setY(float y);
    virtual void setPosition(float x, float y);
    
    virtual float getX();
    virtual float getY();
    virtual void getPosition(float& x, float& y);
    
protected:
    OpenGLTexture* m_Texture;
    float m_X;
    float m_Y;
};

#endif
