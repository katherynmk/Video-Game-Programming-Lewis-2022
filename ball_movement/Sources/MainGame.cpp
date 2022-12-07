#include "Frog.h"
#include "MainGame.h"
#include "MainUpdate.h"
#include "Sprite.h"

using namespace Webfoot;

MainGame MainGame::instance;

//==============================================================================

/// Main GUI
#define GUI_LAYER_NAME "MainGame"

//-----------------------------------------------------------------------------

MainGame::MainGame()
{
   ball = NULL;
   ballr = NULL;
   ballm = NULL;


}

//-----------------------------------------------------------------------------

void MainGame::Init()
{
   Inherited::Init();

   // Create and initialize the ball.
   ball = frog_new Ball();
   ballr = frog_new Ballr();
   ballm = frog_new Ballm();



   ball->Init();
   ballr->Init();
   ballm->Init();

}

//-----------------------------------------------------------------------------

void MainGame::Deinit()
{
   // Deinitialize and delete the ball.
   if(ball)
   {
      ball->Deinit();
      frog_delete ball;
      ball = NULL;
   }
   if (ballr)
   {
	   ballr->Deinit();
	   frog_delete ballr;
	   ballr = NULL;
   }
   if (ballm)
   {
	   ballm->Deinit();
	   frog_delete ballm;
	   ballm = NULL;
   }


   
   Inherited::Deinit();
}

//-----------------------------------------------------------------------------

const char* MainGame::GUILayerNameGet()
{
   return GUI_LAYER_NAME;
}

//-----------------------------------------------------------------------------

void MainGame::Update()
{
   Inherited::Update();

   unsigned int dt = theClock->LoopDurationGet();

   ball->Update(dt);
   ballr->Update(dt);
   ballm->Update(dt);


   // Return to the previous menu if the escape key is pressed.
   if(!theStates->StateChangeCheck() && theKeyboard->KeyJustPressed(KEY_ESCAPE))
   {
      theMainGame->StateChangeTransitionBegin(true);
      theStates->Pop();
   }
}

//-----------------------------------------------------------------------------

void MainGame::Draw()
{
	
   ball->Draw();
   ballr->Draw();
   ballm->Draw();

}

//==============================================================================

Ball::Ball()
{
   // Initialize pointers to NULL for safety.
   image = NULL;

}
Ballr::Ballr()
{
	// Initialize pointers to NULL for safety.
	image = NULL;

}
Ballm::Ballm()
{
	// Initialize pointers to NULL for safety.
	image = NULL;

}

//------------------------------------------------------------------------------

void Ball::Init()
{
   // Load the image of the ball.
   image = theImages->Load("oship");

   // Start the ball in the middle of the screen.
   position = Point2F::Create(theScreen->SizeGet() / 2);

   //velocity.Set
}

void Ballr::Init()
{
	// Load the image of the ball.
	image = theImages->Load("Ball");

	// Start the ball in the middle of the screen.
	position = Point2F::Create(theScreen->SizeGet() / 2);

	//velocity.Set
}
void Ballm::Init()
{
	// Load the image of the ball.
	image = theImages->Load("Ball");

	// Start the ball in the middle of the screen.
	position = Point2F::Create(theScreen->SizeGet() / 2);

	//velocity.Set
}


//------------------------------------------------------------------------------

void Ball::Deinit()
{
   // Unload the image of the ball.
   if(image)
   {
      theImages->Unload(image);
      image = NULL;
   }
}
void Ballr::Deinit()
{
	// Unload the image of the ball.
	if (image)
	{
		theImages->Unload(image);
		image = NULL;
	}
}
void Ballm::Deinit()
{
	// Unload the image of the ball.
	if (image)
	{
		theImages->Unload(image);
		image = NULL;
	}
}

//------------------------------------------------------------------------------


int speed = 7; 
int X = speed;
int Y = speed;

int speed1 = 5;
int X1 = speed1;
int Y1 = speed1;

int speed2 = 2;
int X2 = speed2;
int Y2 = speed2;

void Ball::Update(unsigned int dt)
{
	Point2F positionScreen = Point2F::Create(theScreen->SizeGet()); 

}
void Ballr::Update(unsigned int dt)
{
	Point2F positionScreen = Point2F::Create(theScreen->SizeGet());

	if (positionScreen.x < position.x){
		X1 *= -1;
	}
	else if (position.x < 0){
		X1 *= -1;
	}
	if (positionScreen.y < position.y){
		Y1 *= -1;
	}
	else if (position.y < 0){
		Y1 *= -1;
	}

	position.Set(position.x + X1, position.y + Y1);
}
void Ballm::Update(unsigned int dt)
{
	Point2F positionScreen = Point2F::Create(theScreen->SizeGet()); 

	if (positionScreen.x < position.x){ 
		X2 *= -1; 
	}
	else if (position.x < 0){ 
		X2 *= -1;
	}
	if (positionScreen.y < position.y){ 
		Y2 *= -1;
	}
	else if (position.y < 0){
		Y2 *= -1;
	}

	position.Set(position.x + X2, position.y + Y2);
}

//------------------------------------------------------------------------------

void Ball::Draw()
{
   // The center of the ball is in the center of the image, so use an offset.
   image->Draw(position - (Point2F::Create(image->SizeGet()) / 2.0f));
}
void Ballr::Draw()
{
	// The center of the ball is in the center of the image, so use an offset.
	image->Draw(position - (Point2F::Create(image->SizeGet()) / 2.0f));
}
void Ballm::Draw()
{
	// The center of the ball is in the center of the image, so use an offset.
	image->Draw(position - (Point2F::Create(image->SizeGet()) / 2.0f));
}

//------------------------------------------------------------------------------
