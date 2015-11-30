/// \file ObjectManager.h
/// \brief Interface for the object manager class CObjectManager.

#pragma once

#include "gamedefines.h"
#include "object.h"

/// \brief The object manager.
///
/// The object manager is responsible for managing game objects.

class CObjectManager{
  friend class CObjectWorld;
  protected:
    CGameObject** m_pObjectList; ///< List of game objects.
    int m_nCount; ///< How many objects in list.
    int m_nSize; ///< Maximum size of list.
  public:
    CObjectManager(int size); ///< Constructor
    ~CObjectManager(); ///< Destructor
    CGameObject* create(GameObjectType objecttype); ///< Create new object.
    void move(); ///< Move all objects.
    void draw(); ///< Draw all objects.
    void clear(); ///< Clear the object list.
    void MakeSound(); ///< Make sounds.
}; //CObjectManager