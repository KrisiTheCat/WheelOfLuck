#pragma once
#include "defines.h"
class Segment {
public:
	Segment(Drawable, SDL_Texture*, int);
	~Segment();
	Drawable getDrawable();
	SDL_Texture* getDrawableText();
	int m_angle;
private:
	Drawable m_drawable;
	SDL_Texture* m_drawableText;
};