#ifndef __MATHS_H__
#define __MATHS_H__

struct Vec3 {
	float x;
	float y;
	float z;

};

struct Vec2f {
	float x;
	float y;

	Vec2f() : Vec2f(0.0f, 0.0f) {}
	Vec2f(float x, float y) : x(x), y(y) {}

	Vec2f operator*(float rhs) {
		return Vec2f(x * rhs, y * rhs);
	}
};

#endif