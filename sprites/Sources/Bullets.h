#ifndef __Bullets_H__
#define __Bullets_H__

#include "Frog.h"
#include "MenuState.h"
#include "FrogMath.h"


namespace Webfoot {
class Bullets{
	public:
		Bullets();
		Sprite* bullets;

		virtual void Init();
		virtual void Deinit();
		

		virtual void Update(unsigned int dt);

		virtual void Draw();
		Point2F position;
		Point2F velocity;
		float rotation;

		void fire(Point2F position, float roation, Point2F velocity);
		
		char Input;
		bool isactive;

		void deactivate();
		void activate();
	

	};


}
#endif