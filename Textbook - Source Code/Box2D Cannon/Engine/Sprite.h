/// \file sprite.h
/// \brief Interface for the sprite class C3DSprite.

#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

/// \brief The sprite. 
///
/// The sprite class contains basic information for drawing a
/// billboard sprite in Render World.

class C3DSprite{ //3D sprite
  friend class CSpriteManager;
  private:
    LPDIRECT3DTEXTURE9 m_lpTexture; ///< Pointer to the texture containing the sprite image.
    D3DXIMAGE_INFO m_structImageInfo; ///< Image information.
    LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;  ///< Vertex buffer.
    D3DXMATRIXA16 m_matWorld; ///< World matrix.
    D3DXMATRIXA16 m_matTemp; ///< temporary matrix.

  public:
    C3DSprite(); ///< Constructor.
    BOOL Load(LPDIRECT3DDEVICE9 device,char* filename); ///< Load texture image from file.
    void Draw(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 p, 
      float angle = 0.0f, float xscale=1.0f, float yscale=1.0f); ///< Draw sprite at point p in 3D space.
    void Release(); ///< Release sprite.
}; //C3DSprite