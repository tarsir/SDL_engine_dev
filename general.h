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

using std::endl;

/*
  ----------------CONSTANTS AND GLOBAL VARIABLES--------------------
  - Mostly things for image manipulation involving SDL_Surface variables, and various other SDL objects
  - Also frame rate and resolution shenanigans
*/

//GLOBAL VARIABLES

SDL_Surface *screen = NULL;
SDL_Surface *grass = NULL;
SDL_Surface *water = NULL;
SDL_Surface *txt = NULL;
SDL_Surface *sand = NULL;
SDL_Surface *dude = NULL;

SDL_Event event;

SDL_Rect *text = NULL;

TTF_Font* font = NULL;
TTF_Font* pieces_font = NULL;

Point *click = new Point();

SDL_Color textC = {0,0,0};
SDL_Color wh = {255, 255, 255};

bool quit = false;

//random seed
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

std::mt19937 generator (seed);

//CONSTANTS

const int SCRN_WIDTH = 800;
const int SCRN_HEIGHT = 800;
const int SCRN_BPP = 32;

const int FPS = 30;

//FN PROTOTYPES
void get_out();

//MANAGERS

ResourceManager *resources;

//DATA STRUCTURES

Point* getClick(bool canQuit = true) {
	Point* result = new Point();
	SDL_WaitEvent(&event);
	if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN) {
		SDL_GetMouseState(&result->x, &result->y);
	}
	else if (canQuit && event.type == SDL_QUIT) {
		get_out();
		exit(0);
	}
	else if (event.type != SDL_MOUSEMOTION) {
		result->x = click->x;
		result->y = click->y;
	}
	return result;
}

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
    resources = new ResourceManager();
    resources->autoLoadImages();
    resources->autoLoadMusic();

  	return true;
} //Simply initialization stuff for the SDL objects

//SDL_Surface* imageLoad(std::string filename, bool trans = true);

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

//a more modular version of the load_files() function
//jay kay

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
TTF_Font* openFont(std::string fontfile, int size=24) {
	font = TTF_OpenFont(fontfile.c_str(), size);
	return font;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect* clip = NULL);

void startup()
{
    if (!init() || !musicSystemInit()/* || !load_files()*/) {
        exit(0);
    }

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
	return (generator() % upper) + lower;
}

//GRAPHICAL STUFF


SDL_Surface* imageLoad(std::string filename, bool trans)
{
  	SDL_Surface *spriteX = NULL;
 	SDL_Surface *final = NULL;
  	spriteX = IMG_Load(filename.c_str());
  	if (spriteX != NULL)
  	{
      	final = SDL_DisplayFormat(spriteX);
      	SDL_FreeSurface(spriteX);
		if (final != NULL)
		{
		  	Uint32 colorkey = SDL_MapRGB(final->format, 0xFF,
						   0xFF, 0xFF);
			if (trans == true) {
				SDL_SetColorKey(final, SDL_RLEACCEL | SDL_SRCCOLORKEY,
				  colorkey);
			}
		}
        else {
            std::cerr << "SDL_FreeSurface goofed on " << filename << std::endl;
		}
    }
    else {
        std::cerr << "oops at " << filename;
    }
  	return final;
} //Loads the image at filename, if trans, will load like a sprite map

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface*
		   destination, SDL_Rect* clip)
{
  SDL_Rect Offset;
  Offset.x = x;
  Offset.y = y;

  SDL_BlitSurface(source, clip, destination, &Offset);
} //Puts source at x,y of destination, if clip is set it'll do this all in the bounds of clip

/*void clearBox(int x, int y, int w, int h) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	Uint32 colorkey = SDL_MapRGB(tile->format, 0xFF,
						   0xFF, 0xFF);
	SDL_FillRect(screen, &rect, colorkey);
}*/

void refresh(SDL_Surface* screen, int x = 0, int y = 0, int w = 0, int h = 0){
  SDL_UpdateRect(screen, x, y, w, h);
}
//refreshes the screen because I hate all the SDL_Flip == -1 shit

void makeText(std::string message, TTF_Font* hereFont= font, SDL_Color color = textC, int x = SCRN_WIDTH/2, int y = SCRN_HEIGHT/2)
{
  const char* pancake = message.c_str();
  txt = TTF_RenderText_Solid( hereFont, pancake, color);
  if (txt == NULL) {
    std::cerr << "Could not render:" << message;
  }
  apply_surface(x, y, txt, screen);
  SDL_FreeSurface(txt);
} //Puts message at x,y on the screen, neat stuff


void introAnimation() {
	Uint8 n = 0;
	SDL_Color rawr = {n,n,n};
	while (n < 256) {
		refresh(screen);
		//makeText(<REPLACE THIS WITH MESSAGE>, font, rawr);
		n++;
		rawr.r = rawr.g = rawr.b = n;
		SDL_Delay(5);
	}
}

void clearScreen() {
	Uint32 asdf = SDL_MapRGB(screen->format, 0xFF,
						   0xFF, 0xFF);
	if (SDL_FillRect(screen, NULL, asdf) == -1) {
		std::cerr<< "Could not clear the screen!" << endl;
	}
}

//SHUTDOWN STUFF

void clean_up()
{
  SDL_FreeSurface(screen);
  //SDL_FreeSurface(txt);
  TTF_CloseFont(font);
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
