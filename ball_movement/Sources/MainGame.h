#ifndef __MAINGAME_H__
#define __MAINGAME_H__

#include "Frog.h"
#include "MenuState.h"

namespace Webfoot {

class Ball;
class Ballr;
class Ballm;


//==============================================================================

class MainGame : public MenuState
{
public:
   typedef MenuState Inherited;

   MainGame();
   virtual ~MainGame() {};
   
   virtual void Init();
   virtual void Deinit();

   /// Call this on every frame to update the positions.
   virtual void Update();
   /// Call this on every frame to draw the images.
   virtual void Draw();

   static MainGame instance;

protected:
   /// Returns the name of the GUI layer
   virtual const char* GUILayerNameGet();

   /// The ball that bounces around the screen.
   Ballr* ballr;
   Ballm* ballm;
   Ball* ball;


};

MainGame* const theMainGame = &MainGame::instance;

//==============================================================================

/// A bouncing ball
class Ball
{
public:
   Ball();
   
   /// Initialize the ball
   void Init();
   /// Clean up the ball
   void Deinit();

   /// Make any changes for the given frame.  'dt' is the amount of time that
   /// has passed since the last frame, in milliseconds.
   void Update(unsigned int dt);
   /// Draw the ball.
   void Draw();

protected:
   /// Appearance of the ball.
   Image* image;
   /// Current position of the ball.
   Point2F position;
};

class Ballr
{
public:
	Ballr();

	/// Initialize the ball
	void Init();
	/// Clean up the ball
	void Deinit();

	/// Make any changes for the given frame.  'dt' is the amount of time that
	/// has passed since the last frame, in milliseconds.
	void Update(unsigned int dt);
	/// Draw the ball.
	void Draw();

protected:
	/// Appearance of the ball.
	Image* image;
	/// Current position of the ball.
	Point2F position;
};
class Ballm
{
public:
	Ballm();

	/// Initialize the ball
	void Init();
	/// Clean up the ball
	void Deinit();

	/// Make any changes for the given frame.  'dt' is the amount of time that
	/// has passed since the last frame, in milliseconds.
	void Update(unsigned int dt);
	/// Draw the ball.
	void Draw();

protected:
	/// Appearance of the ball.
	Image* image;
	/// Current position of the ball.
	Point2F position;
};



//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __MAINGAME_H__
