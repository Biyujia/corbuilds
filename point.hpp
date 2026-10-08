#ifndef point
#define point

#include "cglobal.hpp"

struct Point {
	double x, y;

	Point () {
		x = y = 0.;
	}
	
	Point (double x_, double y_) {
		x = x_, y = y_;
	}

	bool operator == (const Point &p) const {
		return GLOBAL::cmp(x, p.x) && GLOBAL::cmp(y, p.y); 
	}

	bool operator != (const Point &p) const {
		return !(*this == p);
	}

	Point operator + (const Point &p) const {
		return Point(x + p.x, y + p.y);
	}

	Point operator - (const Point &p) const {
		return Point(x - p.x, y - p.y);
	}

	Point operator * (double k) const {
		return Point(x * k, y * k);
	}

	Point operator / (double k) const {
		return Point(x / k, y / k);
	}
};

inline void print(const Point& x) {
	printf("[Point] (%.6lf, %.6lf)\n", x.x, x.y);
}

#endif