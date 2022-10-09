#ifndef __MATHS_H__
#define __MATHS_H__

struct Vec3 {
	float x;
	float y;
	float z;

};

struct Vec2 {
	float x;
	float y;

	Vec2() : Vec2(0.0f, 0.0f) {}
	Vec2(float x, float y) : x(x), y(y) {}
};

#endif