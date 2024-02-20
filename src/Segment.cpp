#include "../include/Segment.h"

Segment::Segment(Drawable drawable, int angle, int points) {
	m_drawable = drawable;
	m_angle = angle;
	m_points = points;
}

Segment::~Segment() {
	
}

Drawable Segment::getDrawable() {
	return m_drawable;
}