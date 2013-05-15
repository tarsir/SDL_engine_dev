#ifndef _MUSIC_H_
#define _MUSIC_H_

#include "general.h"
#include "SDL_mixer.h"

extern ResourceManager* resources;

//This function REQUIRES that SDL_Init has been called successfully
//and passed SDL_INIT_AUDIO (or SDL_INIT_EVERYTHING)
bool musicSystemInit() {
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }

	int mixFlags = MIX_INIT_MP3;
	int initted = Mix_Init(mixFlags);
	if (initted&mixFlags != mixFlags) {
		printf("Something went wrong with the mixer init!\n");
		return false;
	}
	return true;
}

Mix_Music* musicLoad(string filename) {
	Mix_Music* music = Mix_LoadMUS(filename.c_str());
	if (!music) {
		printf("\nCouldn't load music file: ");
		printf(filename.c_str());
		printf("\n");
		return NULL;
	}
	return music;
}

bool playNewMusic() {
	if (Mix_PlayingMusic() == 0) {
		Resource<Mix_Music> thisSong = resources->findMusicRsrcByCount();
		//std::cin.get();
		if (thisSong.getResource() != NULL) {
			if (Mix_PlayMusic(thisSong.getResource(),-1) == -1) {
				std::cin.get();
				return false;
			}
		}
	}
	return true;
}

#endif
