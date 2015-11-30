/// \file Cannon.cpp 
/// \brief Code for the cannon class CCannon.

#include "cannon.h"
#include "ObjectWorld.h"
#include "gamedefines.h"

extern CObjectWorld g_cObjectWorld;
extern b2World g_b2dPhysicsWorld;
extern CTimer g_cTimer;

CCannon::CCannon(){
  m_pCannonBarrel = m_pCannonBase = NULL;
  m_pWheel1 = m_pWheel2 = NULL;
  m_pCannonWheelJoint1 = m_pCannonWheelJoint2 = NULL;
  Reset();
} //constructor

/// Create a cannon mount in Physics World.
/// \param x Cannon mount's x coordinate in Render World.
/// \param y Cannon mount's y coordinate in Render World.
/// \param nIndex Cannon mount's collision group index.
/// \return Pointer to cannon mount body in Physics World.

b2Body* CCannon::CreateCannonMount(int x, int y, int nIndex){
  //shape
  b2PolygonShape shape;
  b2Vec2 vertices[3];
  const float s = RW2PW(64);
  vertices[0].Set(-s, -s);
  vertices[1].Set(s, -s);
  vertices[2].Set(0.0f, 0.0f);
  shape.Set(vertices, 3);

  //fixture
  b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;
	fd.restitution = 0.4f;
  fd.filter.groupIndex = nIndex;    

  //body definition
  b2BodyDef bd;
	bd.type = b2_dynamicBody;
  bd.position.Set(RW2PW(x), RW2PW(y));
  
  //body
  b2Body* body = g_b2dPhysicsWorld.CreateBody(&bd);
  body->CreateFixture(&fd);
  return body;
} //CreateCannonMount

/// Create a cannon barrel in Physics World.
/// \param x Cannon barrel's x coordinate in Render World.
/// \param y Cannon barrel's y coordinate in Render World.
/// \param nIndex Cannon barrel's collision group index.
/// \return Pointer to cannon barrel body in Physics World.

b2Body* CCannon::CreateCannonBarrel(int x, int y, int nIndex){ 
  //shape
  b2PolygonShape shape;
  shape.SetAsBox(RW2PW(67), RW2PW(22));

  //fixture
  b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;
	fd.restitution = 0.2f;  
  fd.filter.groupIndex = nIndex;
  
  //body definition
  b2BodyDef bd;
	bd.type = b2_dynamicBody;
  bd.position.Set(RW2PW(x), RW2PW(y));

  //body
  b2Body* body = g_b2dPhysicsWorld.CreateBody(&bd);
  body->CreateFixture(&fd);
  return body;
} //CreateCannonBarrel

/// Create a cannon wheel  in Physics World.
/// \param x Cannon wheel's x coordinate in Render World.
/// \param y Cannon wheel's y coordinate in Render World.
/// \param nIndex Cannon wheel's collision group index.
/// \return Pointer to cannon wheel body in Physics World.

b2Body* CCannon::CreateWheel(int x, int y, int nIndex){
  //shape
  b2CircleShape shape;
	shape.m_radius = RW2PW(16);

  //fixture
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 0.8f;
	fd.restitution = 0.6f;
  fd.filter.groupIndex = nIndex;

  //body definition
  b2BodyDef bd;
	bd.type = b2_dynamicBody;
  bd.position.Set(RW2PW(x), RW2PW(y));

  //body
  b2Body* body;
  body = g_b2dPhysicsWorld.CreateBody(&bd);
  body->CreateFixture(&fd);
  return body;
} //CreateWheel

/// Create a cannon in Physics World and Object World, linking
/// the two representations together.

