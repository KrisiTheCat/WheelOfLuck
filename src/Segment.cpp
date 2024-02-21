#include "../include/Segment.h"

Segment::Segment(Drawable drawable, SDL_Texture* drawableText, int angle) {
	m_drawable = drawable;
	m_drawableText = drawableText;
	m_angle = angle;
}

Segment::~Segment() {
	
}

Drawable Segment::getDrawable() {
	return m_drawable;
}

SDL_Texture* Segment::getDrawableText() {
	return m_drawableText;
}