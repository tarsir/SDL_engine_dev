#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "general.h"
#include "time.h"

using std::string;
using std::vector;

template <typename T>
class Resource {
	private:
		string label;
		T* resource;

	public:
		Resource(string label_, T* rsrc) {
			label=label_;
			resource = rsrc;
		}

		~Resource() {
			//delete resource;
		}

		string getLabel() {
			return label;
		}

		T* getResource() {
			return resource;
		}
};

SDL_Surface* imageLoad(std::string filename, bool trans = true);
Mix_Music* musicLoad(std::string filename);
TTF_Font* openFont(std::string filename, int size=24);


class ResourceManager {
	private:
		vector< Resource<SDL_Surface> > images;
		vector< Resource<SDL_Surface> > sprites;
		vector< Resource<Mix_Music> > music;
		vector< Resource<Mix_Chunk> > sound_fx;
		vector< Resource<TTF_Font> > fonts;
	public:
		int currentMusic;
		ResourceManager() {
			currentMusic = 0;
		}

		void autoLoadImages() {
			vector<char*> fileList = buildListOfDirOrFile("./resources/images");
			for (vector<char*>::iterator it = fileList.begin(); it != fileList.end(); ++it) {
				char temp[100];
				//std::cout << "found something at " << *it << std::endl;
				strcpy(temp, "./resources/images/");
				strcat(temp,*it);
				SDL_Surface *rsrc_img = NULL;
				rsrc_img = imageLoad(temp);
				string rsrc_label = stripFileExtension(string(*it));
				if (rsrc_img == NULL) {
					printf("Couldn't load an image...\n");
				}
				Resource<SDL_Surface> *rsrc = new Resource<SDL_Surface>(rsrc_label, rsrc_img);
				images.push_back(*rsrc);
				std::cout << "made image resource: " << rsrc_label << std::endl;
				delete rsrc;
			}
		}

		void autoLoadFonts() {
			vector<char*> fileList = buildListOfDirOrFile("./resources/fonts");
			for (vector<char*>::iterator it = fileList.begin(); it!= fileList.end(); ++it) {
				char temp[100];
				strcpy(temp, "./resources/fonts/");
				strcat(temp, *it);
				TTF_Font *rsrc_font = NULL;
				rsrc_font = openFont(temp);
				string rsrc_label = stripFileExtension(string(*it));
				if (rsrc_font == NULL) {
					printf("Couldn't load the font...\n");
				}
				Resource<TTF_Font> *rsrc = new Resource<TTF_Font>(rsrc_label, rsrc_font);
				fonts.push_back(*rsrc);
				std::cout << "made font resource: " << rsrc_label << std::endl;
				delete rsrc;
			}
		}

		void autoLoadSprites() {
			vector<char*> fileList = buildListOfDirOrFile("./resources/sprites");
			for (vector<char*>::iterator it = fileList.begin(); it != fileList.end(); ++it) {
				char temp[100];

				strcpy(temp, "./resources/sprites/");
				strcat(temp, *it);
				SDL_Surface* rsrc_img = NULL;
				rsrc_img = imageLoad(temp);
				string rsrc_label = stripFileExtension(string(*it));
				if (rsrc_img == NULL) {
					printf("Couldn't load a sprite...\n");
				}
				Resource<SDL_Surface> *rsrc = new Resource<SDL_Surface> (rsrc_label, rsrc_img);
				sprites.push_back(*rsrc);
				std::cout << "made sprite resource: " << rsrc_label << std::endl;
				delete rsrc;
			}
		}

		void autoLoadMusic() {
			vector<char*> fileList = buildListOfDirOrFile("./resources/music");
			for (vector<char*>::iterator it = fileList.begin(); it != fileList.end(); ++it) {
				char temp[100];
				//std::cout << "found something at " << *it << std::endl;
				strcpy(temp, "./resources/music/");
				strcat(temp, *it);
				Mix_Music* rsrc_music = NULL;
				std::cout << "loading music from " << temp << std::endl;
				rsrc_music = musicLoad(temp);
				string rsrc_label = stripFileExtension(string(*it));
				Resource<Mix_Music> *rsrc = new Resource<Mix_Music>(rsrc_label, rsrc_music);
				music.push_back(*rsrc);
				std::cout << "made music resource: " << rsrc_label << std::endl;
				delete rsrc;
			}
		}


		int imageCount() {
			return this->images.size();
		}

		int musicCount() {
			return this->music.size();
		}

		//works!
		SDL_Surface* findImageByLabel(string targetLabel) {
			for (vector< Resource<SDL_Surface> >::iterator it = images.begin(); it < images.end(); it++) {
				if (((*it).getLabel()).compare(targetLabel) == 0) {
					std::cout << "Match on: " << ((*it).getLabel()) << " = " << targetLabel << std::endl;
					return ((*it).getResource());
				}
			}
			std::cout << "Could not match to label!\n";
			return NULL;
		}

		SDL_Surface* findSpriteByLabel(string targetLabel) {
			for (vector< Resource<SDL_Surface> >::iterator it = sprites.begin(); it < sprites.end(); it++) {
				if (((*it).getLabel()).compare(targetLabel) == 0) {
					std::cout << "Match on: " << ((*it).getLabel()) << " = " << targetLabel << std::endl;
					return ((*it).getResource());
				}
			}
			std::cout << "Could not match to label!\n";
			return NULL;
		}


		//doesn't work for some reason?
		Mix_Music* findMusicByCount(int position=-1) {
			if (position == -1) {
				position = currentMusic;
			}
			Resource< Mix_Music > musicRsrc = music[position%(music.size())];
			std::cout << "music is called " << musicRsrc.getLabel() << std::endl;
			currentMusic++;
			return musicRsrc.getResource();
		}

		//so use this one instead
		Resource<Mix_Music> findMusicRsrcByCount(int position = -1) {
			if (position == -1) {
				position = currentMusic;
			}
			Resource<Mix_Music> musicRsrc = music[position%(music.size())];
			std::cout << "music is called" << musicRsrc.getLabel() << std::endl;
			currentMusic++;
			return musicRsrc;
		}
};

#endif
