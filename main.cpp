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
#define UP 0
#define DOWN 1
#define QUIT 99
using namespace std;

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
string fileTM(const struct stat *fileInfo); // 파일 권한
void gotoxy(int x, int y);                  // 좌표 이동
void frame();                               // 틀 출력
void printfile(vector<sortfile> f);         // 파일 출력
char *timeToString(struct tm *t);           // m_time 년도월 시분초 변경
bool comparen(sortfile a, sortfile b);      // 파일 이름순 정렬
bool comparet(sortfile a, sortfile b);      // 파일 시간순 정렬
int getch();      // 키보드 입력(원리 잘모름)
int keycontrol(); // 키보드 입력
int main() {
    struct stat fileInfo;
    DIR *dirp;
    struct dirent *dirInfo;
    struct passwd *userInfo;
    while (1) {
        /////////////변경
        vector<sortfile> Files; // 파일 담을 벡터
        // ListSort Files = ListSort();
        /////////////

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
            /////////////////변경
            Files.push_back(sortfile(
                userInfo->pw_name, timeToString(localtime(&fileInfo.st_mtime)),
                dirInfo->d_name, Tm, fileInfo.st_size));

            // Files.add(sortfile(
            //     userInfo->pw_name, fileInfo.st_mtime,
            //     dirInfo->d_name, Tm, fileInfo.st_size));
            ///////////////////
        }
        /////////////////변경
        sort(Files.begin(), Files.end(), comparen); // 파일 이름순 정렬
        // Files.sort(compareNameUp);
        //////////////////
        int direct = 7;
        gotoxy(3, direct); // >의 처음 위치
        printf(">");

        /////////////////////변경
        printfile(Files); // 파일 출력
        // printfile(Files.getList());
        //////////////////////
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

                // Files.size를 Files.listSize로

                if (direct < 24 && direct < Files.size() + 6) { // > 아래로 이동
                    direct++;
                    gotoxy(3, direct - 1);
                    printf(" ");
                    gotoxy(3, direct);
                    printf(">");
                }
            } else if (n == QUIT)
                exit(-1);
        }
    }
    return 0;
}

string fileTM(const struct stat *fileInfo) {
    string temp;
    if (S_ISREG(fileInfo->st_mode)) {
        temp += "-";
    } else if (S_ISDIR(fileInfo->st_mode)) {
        temp += "d";
    } else if (S_ISLNK(fileInfo->st_mode)) {
        temp += "l";
    } else if (S_ISSOCK(fileInfo->st_mode)) {
        temp += "s";
    } else if (S_ISFIFO(fileInfo->st_mode)) {
        temp += "FIFO";
    } else if (S_ISCHR(fileInfo->st_mode)) {
        temp += "c";
    } else if (S_ISBLK(fileInfo->st_mode)) {
        temp += "b";
    } else if (S_TYPEISMQ(fileInfo)) {
        temp += "?";
    } else if (S_TYPEISSEM(fileInfo)) {
        temp += "?";
    } else if (S_TYPEISSHM(fileInfo)) {
        temp += "?";
    }

    if (S_IRUSR & fileInfo->st_mode) {
        temp += "r";
    } else {
        temp += "-";
    }
    if (S_IWUSR & fileInfo->st_mode) {
        temp += "w";
    } else {
        temp += "-";
    }
    if (S_IXUSR & fileInfo->st_mode) {
        temp += "x";
    } else {
        temp += "-";
    }
    if (S_IRGRP & fileInfo->st_mode) {
        temp += "r";
    } else {
        temp += "-";
    }
    if (S_IWGRP & fileInfo->st_mode) {
        temp += "w";
    } else {
        temp += "-";
    }
    if (S_IXGRP & fileInfo->st_mode) {
        temp += "x";
    } else {
        temp += "-";
    }
    if (S_IROTH & fileInfo->st_mode) {
        temp += "r";
    } else {
        temp += "-";
    }
    if (S_IWOTH & fileInfo->st_mode) {
        temp += "w";
    } else {
        temp += "-";
    }
    if (S_IXOTH & fileInfo->st_mode) {
        temp += "x";
    } else {
        temp += "-";
    }
    return temp;
}
void gotoxy(int x, int y) {
    printf("\033[%d;%df", y, x);
    fflush(stdout);
}
void frame() {
    gotoxy(1, 1);
    printf("┌──────────────────────────────────────────────────────────────"
           "────"
           "─────────────┐\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│──────────────────────────────────────────────────────────────"
           "────"
           "─────────────│\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│──────────────────────────────────────────────────────────────"
           "────"
           "─────────────│\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("└──────────────────────────────────────────────────────────────"
           "────"
           "─────────────┘\n");
    gotoxy(5, 2);
    printf("File Explorer");
}
void printfile(vector<sortfile> f) {
    for (int i = 0; i < f.size(); i++) {
        if (i == 18)
            break;
        gotoxy(5, i + 7);
        printf("%s", f[i].tm.c_str());
        printf("      %-8s", f[i].username.c_str());
        printf("%8ld", f[i].size);
        printf("      %s", f[i].time.c_str());
        printf("      %s\n", f[i].filename.c_str());
    }
}
// void printfile(list<sortfile> f){
//     list<sortfile>::iterator it;
//     int i = 0;
//     for (it = f.begin(); it != f.end(); it++) {
//         if (i == 18)
//             break;
//         gotoxy(5, i + 7);
//         printf("%s", it->tm.c_str());
//         printf("      %-8s", it->username.c_str());
//         printf("%8ld", it->size);
//         printf("      %s", timeToString(localtime(it->time).c_str());
//         printf("      %s\n", it->filename.c_str());

//         i++;
//     }
// }
char *timeToString(struct tm *t) {
    static char s[20];
    sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900,
            t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    return s;
}
bool comparen(sortfile a, sortfile b) { return a.filename < b.filename; }
bool comparet(sortfile a, sortfile b) { return a.time < b.time; }
int getch() {
    int c;
    struct termios oldattr, newattr;
    tcgetattr(STDIN_FILENO, &oldattr); // 현재 터미널 설정 읽음
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO); // CANONICAL과 ECHO 끔
    newattr.c_cc[VMIN] = 1;  // 최소 입력 문자 수를 1로 설정
    newattr.c_cc[VTIME] = 0; // 최소 읽기 대기 시간을 0으로 설정
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // 터미널에 설정 입력
    c = getchar();                              // 키보드 입력 읽음
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // 원래의 설정으로 복구
    return c;
}
int keycontrol() {
    char t = getch();
    if (t == 'w') // w 입력시 위로
        return UP;
    else if (t == 's') // s 입력시 아래로
        return DOWN;
    else if (t == 'q') // q 입력시 종료
        return QUIT;
}
