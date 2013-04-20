#ifndef _FILES_H_
#define _FILES_H_

#include "general.h"
using std::vector;
using std::string;

vector<char*> buildListOfDirOrFile(string directory = "", bool wantDirectory = false) {
	vector<char*> finalList;
	const char* enddir = string("./").append(directory).c_str(); //this syntax sucks
	DIR *dpdf = opendir(enddir);
	struct dirent *epdf;
	if (dpdf != NULL) {
		while (epdf = readdir(dpdf)) {
			//
			if (epdf->d_name[0] != '.') { //is not . or .. or hidden
				if (wantDirectory && epdf->d_type == DT_DIR) {
					finalList.push_back(epdf->d_name);
				}
				else if (!wantDirectory && epdf->d_type == DT_REG) {
					finalList.push_back(epdf->d_name);
				}
			}
		}
	}
	return finalList;
}

string stripFileExtension(string filename) {
	unsigned extLoc = filename.find_last_of(".");
	if (extLoc != string::npos) {
		return filename.substr(0, extLoc);
	}
	else return filename;
}

#endif
