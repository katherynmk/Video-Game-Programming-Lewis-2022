#include "Frog.h"
#include "MainGame.h"
#include "MainUpdate.h"
#include "FrogMath.h"
#include "Ship.h"
#include "Bullets.h"
#include "Asteroids.h"
#include "math.h"
#include <cmath>
#include <math.h>


using namespace Webfoot;

MainGame MainGame::instance;


//==============================================================================

/// Main GUI
#define GUI_LAYER_NAME "MainGame"

//-----------------------------------------------------------------------------


Ship ship = Ship();
Bullets bullets[3];
Asteroids_big asteroids_big[3];
Asteroids_medium asteroids_medium[6];
Asteroids_small asteroids_small[12];

int health = 100;
int score = 0;
int level = 0;
int lives = 4;
int bigdead = 0;
int meddead = 0;
int smalldead = 0;
bool gameon = true;
bool win = false;

MainGame::MainGame()
{
	
}

//-----------------------------------------------------------------------------

void MainGame::Init()
{
   Inherited::Init();
   ship.Init();
   for (int i = 0; i < 3; i++){
	   bullets[i].Init();
   }
   for (int i = 0; i < 3; i++){
	   asteroids_big[i].Init();
   }
   for (int i = 0; i < 6; i++){
	   asteroids_medium[i].Init();
   }
   for (int i = 0; i < 12; i++){
	   asteroids_small[i].Init();
   }

}

//-----------------------------------------------------------------------------

void MainGame::Deinit()
{

	ship.Deinit();
	for (int i = 0; i < 3; i++){
		bullets[i].Deinit();
	}
	for (int i = 0; i < 3; i++){
		asteroids_big[i].Deinit();
	}
	for (int i = 0; i < 6; i++){
		asteroids_medium[i].Deinit();
	}
	for (int i = 0; i < 12; i++){
		asteroids_small[i].Deinit();
	}
   Inherited::Deinit();
}

//-----------------------------------------------------------------------------

const char* MainGame::GUILayerNameGet()
{
   return GUI_LAYER_NAME;
}

//-----------------------------------------------------------------------------

void Bullets::fire(Point2F ship_pos, float ship_rot, Point2F ship_vel)
{
	rotation = ship.rotation;
	velocity = ship.velocity;
	bullets->VisibleSet(true);
	isactive = true;

}
//math thing
float MainGame::distance(int x1, int y1, int x2, int y2)
{
	int distancex = (x2 - x1) * (x2 - x1);
	int distancey = (y2 - y1) * (y2 - y1);

	double distance = sqrt(distancex + distancey);
	return distance;
}

