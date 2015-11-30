/// \file SpriteMan.h
/// \brief Interface for the sprite manager class CSpriteManager.

#pragma once

#include "defines.h"
#include "sprite.h"
#include "tinyxml.h"


const int NUM_SPRITES = 512; ///< Maximum allowable number of sprites in the Sprite Manager.

/// \brief The sprite manager.

class CSpriteManager{
  private:
    C3DSprite* m_pSprite[NUM_SPRITES]; ///< Sprite pointers.
    char m_pBuffer[MAX_PATH]; ///< File name buffer.
    LPDIRECT3DDEVICE9 m_pd3dDevice;
    BOOL LoadFile(int sprite, const char* file); ///< Load sprite.
  public:
    CSpriteManager(); ///< Constructor
    ~CSpriteManager(); ///< Destructor
    void Load(int sprite, char* name); ///< Load sprite.
    void Draw(int sprite, D3DXVECTOR3 p, float angle=0.0f, float xscale=1.0f, float yscale=1.0f); ///< Draw sprite.
    void Release();  ///< Release textures.
    void SetDevice(LPDIRECT3DDEVICE9 device);  ///< Set the Direct3D device.
}; //CSpriteManager