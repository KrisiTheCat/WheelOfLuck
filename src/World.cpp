#include <iostream>
#include "../include/World.h"
#include <math.h>
#include <ctime>

#define PI 3.14159265

// Pointers to our window, renderer, and texture
SDL_Window* window;
SDL_Renderer* renderer;

World::World() {
	m_spinning = false;
	m_isRunning = true;
	m_peaked = false;
	m_acceleraction = 0;
}

// Destroy all SDL parts to prevent memory leak
World::~World() {
	SDL_DestroyTexture(m_button.texture);
	SDL_DestroyTexture(m_buttonText.texture);
	SDL_DestroyTexture(m_circleBorder.texture);
	SDL_DestroyTexture(m_centerCircle.texture);
	SDL_DestroyTexture(m_topTriangle.texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	m_circleBorder.texture = NULL;
	m_centerCircle.texture = NULL;
	m_topTriangle.texture = NULL;
	for (Segment s : m_segments) {
		SDL_DestroyTexture(s.getDrawable().texture);
	}
	window = NULL;
	renderer = NULL;
	SDL_Quit();
	TTF_Quit();
}

//Initing SDL, TTF and all visible elements with textures and rects
void World::init() {
	srand(time(NULL));
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
	// TTF init
	TTF_Init();
	font = TTF_OpenFont("fonts\\anta.ttf", 125);


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

	//init button
	initEnabledButton();
	m_button.rect.x = 139;
	m_button.rect.y = 660;
	m_button.rect.w = 382;
	m_button.rect.h = 150;
	m_buttonText.rect.x = 169;
	m_buttonText.rect.y = 670;
	m_buttonText.rect.w = 322;
	m_buttonText.rect.h = 130;

	//adding segments
	Drawable segmDrawable;
	segmDrawable.rect.x = 330;
	segmDrawable.rect.y = 85;
	segmDrawable.rect.w = 145;
	segmDrawable.rect.h = 242;

	SDL_Color color = { 255, 255, 255 };
	for (int i = 0; i < 10; i++) {
		int pts;
		switch (i % 5) {
		case 0: segmDrawable.texture = initTexture("img\\dark_blue_segment.bmp");	pts = 100;  break;
		case 1: segmDrawable.texture = initTexture("img\\yellow_segment.bmp");		pts = 200;  break;
		case 2: segmDrawable.texture = initTexture("img\\darkest_segment.bmp");		pts = -500; break;
		case 3: segmDrawable.texture = initTexture("img\\blue_segment.bmp");		pts = 300;  break;
		case 4: segmDrawable.texture = initTexture("img\\orange_segment.bmp");		pts = 400;  break;
		}

		SDL_Surface* surface = TTF_RenderText_Solid(font,to_string(pts).c_str(), color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		m_segments.push_back(Segment(segmDrawable, texture ,i*36));

	}



}

//Update all things that change from frame to frame
void World::update() {
	SDL_Event e;
	//check for mouse click
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_MOUSEBUTTONDOWN:
		{
			int x, y;
			Uint32 buttons = SDL_GetMouseState(&x, &y);
			// if mouse is clicked in the rect of the button
			if (checkCoordsInRect(m_button.rect, x, y)) {
				initDisabledButton();
				m_spinning = true;
			}
			break;
		}
		case SDL_WINDOWEVENT:
		{
			if (e.window.event == SDL_WINDOWEVENT_CLOSE) {   // Stop program
				m_isRunning = false;
			}
			break;
		}
		}
	}
	// check if the wheel is supposed to be spinning
	if (m_spinning) {
		if (!m_peaked) {
			if (m_acceleraction >= rand() % 50 + 50) {
				m_peaked = true;
			}
			m_acceleraction += 2;
		}
		if (m_peaked && m_acceleraction > 0) m_acceleraction--;
		if (m_acceleraction == 0) {
			m_spinning = false;
			for (Segment s : m_segments) {
				if((s.m_angle+m_wheelAngle%360)>=360-18 || (s.m_angle + m_wheelAngle % 360) <= 18){
					m_buttonText.texture = s.getDrawableText();
					break;
				}
			}
		}
		m_wheelAngle += m_acceleraction;
	}
}

//check if Point(x;y) is inside of SDL_Rect
bool World::checkCoordsInRect(SDL_Rect rect, int x, int y){
	if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h) return true;
	return false;
}

bool World::isRunning(){
	return m_isRunning;
}

//draw every detail
void World::draw() {
	// Clear the window to white
	SDL_SetRenderDrawColor(renderer, 170, 170, 170, 255);
	SDL_RenderClear(renderer);

	// Draw every segment first
	for (Segment s : m_segments) {
		displaySegment(s);
	} 

	// Draw all stationary elements
	displayDrawable(m_circleBorder);
	displayDrawable(m_topTriangle);
	displayDrawable(m_centerCircle);
	displayDrawable(m_button);
	displayDrawable(m_buttonText);

	SDL_RenderPresent(renderer);
}

// The act of actually rendering the image
void World::displayDrawable(Drawable drawable) {
	SDL_RenderCopy(renderer, drawable.texture, NULL, &drawable.rect);
}
// The act of actually rendering a segment
void World::displaySegment(Segment segment) {
	SDL_Rect rect = segment.getDrawable().rect; 

	int actualAngle = m_wheelAngle + segment.m_angle;
	double sinA = sin(actualAngle * PI / 180.0);
	double cosA = cos(actualAngle * PI / 180.0);
	rect.x += -rect.w/2 + sinA * rect.h / 2;
	rect.y += +rect.h/2 - cosA * rect.h / 2;

	int actualAngle2 = m_wheelAngle + segment.m_angle - 90;
	SDL_Rect rectText = { rect.x+rect.w/2-40,rect.y + rect.h / 2 - 15,80,30 };

	//Draw the colored triangle
	SDL_RenderCopyEx(renderer,
		segment.getDrawable().texture,
		NULL,
		&rect,
		actualAngle,
		NULL,
		SDL_FLIP_NONE);
	// Display the text on top
	SDL_RenderCopyEx(renderer,
		segment.getDrawableText(),
		NULL,
		&rectText,
		actualAngle2,
		NULL,
		SDL_FLIP_NONE);
}

// Initing graphics for the ENABLED version of the button
void World::initEnabledButton(){
	m_button.texture = initTexture("img\\en_button.bmp");
	SDL_Color color = { 255, 223, 143 };
	SDL_Surface* surface = TTF_RenderText_Solid(font,
		"SPIN", color);
	m_buttonText.texture = SDL_CreateTextureFromSurface(renderer, surface);
}

// Initing graphics for the DISABLED version of the button
void World::initDisabledButton(){
	m_button.texture = initTexture("img\\dis_button.bmp");
	SDL_Color color = { 170, 170, 170 };
	SDL_Surface* surface = TTF_RenderText_Solid(font,
		"...", color);
	m_buttonText.texture = SDL_CreateTextureFromSurface(renderer, surface);
}

//Getting texture by filename
SDL_Texture* World::initTexture(const char* filename){
	SDL_Texture* texture;
	SDL_Surface* buffer = SDL_LoadBMP(filename);
	if (!buffer) {
		cout << "Error loading image "<< filename<<".bmp: " << SDL_GetError() << endl;
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
