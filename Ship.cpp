#include "Frog.h"
#include "MainGame.h"
#include "MainUpdate.h"
#include "FrogMath.h"
#include "Ship.h"

using namespace Webfoot;


Ship::Ship()
{
	ship = NULL;
	balltest = NULL;
}

//----------------------------
void Ship::Init()
{
	ship = frog_new Sprite();
	ship->Init("Sprites/Sprites", "Ship");

	balltest = theImages->Load("Ball");

	position = Point2F::Create(theScreen->SizeGet() / 2);
	acceleration = Point2F::Create(0.0f, 0.0f);
	theta = 0;
	direction_changed = FALSE;
}

void Ship::Deinit()
{
	if (ship){
		SmartDeinitDelete(ship);
	}
	if (balltest){
		theImages->Unload(balltest);
		balltest = NULL;
	}
}

void Ship::Update(unsigned int dt)
{
	float dtSeconds = (float)dt / 1000.f;

	position += acceleration * dtSeconds;
	ship->PositionSet(position -(Point2F::Create(0,ship->SizeGet(1).y/2)));
	ship->Update(dt);
	ballpos = Point2F::Create(position-(ship->SizeGet(1)));
	position_get = position;

	if (position.y < 0){position.y = theScreen->SizeGet().y;}
	if (position.y > theScreen->SizeGet().y){position.y = 0;}
	if (position.x < 0){ position.x = theScreen->SizeGet().x;}
	if (position.x > theScreen->SizeGet().x){ position.x = 0; }



	Input = theKeyboard->KeyPressed();
	if (theKeyboard->KeyPressed(KEY_UP))
	{
		DebugPrintf("Up arrow pressed\n");
		acceleration.Set(150.0f, 0.0f);
	}
	if (theKeyboard->KeyPressed(KEY_LEFT))
	{
		theta += 1;
		direction_changed = TRUE;
	}
	if (theKeyboard->KeyJustPressed(KEY_DOWN)){
		acceleration.Set(0.0f, 0.0f);
	}
	if (theKeyboard->KeyPressed(KEY_RIGHT))
	{
		theta -= 1;
		direction_changed = TRUE;
	}
	ship->RotationSet(theta);
}

void Ship::Draw()
{
	ship->Draw();
	balltest->Draw(ballpos);

}

