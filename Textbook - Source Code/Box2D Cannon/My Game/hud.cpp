/// \file hud.cpp
/// \brief Code for the heads-up display class CHeadsUpDisplay.

#include "hud.h"

#include "gamedefines.h"
#include "renderworld.h"

extern CRenderWorld g_cRenderWorld;

CHeadsUpDisplay::CHeadsUpDisplay(int size): CObjectManager(size){
} //constructor

/// Draw HUD, which includes a clock and a thermometer.
/// \param secs Number of seconds to display on clock.
/// \param temp Temperature to display on thermometer.
/// \param maxtemp Maximimum temperature that thermometer can display.
    
void CHeadsUpDisplay::draw(int secs, float temp, float maxtemp){

  CGameObject* p=NULL; //handy object pointer
  secs = secs % 60; //to be sure

  for(int i=0; i<m_nSize; i++){ //for each object slot
    p = m_pObjectList[i]; //object in the slot
    if(p){ //if there's really an object there
      BOOL bDraw = FALSE; //don't draw unless we're sure
      float x=0.0f, y=0.0f, a=0.0f; //screen position and angle
      switch(p->m_nObjectType){ //depending on what type it is
        //temperature guage objects
          case TEMPGUAGE_OBJECT:
            bDraw = TRUE; x = 140.0f; y = 40.0f;
            break;       
          case TEMPMAXNEEDLE_OBJECT:
            bDraw = TRUE; x = 25 + min(300.0f, 256.0f * maxtemp/178.0f); y = 40.0f;
            break;     
          case TEMPNEEDLE_OBJECT:
            bDraw = TRUE; x = 25 + min(300.0f, 256.0f * temp/178.0f); y = 40.0f;
            break;
         //clock objects
          case CLOCKFACE_OBJECT:
            bDraw = TRUE; x = 350.0f; y = 70.0f;
            break;  
          case CLOCKNEEDLE_OBJECT:
            bDraw = TRUE; x = 350.0f; y = 70.0f;
            a =  ((float)secs/60.0f)*2.0f*b2_pi;
            break; 
          case PIRATE_OBJECT: //easter egg
            if(m_bEasterEgg)
              g_cRenderWorld.draw(p->m_nObjectType, 80.0f,  128.0f);
            return; //bail 
      } //switch object type

      //now draw with y measured down from the top of the screen
      if(bDraw)g_cRenderWorld.drawtop(p->m_nObjectType, x, y, a);
    } //if p is not NULL
  } //for each object
} //draw