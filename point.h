#ifndef _POINT_H_
#define _POINT_H_

#include "general.h"

class Point {
	private:
		bool isInBox(int x0, int y0, int w, int h) {
			if ((x > x0 && x < (x0 + w)) && (y > y0) && (y < (y0 + h))) {
				return true;
			}
			else return false;
		}

		void pointTuple() {
			std::cout << "(" << x << "," << y << ")";
		}
    public:
		Point() {
			x = y = 0;
		}
		Point(int _x, int _y) {
			x = _x;
			y = _y;
		}
		int x, y;
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
};

#endif
