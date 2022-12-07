#include "Frog.h"
#include "MainGame.h"
#include "MainUpdate.h"
#include "FrogMath.h"
#include "Ship.h"
#include "Sprite.h"

using namespace Webfoot;


Ship::Ship()
{
	ship = NULL;
}

//----------------------------
void Ship::Init()
{
	ship = frog_new Sprite();
	ship->Init("Sprites/Sprites", "Ship");
	//ship->RotationSet();

	position = Point2F::Create(theScreen->SizeGet() / 2);
	//starts at 0,0
	velocity = { 0.0, 0.0 };
	rotation = 0.0;
}


void Ship::Deinit()
{
	if (ship){
		SmartDeinitDelete(ship);
	}

}


void Ship::Update(unsigned int dt)
{
	float dtSeconds = (float)dt / 1000.f;
	//sets ship in middle of screen
	ship->PositionSet(position - (Point2F::Create(0, ship->SizeGet(1).y / 2)));
	//have to update so it animamtes
	ship->Update(dt);


	//keeps the ship on the screen
	if (position.y < 0){position.y = theScreen->SizeGet().y;}
	if (position.y > theScreen->SizeGet().y){position.y = 0;}
	if (position.x < 0){ position.x = theScreen->SizeGet().x;}
	if (position.x > theScreen->SizeGet().x){ position.x = 0; }


	//where we move the ship
	Input = theKeyboard->KeyPressed();
	if (theKeyboard->KeyPressed(KEY_W))
	{
		velocity.x = position.x + 4*FrogMath::Cos(-90 - rotation);
		velocity.y = position.y + 4*FrogMath::Sin(-90 - rotation);

		position = velocity;
	
	}

	if (theKeyboard->KeyPressed(KEY_A))
	{
		rotation += 2.00;
		ship->RotationSet(rotation);

	}
	if (theKeyboard->KeyPressed(KEY_D))
	{
		rotation -= 2.00;
		ship->RotationSet(rotation);
	}
	
}
Point2F Ship::position_get()
{
	return position;
}
Point2F Ship::velocity_get()
{
	return velocity;
}
float Ship::rotation_get()
{
	return rotation;
}

void Ship::Draw()
{
	ship->Draw();

}
void Ship::deactivate()
{
	ship->VisibleSet(false);


}
void Ship::activate()
{
	ship->VisibleSet(true);


}


