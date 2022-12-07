#ifndef __ASTEROIDS_H__
#define __ASTEROIDS_H__

#include "Frog.h"
#include "MenuState.h"
#include "FrogMath.h"


namespace Webfoot {
class Asteroids_big{
	public:
		Asteroids_big();
		Sprite* asteroids_big;

		virtual void Init();
		virtual void Deinit();
		virtual void Update(unsigned int dt);
		virtual void Draw();
		Point2F position;
		Point2F velocity;
		Point2F scale;
		int speed;
		bool isactive;
		void deactivate();
		void activate();

		Point2F getsize();
	};
class Asteroids_medium{
public:
	Asteroids_medium();
	Sprite* asteroids_medium;

	virtual void Init();
	virtual void Deinit();
	virtual void Update(unsigned int dt);
	virtual void Draw();
	Point2F scale;
	Point2F position;
	Point2F velocity;
	int speed;
	bool isactive;
	void deactivate();
	void activate();

	Point2F getsize();
};
class Asteroids_small{
public:
	Asteroids_small();
	Sprite* asteroids_small;

	virtual void Init();
	virtual void Deinit();
	virtual void Update(unsigned int dt);
	virtual void Draw();
	Point2F scale;
	Point2F position;
	Point2F velocity;
	int speed;
	bool isactive;
	void deactivate();
	void activate();

	Point2F getsize();
};

}
#endif