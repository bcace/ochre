#include "geometry.h"
#include <math.h>

using namespace pine;

//
// rect
//

Rect::Rect() {}

Rect::Rect(const Point &_min, const Point &_max) : min(_min), max(_max) {}

Rect::Rect(double minx, double miny, double maxx, double maxy) : min(minx, miny), max(maxx, maxy) {}

Rect &Rect::operator=(const Rect &other) {
	min = other.min;
	max = other.max;
	return *this;
}

double Rect::W() const {
	return max.x - min.x;
}

double Rect::H() const {
	return max.y - min.y;
}

Point Rect::Center() const {
	return min + Point(W() * 0.5, H() * 0.5);
}

bool Rect::Contains(const Point &p) const {
	return p >= min && p <= max;
}

//
// point
//

Point::Point() : x(0.0), y(0.0) {}

Point::Point(double _x, double _y) : x(_x), y(_y) {}

Point &Point::operator=(const Point &other) {
	x = other.x;
	y = other.y;
	return *this;
}

Point Point::operator+(const Point &p) const {
	return Point(x + p.x, y + p.y);
}

Point Point::operator-(const Point &p) const {
	return Point(x - p.x, y - p.y);
}

Point Point::operator*(double v) const {
	return Point(x  * v, y * v);
}

Point &Point::operator+=(double v) {
	x += v;
	y += v;
	return *this;
}

Point &Point::operator-=(double v) {
	x -= v;
	y -= v;
	return *this;
}

bool Point::operator<=(const Point &p) const {
	return x <= p.x && y <= p.y;
}

bool Point::operator>=(const Point &p) const {
	return x >= p.x && y >= p.y;
}

//
// bounds
//

Bounds::Bounds() {}

Bounds::Bounds(double d) :
	x(d), y(d), z(d) {}

Bounds::Bounds(const Range &_x, const Range &_y, const Range &_z) :
	x(_x), y(_y), z(_z) {}

Bounds::Bounds(const Box &b) :
	x(b.min.x, b.max.x), y(b.min.y, b.max.y), z(b.min.z, b.max.z) {}

Bounds::Bounds(double *v) :
	x(v[0], v[3]), y(v[1], v[4]), z(v[2], v[5]) {}

double Bounds::Diagonal() {
	return sqrt(x.Size() * x.Size() + y.Size() * y.Size() + z.Size() * z.Size());
}

void Bounds::Absorb(const Bounds &b) {
	x.Absorb(b.x);
	y.Absorb(b.y);
	z.Absorb(b.z);
}
