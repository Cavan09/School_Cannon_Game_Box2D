/// \file hud.h
/// \brief Interface for the heads-up display class CHeadsUpDisplay.

#pragma once

#include "ObjectManager.h"

/// \brief The heads-up display.
///
/// The HUD is responsible for managing parts of the heads-up display.

class CHeadsUpDisplay: public CObjectManager{
  public:
    BOOL m_bEasterEgg;

    CHeadsUpDisplay(int size); ///< Constructor.
    void draw(int secs, float temp, float maxtemp); ///< Draw HUD
}; //CHeadsUpDisplay