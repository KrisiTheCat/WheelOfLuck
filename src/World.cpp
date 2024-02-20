#include <iostream>
#include <SDL.h>
#include "../include/World.h"
#include <math.h>
#include <ctime>

#define PI 3.14159265

// Pointers to our window, renderer, and texture
SDL_Window* window;
SDL_Renderer* renderer;

World::World() {
	m_isRunning = true;
}

World::~World() {
	SDL_DestroyTexture(m_circleBorder.texture);
	SDL_DestroyTexture(m_centerCircle.texture);
	SDL_DestroyTexture(m_topTriangle.texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	m_circleBorder.texture = NULL;
	m_centerCircle.texture = NULL;
	m_topTriangle.texture = NULL;
	for (Segment s : m_segments) {
		SDL_DestroyTexture(s.getDrawable().texture);
	}
	window = NULL;
	renderer = NULL;
	SDL_Quit();
}

void World::init() {
	srand(time(0));
	//SDL INIT
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		return;
	}

	window = SDL_CreateWindow("Lucky wheel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 660, 900, SDL_WINDOW_SHOWN);
	if (!window) {
		cout << "Error creating window: " << SDL_GetError() << endl;
		system("pause");
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		cout << "Error creating renderer: " << SDL_GetError() << endl;
		return;
	}

	//init circle border
	m_circleBorder.texture = initTexture("img\\circle.bmp");
	m_circleBorder.rect.x = 80;
	m_circleBorder.rect.y = 80;
	m_circleBorder.rect.w = 500;
	m_circleBorder.rect.h = 500;

	//init triangle
	m_topTriangle.texture = initTexture("img\\triangle.bmp");
	m_topTriangle.rect.x = 255;
	m_topTriangle.rect.y = 60;
	m_topTriangle.rect.w = 150;
	m_topTriangle.rect.h = 110;

	//init center circle
	m_centerCircle.texture = initTexture("img\\center.bmp");
	m_centerCircle.rect.x = 273;
	m_centerCircle.rect.y = 273;
	m_centerCircle.rect.w = 114;
	m_centerCircle.rect.h = 114;

	//adding segments
	Drawable segmDrawable;
	segmDrawable.rect.x = 330;
	segmDrawable.rect.y = 85;
	segmDrawable.rect.w = 145;
	segmDrawable.rect.h = 242;

	for (int i = 0; i < 10; i++) {
		switch (i % 5) {
		case 0: segmDrawable.texture = initTexture("img\\dark_blue_segment.bmp"); break;
		case 1: segmDrawable.texture = initTexture("img\\yellow_segment.bmp"); break;
		case 2: segmDrawable.texture = initTexture("img\\darkest_segment.bmp"); break;
		case 3: segmDrawable.texture = initTexture("img\\blue_segment.bmp"); break;
		case 4: segmDrawable.texture = initTexture("img\\orange_segment.bmp"); break;
		}
		m_segments.push_back(Segment(segmDrawable,i*36,100));

	}
	m_peaked = false;
	m_acceleraction = 0;

}

//update all things that change from frame to frame
void World::update() {
	if (!m_peaked) {
		if (m_acceleraction >= rand()%50+50) {
			m_peaked = true;
		}
		m_acceleraction+=2;
	}
	if(m_peaked && m_acceleraction>0) m_acceleraction--;
	m_wheelAngle += m_acceleraction;
}

bool World::isRunning()
{
	return m_isRunning;
}

//draw every detail
void World::draw() {
	// Clear the window to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	for (Segment s : m_segments) {
		displaySegment(s);
	}

	displayDrawable(m_circleBorder);
	displayDrawable(m_topTriangle);
	displayDrawable(m_centerCircle);

	SDL_RenderPresent(renderer);
}

// the act of actually rendering the image
void World::displayDrawable(Drawable drawable) {
	SDL_RenderCopy(renderer, drawable.texture, NULL, &drawable.rect);
}
// the act of actually rendering a segment
void World::displaySegment(Segment segment) {
	SDL_Rect rect = segment.getDrawable().rect; 
	int actualAngle = m_wheelAngle + segment.m_angle;
	double sinA = sin(actualAngle * PI / 180.0);
	double cosA = cos(actualAngle * PI / 180.0);
	rect.x += -rect.w/2+cosA*(rect.w/2) + sinA * rect.h / 2;
	rect.y += +rect.h/2+sinA*(rect.w/2) - cosA * rect.h / 2;
	SDL_RenderCopyEx(renderer,
		segment.getDrawable().texture,
		NULL,
		&rect,
		actualAngle,
		NULL,
		SDL_FLIP_NONE);
}

//getting texture from filename
SDL_Texture* World::initTexture(const char* filename){
	SDL_Texture* texture;
	SDL_Surface* buffer = SDL_LoadBMP(filename);
	if (!buffer) {
		cout << "Error loading image test.bmp: " << SDL_GetError() << endl;
		return nullptr;
	}

	texture = SDL_CreateTextureFromSurface(renderer, buffer);
	SDL_FreeSurface(buffer);
	buffer = NULL;
	if (!texture) {
		cout << "Error creating texture: " << SDL_GetError() << endl;
		return nullptr;
	}
	return texture;
}
