//
//  GameObject.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "GameObject.h"


GameObject::GameObject() :
    m_Texture(NULL),
    m_X(0.0f),
    m_Y(0.0f)
{
    
}

GameObject::~GameObject()
{
    if(m_Texture != NULL)
    {
        delete m_Texture;
        m_Texture = NULL;
    }
}

void GameObject::update(double aDelta)
{
    if(m_Texture != NULL)
    {
        if(m_Texture->isAnimated() == true)
        {
            OpenGLAnimatedTexture* animatedTexture = (OpenGLAnimatedTexture*)m_Texture;
            animatedTexture->update(aDelta);
        }
    }
}

void GameObject::paint()
{
    if(m_Texture != NULL)
    {
        OpenGLRenderer::getInstance()->drawTexture(m_Texture, m_X, m_Y);
    }
}

void GameObject::reset()
{
    setPosition(0.0f, 0.0f);
}

void GameObject::setX(float aX)
{
    m_X = aX;
}

void GameObject::setY(float aY)
{
    m_Y = aY;
}

void GameObject::setPosition(float aX, float aY)
{
    setX(aX);
    setY(aY);
}

float GameObject::getX()
{
    return m_X;
}

float GameObject::getY()
{
    return m_Y;
}

void GameObject::getPosition(float& aX, float& aY)
{
    aX = getX();
    aY = getY();
}