void MainGame::Update()
{
	Inherited::Update();

	unsigned int dt = theClock->LoopDurationGet();

	if (level < 5 && lives > 0){
		if (theKeyboard->KeyJustPressed(KEY_SPACE))
		{
			for (int i = 0; i < 3; i++){
				if (bullets[i].isactive == false){
					bullets[i].fire(ship.position, ship.rotation, ship.velocity);
					bullets[i].position = ship.position;
					break;
				}

			}
		}
		ship.Update(dt);
		//collision good enough
		for (int a = 0; a < 3; a++){
			int smallc = 0;
			int medc = 0;
			//bullet things
			Point2F pos = bullets[a].position;
			bool act = bullets[a].isactive;

			//death to big asteroids
			for (int b = 0; b < 3; b++){
				int distance1 = distance(asteroids_big[b].position.x, asteroids_big[b].position.y, pos.x, pos.y);
				int rad = 120;//size of bullets and size of asteroids

				//if distance is less than radius 1 plus radius 2
				if (distance1 < rad && asteroids_big[b].isactive == true && act == true){
					asteroids_big[b].deactivate();
					score += 100;
					medc += 2;
				}
			}
			//death to medium asteriods
			for (int c = 0; c < 6; c++){
				for (int d = 0; d < medc; d++){
					if (asteroids_medium[c].isactive == false){
						asteroids_medium[c].activate();
					}
				}
				int distance2 = distance(asteroids_medium[c].position.x, asteroids_medium[c].position.y, pos.x, pos.y);
				int rad_2 = 40;
				if (distance2 < rad_2 && asteroids_medium[c].isactive == true && act == true){
					asteroids_medium[c].deactivate();
					score += 50;
					smallc += 2;
				}
			}
			//death to small asteroids
			for (int e = 0; e < 12; e++){
				for (int f = 0; f < smallc; f++){
					if (asteroids_small[e].isactive == false){
						asteroids_small[e].activate();

					}
				}
				int distance3 = distance(asteroids_small[e].position.x, asteroids_small[e].position.y, pos.x, pos.y);
				int rad_3 = 20;
				if (distance3 < rad_3 && asteroids_small[e].isactive == true && act == true){
					asteroids_small[e].deactivate();
					score += 10;
				}
			}

		}
		//end collision

		//ship get hit
		for (int f = 0; f < 3; f++){
			int distance4 = distance(asteroids_big[f].position.x, asteroids_big[f].position.y, ship.position.x, ship.position.y);
			int rad_4 = 120;
			if (distance4 < rad_4 && asteroids_big[f].isactive == true){
				health = health - 1;
				//DebugPrintf("big hit");

			}
		}
		for (int g = 0; g < 6; g++){
			int distance5 = distance(asteroids_medium[g].position.x, asteroids_medium[g].position.y, ship.position.x, ship.position.y);
			int rad_5 = 80;
			if (distance5 < rad_5 && asteroids_medium[g].isactive == true){
				health = health - 1;
				//DebugPrintf("med hit");

			}
		}
		for (int h = 0; h < 12; h++){
			int distance6 = distance(asteroids_small[h].position.x, asteroids_small[h].position.y, ship.position.x, ship.position.y);
			int rad_6 = 35;
			if (distance6 < rad_6 && asteroids_small[h].isactive == true){
				health = health - 1;
				//DebugPrintf("small hit");


			}
		}

		//end of ship kill

		for (int i = 0; i < 3; i++){
			bullets[i].Update(dt);

		}

		for (int i = 0; i < 3; i++){
			asteroids_big[i].Update(dt);
		}

	}
	for (int i = 0; i < 6; i++){
		asteroids_medium[i].Update(dt);
	}
	for (int i = 0; i < 12; i++){
		asteroids_small[i].Update(dt);

	}

	if (health <= 0){
		lives -= 1;
		health = 100;
	}



	//if u run out of lives (working)
	else if (lives <= 0){
		DebugPrintf("dead");
		gameon = false;

		for (int i = 0; i < 3; i++){
			bullets[i].deactivate();
		}

		for (int i = 0; i < 3; i++){
			asteroids_big[i].deactivate();
		}


		for (int i = 0; i < 6; i++){
			asteroids_medium[i].deactivate();
		}
		for (int i = 0; i < 12; i++){
			asteroids_small[i].deactivate();

		}

		ship.deactivate();
	
}
	if (score > 780){
		win = true;
	}
   

   // Return to the previous menu if the escape key is pressed.
   if (!theStates->StateChangeCheck() && theKeyboard->KeyJustPressed(KEY_ESCAPE))
   {
	   theMainGame->StateChangeTransitionBegin(true);
	   theStates->Pop();
   }
}

//-----------------------------------------------------------------------------

void MainGame::Draw()
{
	Image* bg;
	bg = theImages->Load("background");
	bg->Draw(Point2F::Create(0, 0));

	DrawText;

	ship.Draw();
	
	for (int i = 0; i < 3; i++){
		bullets[i].Draw();
	}
	for (int i = 0; i < 3; i++){
		asteroids_big[i].Draw();
	}
	for (int i = 0; i < 6; i++){
		asteroids_medium[i].Draw();
	}
	for (int i = 0; i < 12; i++){
		asteroids_small[i].Draw();
	}

	char tempString[20];
	UTF8Snprintf(tempString, 20, "Score: %d", score);

	LabelWidget* scoreLabel = dynamic_cast<LabelWidget*>(theGUI->WidgetGetByPath(
		GUI_LAYER_NAME ".Score"));


	char tempString2[20];
	UTF8Snprintf(tempString2, 20, "Lives: %d", lives);

	LabelWidget* livesLabel = dynamic_cast<LabelWidget*>(theGUI->WidgetGetByPath(
		GUI_LAYER_NAME ".Lives"));

	

	if (lives > 0){
		scoreLabel->TextSet(tempString);
		livesLabel->TextSet(tempString2);

	}
	else if (gameon == false){
		livesLabel->TextSet("");
		scoreLabel->TextSet("");
		char tempStringBig[60];

		UTF8Snprintf(tempStringBig, 60, "You Lost!\n\nFinal Score: %d", score);
		LabelWidget* endLabel = dynamic_cast<LabelWidget*>(theGUI->WidgetGetByPath(
			GUI_LAYER_NAME ".End"));

		endLabel->TextSet(tempStringBig);
	}
	else if (win == true){
		livesLabel->TextSet("");
		scoreLabel->TextSet("");
		char tempStringBig[60];

		if (win == true) {
			UTF8Snprintf(tempStringBig, 60, "You Won!\n\nFinal Score: %d", score);
		}
		LabelWidget* endLabel = dynamic_cast<LabelWidget*>(theGUI->WidgetGetByPath(
			GUI_LAYER_NAME ".End"));

		endLabel->TextSet(tempStringBig);
	}
}