void CCannon::create(){
  const int nIndex = -42; //Collision index. Negative means don't collide.
  const int nX = 300; 
  const int nY = 62;

  //Create cannon parts in Object World
  CGameObject* pBarrel = g_cObjectWorld.create(CANNONBARREL_OBJECT);
  CGameObject* pMount = g_cObjectWorld.create(CANNONMOUNT_OBJECT);
  CGameObject* pWheel1 = g_cObjectWorld.create(WHEEL_OBJECT);
  CGameObject* pWheel2 = g_cObjectWorld.create(WHEEL_OBJECT);

  //Create cannon parts in Physics World

  //base, barrel, and wheels
  m_pCannonBase = CreateCannonMount(nX, nY + 84, nIndex);
  m_pCannonBarrel = CreateCannonBarrel(nX, nY + 72, nIndex);
  m_pWheel1 = CreateWheel(nX - 30, nY + 16, nIndex);
  m_pWheel2 = CreateWheel(nX + 30, nY + 16, nIndex);
  
  //wheel joint definition
  b2WheelJointDef wd;
  b2Vec2 axis(0.0f, 0.9f); //vertical axis for wheel suspension
  wd.Initialize(m_pCannonBase, m_pWheel1, m_pWheel1->GetPosition(), axis);
  wd.dampingRatio = 0.9f;
  wd.motorSpeed = 0.0f;
  wd.maxMotorTorque = 1000.0f;
  wd.enableMotor = TRUE;

  //create wheel joint for wheel 1
  m_pCannonWheelJoint1 = (b2WheelJoint*)g_b2dPhysicsWorld.CreateJoint(&wd);

  //create wheel joint for wheel 2
  wd.Initialize(m_pCannonBase, m_pWheel2, m_pWheel2->GetPosition(), axis);
  m_pCannonWheelJoint2 = (b2WheelJoint*)g_b2dPhysicsWorld.CreateJoint(&wd);

  //revolute joint definition
  b2RevoluteJointDef jointDef;
  jointDef.Initialize(m_pCannonBarrel, m_pCannonBase, m_pCannonBarrel->GetWorldCenter());
  jointDef.maxMotorTorque = 1000.0f;
  jointDef.motorSpeed = 0.0f;
  jointDef.enableMotor = true;
  jointDef.lowerAngle = -b2_pi/4.0f;
  jointDef.upperAngle = 0.0f;
  jointDef.enableLimit = TRUE;

  //create revolute joint
  m_pCannonBarrelJoint = (b2RevoluteJoint*)g_b2dPhysicsWorld.CreateJoint(&jointDef);
  
  //tell Object World cannon parts about Physics World counterparts
  pBarrel->SetPhysicsBody(m_pCannonBarrel);
  pMount->SetPhysicsBody(m_pCannonBase);
  pWheel1->SetPhysicsBody(m_pWheel1);
  pWheel2->SetPhysicsBody(m_pWheel2);
} //create

/// Rotate the cannon barrel up or down by a small increment.
/// \param angle Angle increment in radians. Positive means up, negative means down.

void CCannon::BarrelUp(float angle){
  m_pCannonBarrel->SetTransform(m_pCannonBarrel->GetPosition(), 
    m_pCannonBarrel->GetAngle() + angle);
} //BarrelUp

/// Start the cannon moving using motors in the wheels.
/// \param speed Speed of cannon. Positive means left, negative means right.

void CCannon::StartMovingLeft(float speed){
  if(m_pCannonWheelJoint1){
    m_pCannonWheelJoint1->SetMotorSpeed(speed);
    m_pCannonWheelJoint1->EnableMotor(TRUE);
  } //if
  if(m_pCannonWheelJoint2){
    m_pCannonWheelJoint2->SetMotorSpeed(speed);
    m_pCannonWheelJoint2->EnableMotor(TRUE); 
  } //if
} //StartMovingLeft

/// Stop the cannon by setting motor speed to zero and disabling the motors.

void CCannon::StopMoving(){
  if(m_pCannonWheelJoint1){
    m_pCannonWheelJoint1->SetMotorSpeed(0.0f);
    m_pCannonWheelJoint1->EnableMotor(FALSE);
  } //if
  if(m_pCannonWheelJoint2){
    m_pCannonWheelJoint2->SetMotorSpeed(0.0f);
    m_pCannonWheelJoint2->EnableMotor(FALSE); 
  } //if
} //StopMoving

/// Apply an impulse to part of the cannon, which is made up of many bodies in Physics World.
/// \param b Body to apply impulse to. 
/// \param v Vector direction and magnitude of impulse.
/// \param ds Vector dispolacement to point at which impulse is applied.

void CCannon::Impulse(b2Body* b, b2Vec2& v, b2Vec2& ds){
  b->ApplyLinearImpulse(v, b->GetPosition() + ds);
}; //Impulse

/// Reset the collision group index for part of cannon, which is made up of many bodies in Physics World.
/// \param b Body to reset collision group index of. 

void CCannon::ResetCollisionGroupIndex(b2Body* b){
  b2Filter f = b->GetFixtureList()->GetFilterData(); 
  f.groupIndex = 0;
  b->GetFixtureList()->SetFilterData(f); 
} //ResetCollisionGroupIndex

/// Make the cannon explode by destroying the joints, applying impulses to cannon parts so that they
/// fly apart, and resetting their collision group indices so that they can collide.

