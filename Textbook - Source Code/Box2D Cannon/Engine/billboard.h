/// \file billboard.h
/// \brief Billboard vertex flexible vertex format.

#pragma once

#include "defines.h"

/// \brief Billboard vertex structure. 
/// Custom vertex format for representing a vanilla billboard object. Consists 
/// of position, texture coordinates, and flexible vertex format.

struct BILLBOARDVERTEX{ 
  D3DXVECTOR3 p; ///< Position.
  float tu,tv; ///< Texture coordinates.
  static const DWORD FVF; ///< Flexible vertex format.
};