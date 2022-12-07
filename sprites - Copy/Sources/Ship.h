#ifndef __SHIP_H__
#define __SHIP_H__

#include "Frog.h"
#include "MenuState.h"
#include "FrogMath.h"


namespace Webfoot {
class Ship{
	public:
		Ship();
		Sprite* ship;

		virtual void Init();
		virtual void Deinit();
		

		virtual void Update(unsigned int dt);

		virtual void Draw();
		Point2F position;
		Point2F velocity;
		Point2F thrust_velocity;
		Point2F friction;
		float rotation;
		
		Point2F position_get();
		Point2F velocity_get();
		float rotation_get();
		char Input;
	};


}
#endif