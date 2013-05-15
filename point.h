#ifndef _POINT_H_
#define _POINT_H_

#include "general.h"

extern SDL_Event event;

void get_out();

class Point {
	private:
		int x, y;
		bool isInBox(int x0, int y0, int w, int h) {
			if ((x > x0 && x < (x0 + w)) && (y > y0) && (y < (y0 + h))) {
				return true;
			}
			else return false;
		}
    public:
		Point() {
			x = y = 0;
		}
		Point(int _x, int _y) {
			x = _x;
			y = _y;
		}

		void setXPos(int newX) {
			this->x = newX;
		}

		void setYPos(int newY) {
			this->y = newY;
		}

		int getXPos() {
			return x;
		}
		int getYPos() {
			return y;
		}

		Point operator=(const Point &right) {
			if (this == &right) {
				return *this;
			}
			this->x = right.x;
			this->y = right.y;
			return *this;
		}

		bool operator==(const Point* &right) {
			if (this->x == right->x && this->y == right->y) {
				return true;
			}
			else return false;
		}

		void pointTuple() {
			std::cout << "(" << x << "," << y << ")";
		}
};

Point *click = new Point();

Point* getClick(bool canQuit = true) {
	Point* result = new Point();
	SDL_WaitEvent(&event);
	if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN) {
		int _x, _y;
		SDL_GetMouseState(&_x, &_y);
		result->setXPos(_x);
		result->setYPos(_y);
	}
	else if (canQuit && event.type == SDL_QUIT) {
		get_out();
		exit(0);
	}
	else if (event.type != SDL_MOUSEMOTION) {
		result->setXPos(click->getXPos());
		result->setYPos(click->getYPos());
	}
	return result;
}

#endif
