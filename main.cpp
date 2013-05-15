#include "general.h"
#include <random>
#include <chrono>

char **testMap;

using std::vector;

enum Terrain {
	GRASS = 0,
	WATER,
	SAND
};

SDL_Surface* findSpriteByLabel(string);

int main() {
	int cols, rows = cols = 10;
	startup();
	std::cin.get();
	//playNewMusic();
	std::cin.get();

	for (int i = 0; i < imageList.size(); i++) {
		std::cout << imageList[i].label << endl;
	}

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::mt19937 generator (seed);
	std::cout << "Generator makes: " << generator() << std::endl;

	int i = 0;
	/*for (std::vector< resourceData<SDL_Surface> >::iterator it = imageList.begin(); it!= imageList.end(); ++it) {
		apply_surface(i*32,0, ((*it).resource), screen);
		std::cout << "Printing " << ((*it).label) << "...\n";
		refresh(screen);
		i++;
		SDL_Delay(1000);
	}*/
//	int loc;

	testMap = new char*[rows];
	for (i = 0; i < 10; i++) {
		testMap[i] = new char[cols];
	}

	//initialize the test map
	for (i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			switch (getRand(0, resources->imageCount())) {
				case GRASS:
					testMap[i][j] = 'G';
					break;
				case WATER:
					testMap[i][j] = 'W';
					break;
				case SAND:
					testMap[i][j] = 'S';
					break;
				default:
					testMap[i][j] = 'X';
			}
		}
	}
	std::cin.get();
	printf("Initialized the map\n");

	//draw and print the test map
	SDL_Surface* temp;
	std::string labelKey;
	labelKey.reserve(10);
	std::cin.get();
	for (i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			switch (testMap[i][j]) {
				case 'G':
					labelKey = "grass";
					break;
				case 'W':
				case 'X':
					labelKey = "water";
					break;
				case 'S':
					labelKey = "sand";
					break;
			}
			temp = resources->findImageByLabel(labelKey);

			printf("NO GOOF\n");
			apply_surface(j*32, i*32, temp, screen);
			std::cout << testMap[i][j];
			refresh(screen);

			printf("NO GOOF\n");
			//SDL_Delay(500);
			printf("NO GOOF\n");
		}
		std::cout << std::endl;
	}

	printf("Putting little dude...somewhere\n");
	int x = getRand(SCRN_WIDTH/2, SCRN_WIDTH), y = getRand(SCRN_HEIGHT/2, SCRN_HEIGHT);
	Character *duder = new Character(new Point(x,y), true, "littledude");
	MovingObject *ball = new MovingObject(new Point(SCRN_WIDTH/2, 0), "ball", 5, 9);
	while (true) {
		fps.start();
		clearScreen();
		duder->update();
		ball->update();
		duder->draw();
		ball->draw();
		while (SDL_PollEvent(&event)) {
			duder->handle_input();
			if (event.type == SDL_QUIT) {
				exit(0);
			}
		}
		duder->displayInfo();
		refresh(screen);
		frame++;
		fpsDelay();
	}

	makeText("ermahgurd");

	refresh(screen);
	std::cin.get();
	/*for (i = 0; i < 10; i++) {
		loc = generator() % imageList.size();
		apply_surface(i*32, 0, (imageList[loc].resource), screen);
		std::cout << "Printing " << (imageList[loc].label) << "...\n";
		refresh(screen);
		SDL_Delay(1000);
	}*/

	return 0;
}
