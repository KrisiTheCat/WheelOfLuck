
#include "defines.h"
#include "Segment.h"
class World {
public:
	World();
	~World();
	void init();
	void draw();
	void update();
	bool isRunning();

private:
	bool m_isRunning;
	Drawable m_circleBorder;
	Drawable m_topTriangle;
	Drawable m_centerCircle;
	Drawable m_button;
	Drawable m_buttonText;

	vector<Segment> m_segments;
	int m_wheelAngle = 0;
	int m_acceleraction = 0;
	bool m_peaked;

	void displayDrawable(Drawable drawable);
	void displaySegment(Segment segment);
	SDL_Texture* initTexture(const char* filename);
}; 
