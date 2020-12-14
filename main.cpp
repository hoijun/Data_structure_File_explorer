#include "tool.cpp"
#include "sort.hpp"

#include <algorithm>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
#define MAX_PATH_LEN 1024

class sortfile { // 파일 정렬 클래스
  public:
    string tm;
    string username;
    string time;
    string filename;
    off_t size;
    sortfile(){}
    sortfile(string u, string t, string f, string m, off_t s) {
        username = u;
        time = t;
        filename = f;
        tm = m;
        size = s;
    }
};
int main() {
    while (1) {
        struct stat fileInfo;
        DIR *dirp;
        struct dirent *dirInfo;
        struct passwd *userInfo;
        vector<sortfile> Files; // 파일 담을 벡터
        char cwd[MAX_PATH_LEN + 1] = {
            '\0',
        };
        if (getcwd(cwd, MAX_PATH_LEN) == NULL) {
            perror("getcwd() error!");
            exit(-3);
        }
        if (stat(cwd, &fileInfo) == -1) {
            perror("stat() error!");
            exit(-4);
        }
        system("clear");
        frame();
        gotoxy(5, 4);
        printf("Current Directory: %s\n", cwd);
        dirp = opendir(cwd);
        while ((dirInfo = readdir(dirp)) != NULL) {
            if (stat(("%s/%s", cwd, dirInfo->d_name), &fileInfo) == -1) {
                perror("stat()	error!");
                exit(-6);
            }
            string Tm = fileTM(&fileInfo);
            userInfo = getpwuid(fileInfo.st_uid);
            Files.push_back(sortfile(
                userInfo->pw_name, timeToString(localtime(&fileInfo.st_mtime)),
                dirInfo->d_name, Tm, fileInfo.st_size));
        }
        sort(Files.begin(), Files.end(), comparen); // 파일 이름순 정렬
        int direct = 7;
        gotoxy(3, direct); // >의 처음 위치
        printf(">");
        printfile(Files); // 파일 출력
        while (1) {
            int n = keycontrol(); // 키보드 입력
            if (n == UP) {
                if (direct > 7) { // > 위로 이동
                    direct--;
                    gotoxy(3, direct + 1);
                    printf(" ");
                    gotoxy(3, direct);
                    printf(">");
                }
            } else if (n == DOWN) {
                if (direct < 24 && direct < Files.size() + 6) { // > 아래로 이동
                    direct++;
                    gotoxy(3, direct - 1);
                    printf(" ");
                    gotoxy(3, direct);
                    printf(">");
                }
            } else if (n == ENTER) {
                if (Files.at(direct - 7).tm.find("d") !=
                    string::npos) { // 디렉토리 파일일시
                    if (Files.at(direct - 7).filename.compare(".") == 0 ||
                        Files.at(direct - 7).filename.compare("..") ==
                            0) { //. .. 이면 그냥 이동
                        if (chdir(Files.at(direct - 7).filename.c_str()) ==
                            -1) {
                            perror("chdir()	error!");
                            exit(-1);
                        }
                    } else {
                        if (chdir(("./" + Files.at(direct - 7).filename)
                                      .c_str()) ==
                            -1) { // . .. 이 아니면 ./을 앞에 붙여서 이동
                            perror("chdir()	error!");
                            exit(-1);
                        }
                    }
                    break;
                }
            } else if (n == QUIT)
                exit(-1);
        }
    }
    return 0;
}
