/// \file sndlist.h
/// \brief Enumerated types for sounds.

#pragma once

/// Game sound enumerated type. 
/// These are the sounds used in actual gameplay. Sounds must be listed here in
/// the same order that they are in the sound settings XML file.

enum GameSoundType{ //sounds used in game engine
  THUMP_SOUND, THUMP2_SOUND, CANNONFIRE_SOUND, BOOM_SOUND, TICK_SOUND, 
  WIN_SOUND, LOSE_SOUND, CLANG_SOUND, R_SOUND //easter egg
};