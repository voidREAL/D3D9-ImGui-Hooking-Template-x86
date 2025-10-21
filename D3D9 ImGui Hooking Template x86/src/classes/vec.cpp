#include "../../classes/vec.h"

Vec3::Vec3() {
	x = 0;
	y = 0;
	z = 0;
}

Vec3::Vec3(float xValue, float yValue, float zValue)
{
	x = xValue;
	y = yValue;
	z = zValue;
}

Vec2::Vec2()
{
	x = 0;
	y = 0;
}

Vec2::Vec2(float xValue, float yValue)
{
	x = xValue;
	y = yValue;
}
