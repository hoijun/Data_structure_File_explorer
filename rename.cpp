#include "rename.h"
#include "gotoxy.h"
#include <dirent.h>
#include <stdio.h>

int rename_main(std::string oldfilename) {
    char newfilename[100];
    gotoxy(3, 28);
    printf("새로운 파일 이름:  ");
    scanf("%s", newfilename);
    int nResult = rename(oldfilename.c_str(), newfilename);
    return 0;
}
