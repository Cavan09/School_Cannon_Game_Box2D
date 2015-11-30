/// \file text.h
/// \brief Interface for the text manager class CTextManager.

#pragma once

#include <d3d9.h>

/// \brief The text manager.
///
/// Provides an interface for drawing text to a surface.

class CTextManager{ //screen text manager
  private:
    HDC m_hdc; ///< Device context.
    IDirect3DSurface9* m_pSurface; ///< Surface to write text to.
    COLORREF m_cOldTextColor; ///< Saved old text color, use to reset.
    HFONT m_hFont;
  public:
    void GetDC(IDirect3DSurface9* buffer, COLORREF color = RGB(255, 255, 255)); ///< Get device context.
    void ReleaseDC(); ///< Release device context.
    void Write(char* string); ///< Write string at cursor position.
}; //CTextManager