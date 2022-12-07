#include "Frog.h"
#include "MainGame.h"
#include "MainUpdate.h"
#include "FrogMath.h"
#include "Asteroids.h"
#include "Sprite.h"

using namespace Webfoot;
Asteroids_big::Asteroids_big()
{
	asteroids_big = NULL;
}
Asteroids_medium::Asteroids_medium()
{
	asteroids_medium = NULL;
}
Asteroids_small::Asteroids_small()
{
	asteroids_small = NULL;
}

//----------------------------
void Asteroids_big::Init()
{
	asteroids_big = frog_new Sprite();
	asteroids_big->Init("Sprites/Sprites", "Asteroids_Big");
	asteroids_big->VisibleSet(true);
	isactive = true;

	position = Point2F::Create(theScreen->SizeGet() / 2);
	speed = ((rand() % 1)+1);
	velocity = Point2F::Create((rand() % 5)+1, (rand() % 5)+1);
	
}
void Asteroids_medium::Init()
{
	asteroids_medium = frog_new Sprite();
	asteroids_medium->Init("Sprites/Sprites", "Asteroids_Medium");
	isactive = false;
	asteroids_medium->VisibleSet(false);

	position = Point2F::Create(theScreen->SizeGet() / 2);

	speed = ((rand() % 1) + 1);
	velocity = Point2F::Create((rand() % 5) + 1, (rand() % 5) + 1);

}
void Asteroids_small::Init()
{
	asteroids_small = frog_new Sprite();
	asteroids_small->Init("Sprites/Sprites", "Asteroids_Small");
	isactive = false;
	asteroids_small->VisibleSet(false);
	position = Point2F::Create(theScreen->SizeGet() / 2);

	speed = (rand() % 1) + 1;
	velocity = Point2F::Create((rand() % 5) + 1, (rand() % 5) + 1);

}
void Asteroids_big::Deinit()
{
	if (asteroids_big){
		SmartDeinitDelete(asteroids_big);
	}

}
void Asteroids_medium::Deinit()
{
	if (asteroids_medium){
		SmartDeinitDelete(asteroids_medium);
	}

}
void Asteroids_small::Deinit()
{
	if (asteroids_small){
		SmartDeinitDelete(asteroids_small);
	}

}

void Asteroids_big::Update(unsigned int dt)
{
	float dtSeconds = (float)dt / 1000.f;
	//sets ship in middle of screen
	asteroids_big->PositionSet(position - (Point2F::Create(0, asteroids_big->SizeGet(1).y / 2)));
	//have to update so it animamtes
	asteroids_big->Update(dt);


	//keeps the ship on the screen
	if (position.y < 0){
		position.y = theScreen->SizeGet().y;
	}
	if (position.y > theScreen->SizeGet().y){
		position.y = 0;
	}
	if (position.x < 0){
		position.x = theScreen->SizeGet().x;
	}
	if (position.x > theScreen->SizeGet().x){ 
		position.x = 0;
	}

	position.x -= speed*velocity.x/2;
	position.y += speed*velocity.y/2;
}
void Asteroids_medium::Update(unsigned int dt)
{
	float dtSeconds = (float)dt / 1000.f;
	//sets ship in middle of screen
	asteroids_medium->PositionSet(position - (Point2F::Create(0, asteroids_medium->SizeGet(1).y / 2)));
	//have to update so it animamtes
	asteroids_medium->Update(dt);


	//keeps the ship on the screen
	if (position.y < 0){
		position.y = theScreen->SizeGet().y;
	}
	if (position.y > theScreen->SizeGet().y){
		position.y = 0;
	}
	if (position.x < 0){
		position.x = theScreen->SizeGet().x;
	}
	if (position.x > theScreen->SizeGet().x){
		position.x = 0;
	}

	position.x += speed*velocity.x/2;
	position.y -= speed*velocity.y/2;
}
void Asteroids_small::Update(unsigned int dt)
{
	float dtSeconds = (float)dt / 1000.f;
	//sets ship in middle of screen
	asteroids_small->PositionSet(position - (Point2F::Create(0, asteroids_small->SizeGet(1).y / 2)));
	//have to update so it animamtes
	asteroids_small->Update(dt);


	//keeps the ship on the screen
	if (position.y < 0){
		position.y = theScreen->SizeGet().y;
	}
	if (position.y > theScreen->SizeGet().y){
		position.y = 0;
	}
	if (position.x < 0){
		position.x = theScreen->SizeGet().x;
	}
	if (position.x > theScreen->SizeGet().x){
		position.x = 0;
	}

	position.x -= speed*velocity.x/2;
	position.y += speed*velocity.y/2;
}

void Asteroids_big::Draw()
{
	asteroids_big->Draw();

}
void Asteroids_medium::Draw()
{
	asteroids_medium->Draw();

}
void Asteroids_small::Draw()
{
	asteroids_small->Draw();

}
void Asteroids_big::deactivate()
{
	asteroids_big->VisibleSet(false);
	isactive = false;

}
void Asteroids_medium::deactivate()
{
	asteroids_medium->VisibleSet(false);
	isactive = false;

}
void Asteroids_small::deactivate()
{
	asteroids_small->VisibleSet(false);
	isactive = false;

}
void Asteroids_big::activate()
{
	asteroids_big->VisibleSet(true);
	isactive = true;

}
void Asteroids_medium::activate()
{
	asteroids_medium->VisibleSet(true);
	isactive = true;

}
void Asteroids_small::activate()
{
	asteroids_small->VisibleSet(true);
	isactive = true;

}