void CCannon::Explode(){
  //break joints
  if(m_pCannonWheelJoint1){ 
    g_b2dPhysicsWorld.DestroyJoint(m_pCannonWheelJoint1);
    m_pCannonWheelJoint1 = NULL;
  }
  if(m_pCannonWheelJoint2){
    g_b2dPhysicsWorld.DestroyJoint(m_pCannonWheelJoint2);
    m_pCannonWheelJoint2 = NULL;
  }
  if(m_pCannonBarrelJoint){
    g_b2dPhysicsWorld.DestroyJoint(m_pCannonBarrelJoint);
    m_pCannonBarrelJoint = NULL;     
  }

  //apply impulses to cannon parts so that they fly apart
  Impulse(m_pCannonBase,
    b2Vec2(0, 50), b2Vec2(80, 80));
  Impulse(m_pCannonBarrel,
    b2Vec2(0, 100), b2Vec2(40, 40));
  Impulse(m_pWheel1,
    b2Vec2(-50, 200), b2Vec2(1, 1));
  Impulse(m_pWheel2,
    b2Vec2(50, 220), b2Vec2(-1, -1));

  //allow cannon components to collide with each other
  ResetCollisionGroupIndex(m_pCannonBase);
  ResetCollisionGroupIndex(m_pCannonBarrel);
  ResetCollisionGroupIndex(m_pWheel1);
  ResetCollisionGroupIndex(m_pWheel2);

  m_bCannonExploded = TRUE;
} //Explode 

/// If enough time has passed for reloading, create a cannonball
/// in both Render World and Physics World and send it on its way. Apply a recoil
/// impulse to the cannon in Physics World. Increment the cannon's temperature.
/// All this is assuming that the cannon isn't nose down and hasn't already exploded.

BOOL CCannon::Fire(){      
  static int nLastFireTime=0;
  if(!m_bCannonExploded && g_cTimer.elapsed(nLastFireTime, 250)){
    m_fCannonTemp += 50.0f;

    //shape
    b2CircleShape ballshape;
	  ballshape.m_radius = RW2PW(16);

    //fixture
	  b2FixtureDef ballfd;
	  ballfd.shape = &ballshape;
	  ballfd.density = 0.5f;
	  ballfd.restitution = 0.3f;

    //body definition
    b2BodyDef bd;
	  bd.type = b2_dynamicBody;
    b2Vec2 v = m_pCannonBarrel->GetPosition() +
      b2Mul(b2Rot(m_pCannonBarrel->GetAngle()), b2Vec2(RW2PW(85),0));
    bd.position.Set(v.x, v.y);

    //now we're through with the prep, fire the bloody thing
    if(v.y < 0.0f) //cannon is barrel-to-ground
      Explode(); //B O O M. Bad news.
    else{ //barrel is clear, go ahead and fire
      //create cannonball in Object World
      CGameObject* pGameObject = g_cObjectWorld.create(BALL_OBJECT);

      //Physics World body for cannonball
	    b2Body* pCannonball = g_b2dPhysicsWorld.CreateBody(&bd);
      pGameObject->SetPhysicsBody(pCannonball); //tell object world body about physics world body
      pCannonball->CreateFixture(&ballfd);

      //apply impulses to ball and cannon barrel
      StopMoving();
      b2Vec2 vImpulse = b2Mul( //impulse vector 
        b2Rot(m_pCannonBarrel->GetAngle()), 
        b2Vec2(200, 0)); //200 kluged for fPhysicsRescaleValue=10
      Impulse(pCannonball, vImpulse); //fire ball   
      Impulse(m_pCannonBarrel, -0.5f * vImpulse); //recoil
      m_nCannonBallsFired++;
      return TRUE;
    } //else
  } //if
  return FALSE;
} //Fire

/// Return the number of cannonballs fired in this level of the game.
/// \return Number of cannonballs fired.

int CCannon::BallsFired(){
  return m_nCannonBallsFired;
} //BallsFired

/// Cool the cannon's temperature, unless it is too high already,
/// in which case mark it for exploding on the next fire.

void CCannon::CoolDown(){
  if(m_fCannonTemp > m_fCannonMaxTemp) //recompute max temp
    m_fCannonMaxTemp = m_fCannonTemp;
  if(m_fCannonTemp > 0) //cool off
    m_fCannonTemp -= m_fCannonTemp/64.0f;
  if(m_fCannonTemp >= CANNONEXPLODETEMP && !m_bCannonExploded) //too hot
    Explode();
} //CoolDown

/// Determine whether the cannon has exploded already.
/// \return TRUE If it's dead, Dave.

BOOL CCannon::IsDead(){
  return m_bCannonExploded;
} //IsDead

/// Reset the cannon to its initial conditions ready for a new level to begin.

void CCannon::Reset(){
  m_fCannonTemp = m_fCannonMaxTemp = 0.0f;
  m_nCannonBallsFired = 0;
  m_bCannonExploded = FALSE;
} //Reset