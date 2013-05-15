#ifndef _COOLSTUFF_H_
#define _COOLSTUFF_H_

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <list>
#include <random>
#include <chrono>
#include <vector>

#include <dirent.h>

#include "point.h"
#include "files.h"
#include "resourcemanager.h"
#include "music.h"
#include "graphics.h"
#include "object.h"
#include "timing.h"

using std::endl;

/*
  ----------------CONSTANTS AND GLOBAL VARIABLES--------------------
  - Mostly things for image manipulation involving SDL_Surface variables, and various other SDL objects
  - Also frame rate and resolution shenanigans
*/

//GLOBAL VARIABLES

SDL_Event event;

SDL_Surface *screen = NULL;

SDL_Rect *text = NULL;

TTF_Font* pieces_font = NULL;

SDL_Color textC = {0,0,0};
SDL_Color wh = {255, 255, 255};

bool quit = false;

//random seed
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

std::mt19937 generator (seed);

//CONSTANTS

const int SCRN_WIDTH = 640;
const int SCRN_HEIGHT = 480;
const int SCRN_BPP = 32;

const int FPS = 30;

//FN PROTOTYPES
void get_out();

//MANAGERS

ResourceManager *resources;

//DATA STRUCTURES

Point *invalid = new Point(-1, -1);

template <typename T>
struct ResourceData{
	std::string label;
	T* resource;

	void init(std::string name, T* rsrc) {
		label = name;
		resource = rsrc;
	}
};

std::vector<ResourceData<SDL_Surface> > imageList;

typedef struct ResourceData<SDL_Surface> rsrcSfcDat;
typedef std::vector<ResourceData<SDL_Surface> > rsrcSfcVec;

/*
  ---------------FUNCTION PROTOTYPES---------------------------------
  - Generic function prototypes for all-purpose graphical functions -
  -
*/

//INITIALIZATION AND SUCH

bool init()
{
  	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
      	return false;
    }
  	screen = SDL_SetVideoMode(SCRN_WIDTH, SCRN_HEIGHT, SCRN_BPP,
			    SDL_SWSURFACE);
  	SDL_WM_SetCaption("Level Editor", "Level Editor");
  	if (TTF_Init() == -1) {
    	return false;
  	}
  	if (screen == NULL)
    {
      	return false;
    }
    if (!imageSystemInit()) {
    	return false;
    }
    //musicSystemInit();
    resources = new ResourceManager();
    resources->autoLoadImages();
    resources->autoLoadMusic();
    resources->autoLoadSprites();
    resources->autoLoadFonts();

  	return true;
} //Simply initialization stuff for the SDL objects

//SDL_Surface* imageLoad(std::string filename, bool trans = true);


//who needs this shit anymore?  deprecated!
/*
bool load_files()
{
  	grass = imageLoad("grass.png");
  	water = imageLoad("water.png");
  	sand = imageLoad("sand.png");
  	dude = imageLoad("littledude.png");
  	rsrcSfcDat rsrcS;
  	rsrcSfcDat rsrcG;
  	rsrcSfcDat rsrcW;
  	rsrcSfcDat rsrcDude;

	rsrcS.init("sand", sand);
	rsrcG.init("grass", grass);
	rsrcDude.init("littledude", dude);
	rsrcW.init("water", water);

  	imageList.push_back(rsrcG);
  	imageList.push_back(rsrcS);
  	imageList.push_back(rsrcW);
  	imageList.push_back(rsrcDude);
  	if (grass == NULL || water == NULL || sand == NULL || dude == NULL)
    {
      	return false;
    }
  	return true;
} //Loads files like fonts and whatever else it needs at startup
*/

//a more modular version of the load_files() function
//jay kay


//also hella-depped
bool putImageInList(std::string label, std::string filename, bool trans = false) {
	rsrcSfcDat temp;
	SDL_Surface* tempImg = imageLoad(filename, trans);
	if (tempImg == NULL) {
		return false;
	}
	temp.resource = tempImg;
	temp.label = label;
	imageList.push_back(temp);
	return true;
}

//but here's a "load font" one for more modularization
//not yet hella-depped
TTF_Font* openFont(std::string fontfile, int size) {
	TTF_Font *ourFont = TTF_OpenFont(fontfile.c_str(), size);
	return ourFont;
}

void startup()
{
	timeStart = clock();
    if (!init() || !musicSystemInit()/* || !load_files()*/) {
        exit(0);
    }
    timeEnd = clock();
    timeDiff = timeEnd - timeStart;
    printf("Startup took %i clocks (%f seconds)\n", timeDiff, ((float)timeDiff)/CLOCKS_PER_SEC);

  	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF,
					0xFF, 0xFF));
  	//apply_surface(0,0,bg, screen);
} //The very beginning stuff.  Right now it sends that bitch a smiley face

//GENERIC STUFF

template <class T>
inline std::string to_string (const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}


//random number wuuuuut
int getRand(int lower, int upper) {
	return (generator() % (upper-lower)) + lower;
}

//GRAPHICAL STUFF



//SHUTDOWN STUFF

void clean_up()
{
  SDL_FreeSurface(screen);
  //SDL_FreeSurface(txt);
} //Frees all the SDL objects, nuff said

void get_out()
{
  clean_up();
  TTF_Quit();
  SDL_Quit();
} //shuts down SDL and exits the program

/*
  ----------------CLASSES--------------------------------------------
  -
*/

/*
  ----------------TESTING JUNK---------------
  -
*/


#endif
