#include "Frog.h"
#include "MainGame.h"
#include "MainUpdate.h"
#include "FrogMath.h"
#include "Bullets.h"
#include "Sprite.h"

using namespace Webfoot;


Bullets::Bullets()
{
	bullets = NULL;
}


//----------------------------

void Bullets::Init()
{
	bullets = frog_new Sprite();
	bullets->Init("Sprites/Sprites", "Bullets");
	bullets->VisibleSet(false);


	position = Point2F::Create(theScreen->SizeGet() / 2);
	//starts at 0,0
	velocity = { 0.0, 0.0 };
	rotation = 0.0;

	isactive = false;
}


void Bullets::Deinit()
{
	if (bullets){
		SmartDeinitDelete(bullets);
	}

}

void Bullets::Update(unsigned int dt)
{
	float dtSeconds = (float)dt / 1000.f;
	//sets ship in middle of screen
	bullets->PositionSet(position - (Point2F::Create(0, bullets->SizeGet(1).y / 2)));
	//have to update so it animamtes
	bullets->Update(dt);


	//keeps the ship on the screen
	if (position.y < 0){
		position.y = theScreen->SizeGet().y;
		isactive = false;
		bullets->VisibleSet(false);

	}
	if (position.y > theScreen->SizeGet().y){
		isactive = false;
		position.y = 0;
		bullets->VisibleSet(false);
	}
	if (position.x < 0){
		position.x = theScreen->SizeGet().x;
		isactive = false;
		bullets->VisibleSet(false);
	}
	if (position.x > theScreen->SizeGet().x){
		position.x = 0;
		isactive = false;
		bullets->VisibleSet(false);
		
	}

	position.x += 7*FrogMath::Cos(-90 - rotation);
	position.y += 7*FrogMath::Sin(-90 - rotation);

	
}


void Bullets::Draw()
{
	bullets->Draw();

}
void Bullets::deactivate()
{
	bullets->VisibleSet(false);
	

}
void Bullets::activate()
{
	bullets->VisibleSet(true);


}
