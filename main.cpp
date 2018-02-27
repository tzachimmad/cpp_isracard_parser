//
//  main.cpp
//  Interview_prep
//
//  Created by Tzachi Lapidot on 27/02/2018.
//  Copyright © 2018 Tzachi Lapidot. All rights reserved.
//

#include <iostream>
#include <map>
#include <vector>
#include "Estab.hpp"
#include <dirent.h>
#include <fstream>

int write_to_file (std::string path) {
    
    std::fstream fs;
    fs.open (path, std::fstream::in | std::fstream::out | std::fstream::app);
    
    fs << " more lorem ipsum";
    
    fs.close();
    
    return 0;
}

void fill_file_list(std::string path, std::vector<std::string> &files)
{
    DIR           *dirp;
    struct dirent *directory;
    
    dirp = opendir(path.c_str());
    if (dirp)
    {
        while ((directory = readdir(dirp)) != NULL)
        {
            files.push_back(std::string(directory->d_name));
        }
        
        closedir(dirp);
    }
}

int main(int argc, const char * argv[]) {
    std::vector<std::string> files;
    std::map<std::string,Estab> estabMap;
    return 0;
}
