/// \file renderer.h 
/// \brief Definition of the renderer class CRenderer.

#pragma once

#include "text.h"
#include "spriteman.h"

/// \brief The renderer.
///
/// The renderer class handles all the nasty Direct3D details that I don't
/// want readers to get into, or even be distracted by.

class CRenderer{

  friend void InitGame();

  protected:
    D3DPRESENT_PARAMETERS m_d3dpp; ///< Device specifications.
    IDirect3D9* m_d3d; ///< D3D object.
    IDirect3DSurface9* m_d3dbackbuffer; ///< Back buffer for rendering.
    CTextManager m_cTextManager; ///< Text manager.
    LPDIRECT3DVERTEXBUFFER9 m_pBackgroundVB;  ///< Vertex buffer for background.
    LPDIRECT3DTEXTURE9 m_pBackgroundTexture; ///< Texture for background.
    IDirect3DDevice9* m_d3ddevice; ///< Graphics device.
    CSpriteManager m_cSpriteManager; ///< Sprite manager.
    int m_nScreenWidth; ///< Screen width in pixels.
    int m_nScreenHeight; ///< Screen height in pixels.

    HRESULT CreateD3DDevice(HWND hwnd); ///< Create D3D device.
    void SetWorldMatrix(float dx=0.0f); ///< Set the world matrix.
    void SetViewMatrix(float x, float y, float z);  ///< Set the view matrix.
    void SetProjectionMatrix();  ///< Set the projection matrix.
    void SetAmbientLighting();  ///< Set ambient lighting.
    void SetTextureStates(); ///< Set texture stage states.
    void SetD3dStates(); ///< Set D3D states.
    BOOL RestoreDevice(); ///< Restore lost device.
    BOOL DeviceLost(); ///< Check for lost device.
    void ReleaseD3D(); ///< Release the D3D object.
    void LoadTexture(LPDIRECT3DTEXTURE9& texture, char* filename); ///< Load a texture.
    BOOL PageFlip(); ///< Graphics device page flip.
    void GetTextDC(COLORREF color); ///< Get Windows device context.
    void ReleaseTextDC(); ///< Release device context.
    void TextWrite(char *string, COLORREF color=RGB(255, 255, 255)); ///< Write text to screen.
    void InitBackgroundVertexBuffer(); ///< Initialize vertex buffer for background.
    void LoadBackground(); ///< Load the background image.

  public:
    CRenderer(); ///< Constructor.
    void Initialize(); ///< Initialize Direct3D.
    BOOL BeginScene(); ///< Start of frame.
    void DrawBackground(); ///< Draw the background to the screen.
    void EndScene(); ///< End of frame.
    void End(); ///< Release all DirectX resources.
    void Load(int sprite, char* name); ///< Load sprite.
    void draw(int sprite, float x, float y, float angle=0.0f, float xsize=1.0f, float ysize=1.0f); ///< Draw sprite.
    void drawtop(int sprite, float x, float y, float angle=0.0f, float size=1.0f); ///< Draw sprite at top of screen.
    void GetWorldSize(int &x, int &y); ///< Get the Render World width and height.
}; //CRenderer