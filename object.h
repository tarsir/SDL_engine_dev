#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "general.h"
#include "point.h"

class Object {
	protected:
		Point* location; //coordinates in space
		bool canMove;
	public:
		Object(Point* initLocation, bool movable) {
			location = *initLocation;
			canMove = movable;
		}

		virtual void handle_input() =0;
};

class Character : public Object {
	private:
		int xVelocity, yVelocity;
		bool isPlayer;
	public:
		Character(Point* initLocation, bool pcOrNot, int xVel, int yVel)
		  : Object(initLocation, true) {
			isPlayer = pcOrNot;
			xVelocity = xVel;
			yVelocity = yVel;
		}
};


#endif
