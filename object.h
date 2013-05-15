#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "general.h"
#include "point.h"

extern clock_t timeStart, timeEnd, timeDiff;

class Object {
	protected:
		Point* location; //coordinates in space
		bool canMove;
		string labelName;
	public:
		Object(Point* initLocation, bool movable, string label) {
			location = initLocation;
			canMove = movable;
			labelName = label;
		}

		virtual void handle_input() =0;
		virtual void update() =0;
		virtual string getLabel() =0;
		virtual void draw() =0;
		virtual void displayInfo() =0;
};

class MovingObject : public Object {
	protected:
		int xVelocity, yVelocity;
		int width, height;
	public:
		MovingObject(Point* initLoc, string label, int xVel = 0, int yVel = 0)
			: Object(initLoc, true, label) {
			xVelocity = xVel;
			yVelocity = yVel;
			labelName = label;
			width = height = -1;
		}

		void handle_input() {

		}

		void update() {
			int inityPos = location->getYPos(), deltaY;
			yVelocity += 5;
			location->setXPos(location->getXPos()+xVelocity);
			location->setYPos(location->getYPos()+yVelocity);
			if (location->getYPos() < 0)  { yVelocity *= -1; location->setYPos(0); }
			else if (location->getYPos() > SCRN_HEIGHT-height) { yVelocity *= -1; location->setYPos(SCRN_HEIGHT-height);}
			if (location->getXPos() < 0)  { xVelocity *= -1; location->setXPos(0); }
			else if (location->getXPos() > SCRN_WIDTH-width) { xVelocity *= -1; location->setXPos(SCRN_WIDTH-width); }
			deltaY = location->getYPos() - inityPos;
			if (deltaY == 0) {
				xVelocity -= 1;
				if (xVelocity < 0) {
					xVelocity = 0;
				}
			}
		}
		string getLabel() {
			return labelName;
		}

		void draw() {
			//timeStart = clock();
			SDL_Surface* image = resources->findSpriteByLabel(labelName);
			apply_surface(location->getXPos(), location->getYPos(), image, screen);
			if (width == -1 || height == -1) {
				width = image->w;
				height = image->h;
			}
			/* Can use the timing code later
			timeEnd = clock();
			timeDiff = timeEnd - timeStart;
			printf("Drawing took %i clocks (%f seconds)\n", timeDiff, ((float)timeDiff)/CLOCKS_PER_SEC);
			*/
		}

		void displayInfo() {
			location->pointTuple();
			std::cout << endl;
			printf("xvel = %i\tyvel = %i\n", xVelocity, yVelocity);
			printf("width = %i\txheight = %i\n", width, height);
		}
};

class Character : public MovingObject {
	private:
		bool isPlayer;
	public:
		Character(Point* initLocation, bool pcOrNot, string label, int xVel=0, int yVel=0)
		  : MovingObject(initLocation, label, xVel, yVel) {
			isPlayer = pcOrNot;
		}
		void handle_input() {
			if (isPlayer) {
				if (event.type == SDL_KEYDOWN) {
					switch(event.key.keysym.sym) {
						case SDLK_UP:
							yVelocity -= 5;
							break;
						case SDLK_DOWN:
							yVelocity += 5;
							break;
						case SDLK_RIGHT:
							xVelocity += 5;
							break;
						case SDLK_LEFT:
							xVelocity -= 5;
							break;
						default:
							break;
					}
				}
			}
		}

		void update() {
			location->setXPos(location->getXPos()+xVelocity);
			location->setYPos(location->getYPos()+yVelocity);
			if (location->getYPos() < 0)  { yVelocity = 0; location->setYPos(0); }
			else if (location->getYPos() > SCRN_HEIGHT-height) { yVelocity = 0; location->setYPos(SCRN_HEIGHT-height); }
			if (location->getXPos() < 0)  { xVelocity = 0; location->setXPos(0); }
			else if (location->getXPos() > SCRN_WIDTH-width) { xVelocity = 0; location->setXPos(SCRN_WIDTH-width);}
		}
};


#endif
