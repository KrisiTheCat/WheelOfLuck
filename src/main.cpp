#include "../include/defines.h"
#include "../include/World.h"


using namespace std;



int main(int argc, char** args) {
	World world;
	world.init();

	while (world.isRunning()) {
		world.update();
		world.draw();
		SDL_Delay(30);
	}

	return 0;
}
