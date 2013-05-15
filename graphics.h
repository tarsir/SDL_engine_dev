#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "general.h"

using std::endl;

extern TTF_Font* font;
extern SDL_Surface* screen;
extern SDL_Color textC;
extern const int SCRN_WIDTH;
extern const int SCRN_HEIGHT;


SDL_Surface* txt;

bool imageSystemInit() {
	int flags = IMG_INIT_JPG|IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted&flags) != flags) {
		printf("IMG_Init did not acquire JPG and PNG support.\n");
		return false;
	}
	return true;
}

SDL_Surface* imageLoad(std::string filename, bool trans)
{
  	SDL_Surface *spriteX = NULL;
 	SDL_Surface *final = NULL;
  	spriteX = IMG_Load(filename.c_str());
  	if (spriteX != NULL)
  	{
  		if (trans) {
			final = SDL_DisplayFormatAlpha(spriteX);
  		}
  		else {
  			final = SDL_DisplayFormat(spriteX);
  		}
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

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect* clip = NULL);

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

#endif
