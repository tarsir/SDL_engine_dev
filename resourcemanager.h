#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "general.h"

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


class ResourceManager {
	private:
		vector< Resource<SDL_Surface> > images;
		vector< Resource<Mix_Music> > music;
		vector< Resource<Mix_Chunk> > sound_fx;
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

		void autoLoadMusic() {
			vector<char*> fileList = buildListOfDirOrFile("./resources/music");
			for (vector<char*>::iterator it = fileList.begin(); it != fileList.end(); ++it) {
				char temp[100];
				//std::cout << "found something at " << *it << std::endl;
				strcpy(temp, "./resources/music/");
				strcat(temp, *it);
				Mix_Music* rsrc_music = NULL;
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

		Mix_Music* findMusicByCount(int position=-1) {
			if (position == -1) {
				position = currentMusic;
			}
			Resource< Mix_Music > musicRsrc = music[position%(music.size())];
			currentMusic++;
			return musicRsrc.getResource();
		}
};

#endif
