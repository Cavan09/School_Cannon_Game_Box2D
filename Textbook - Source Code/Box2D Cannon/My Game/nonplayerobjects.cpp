/// \file nonplayerobjects.cpp
/// \brief Code for the creation of nonplayer objects.
///
/// Creating things with a physics engine means a lot of tedious,
/// intricate initialization code. It makes sense to hide it in a file of
/// its own. It's not that it's so difficult, but that there is so much of it.
/// There are so many niggly little details to get right.

#include "gamedefines.h"
#include "Box2D\Box2D.h"
#include "ObjectWorld.h"

extern b2World g_b2dPhysicsWorld;
extern CObjectWorld g_cObjectWorld;

/// Create world edges in Physics World.
/// Place Box2D edge shapes in the Physics World in places that correspond to the
/// top, bottom, right, and left edges of the screen in Render World, and a
/// ledge for the cannon to sit upon. The left and  right edges continue upwards
/// for a distance. There is no top to the world.

void CreateWorldEdges(){
  float w, h;
  g_cObjectWorld.GetWorldSize(w, h);
  w = RW2PW(w);
  h = RW2PW(2.0f * h);

  const b2Vec2 vBottomLeft = b2Vec2(0, 0);
  const b2Vec2 vBottomRight = b2Vec2(w, 0);
  const b2Vec2 vTopLeft = b2Vec2(0, h);
  const b2Vec2 vTopRight = b2Vec2(w, h);

  //Box2D ground
  b2BodyDef bd;
  b2Body* edge = g_b2dPhysicsWorld.CreateBody(&bd);
  b2EdgeShape shape;  
  shape.Set(vBottomLeft, vBottomRight);
  edge->CreateFixture(&shape, 0);

  //Box2D left edge of screen
  shape.Set(vBottomLeft, vTopLeft);
  edge->CreateFixture(&shape, 0);

  //Box2D right edge of screen
  shape.Set(vBottomRight, vTopRight);
  edge->CreateFixture(&shape, 0); 

  //ledge
  const float lh = RW2PW(62); //ledge height
  shape.Set(b2Vec2(0, lh), b2Vec2(w/2, lh));
  edge->CreateFixture(&shape, 0);
} //CreateWorldEdges

/// Place a book in Physics World and Object World.
/// \param x Horizontal coordinate in Physics World units.
/// \param y  Vertical coordinate in Physics World units.
/// \param fd Book fixture definition.

void PlaceBook(float x, float y, const b2FixtureDef& fd){    
  b2BodyDef bd; //Physics World body definition
	bd.type = b2_dynamicBody;
  bd.position.Set(x, y);

  //Object World
  CGameObject* pGameObject = g_cObjectWorld.create(BOOK_OBJECT);

  //Physics World
  b2Body* pBook = g_b2dPhysicsWorld.CreateBody(&bd);
  pGameObject->SetPhysicsBody(pBook); //tell object world body about physics world body
  pBook->CreateFixture(&fd);
} //PlaceBook

/// Create a tower of books at the default location.

void CreateTower(){
  float w, h; //Object World width and height.
  g_cObjectWorld.GetWorldSize(w, h);

  //Box2D book shape and fixture
  b2PolygonShape bookshape;
  bookshape.SetAsBox(RW2PW(27), RW2PW(32));
  b2FixtureDef bookfd;
	bookfd.shape = &bookshape;
	bookfd.density = 1.0f;
	bookfd.restitution = 0.3f;

  //build tower
  for(int i=0; i<12; i++){
    float x = RW2PW(0.7f*w), y = RW2PW(32 + 64*i);
    if(i&1) //single block at odd-numbered layers   
      PlaceBook(x + RW2PW(30), y, bookfd);
    else{ //pair of blocks at even-numbered layers
      PlaceBook(x, y, bookfd);
      PlaceBook(x + RW2PW(60), y, bookfd);
    } //else
  } //for
} //CreateTower