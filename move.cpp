#include "move.h"
#include "gotoxy.h"
#include <dirent.h>
#include <sstream>
#include <stdio.h>
#include <vector>

int move_main(std::string filename) {
    char path1[100];
    gotoxy(3, 29);
    printf("경로를 입력해주세요: ");
    scanf("%s", path1);
    std::istringstream ss(filename);
    std::string tempstring = path1;
    std::string stringBuffer;
    std::vector<std::string> temp;
    while (getline(ss, stringBuffer, ' ')) {
        temp.push_back(stringBuffer);
    }
    for (int i = 0; i < temp.size(); i++) {
        std::string realpath = "";
        realpath = tempstring + "/" + temp[i];
        rename(temp[i].c_str(), realpath.c_str());
    }
}
