#include "../include/defines.h"
#include "../include/World.h"


using namespace std;

int main(int argc, char** args) {
	World world; // Variable that contains all actions
	world.init(); // Initing all values to entry position

	while (world.isRunning()) { // Checking if the program is running
		world.update();         // Update the program (mainly the wheel + mouse click listener)
		world.draw();           // Draw the current status
		SDL_Delay(30);          // Wait a bit (it's too fast otherwise)
	}

	return 0;
}
