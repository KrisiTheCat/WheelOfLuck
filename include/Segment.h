#pragma once
#include "defines.h"
class Segment {
public:
	Segment(Drawable drawable, int angle, int points);
	~Segment();
	Drawable getDrawable();
	int m_angle;
private:
	Drawable m_drawable;
	int m_points;
};